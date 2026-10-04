#include "CChapterMgr.h"
#include "CPlayerDataMgr.h"
#include "CStageTableMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CChapterMgr* CChapterMgr::s_instance = NULL;

CChapterMgr::CChapterMgr()
    : m_selectedChapter(1)
    , m_selectedSection(1)
    , m_selectedRound(1)
    , m_currentChapter(1)
    , m_currentSection(1)
    , m_currentStage(1)
    , m_maxUnlockedChapter(1)
{
    memset(&m_lastFightResult, 0, sizeof(m_lastFightResult));
    m_lastFightResult.starRating = 3;
    m_lastFightResult.isSuccess = true;
    m_lastFightResult.isWin = true;

    // Mặc định luôn mở chương 1
    UnlockedChapterInfo defaultChap;
    defaultChap.chapterId = 1;
    defaultChap.hardLevel = 1;
    defaultChap.currentSection = 1;
    defaultChap.currentSubround = 1;
    defaultChap.maxSections = 10;
    m_unlockedChapters[1] = defaultChap;
}

CChapterMgr::~CChapterMgr() {
    m_unlockedChapters.clear();
}

CChapterMgr* CChapterMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CChapterMgr();
    }
    return s_instance;
}

void CChapterMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CChapterMgr::setSelectedChapter(int chap) {
    m_selectedChapter = chap;
    if (m_selectedChapter < 1) m_selectedChapter = 1;
}

void CChapterMgr::setSelectedSection(int sec) {
    m_selectedSection = sec;
    if (m_selectedSection < 1) m_selectedSection = 1;
}

bool CChapterMgr::isChapterUnlocked(int chapterId) const {
    if (chapterId <= 1) return true;
    return m_unlockedChapters.find(chapterId) != m_unlockedChapters.end();
}

bool CChapterMgr::isSectionUnlocked(int chapterId, int sectionId) const {
    if (!isChapterUnlocked(chapterId)) return false;
    if (chapterId < m_currentChapter) return true;
    if (chapterId == m_currentChapter) {
        return sectionId <= m_currentSection;
    }
    return false;
}

void CChapterMgr::requestDungeonInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_adventure");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "1400");
    req->setParam("ChapID", CCString::createWithFormat("%d", m_selectedChapter)->getCString());
    req->setParam("StageID", CCString::createWithFormat("%d", m_selectedSection)->getCString());

    req->setCallback(this, callfuncND_selector(CChapterMgr::onDungeonInfoResp));
    req->send();
    CCLog("[CChapterMgr] Gui yeu cau thong tin ai PVE /rl_r_adventure (Chap=%d, Sec=%d)", m_selectedChapter, m_selectedSection);
}

void CChapterMgr::onDungeonInfoResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CChapterMgr] Loi goi /rl_r_adventure: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseDungeonInfoXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification("kNotificationDungeonInfoUpdated", NULL);
}

void CChapterMgr::parseDungeonInfoXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CChapterMgr] Loi parse XML dungeon info");
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    // 1. Đọc node <normal>
    XMLElement* normalElem = root->FirstChildElement("normal");
    if (normalElem) {
        XMLElement* chapIdElem = normalElem->FirstChildElement("chapid");
        if (chapIdElem && chapIdElem->GetText()) {
            m_currentChapter = atoi(chapIdElem->GetText());
        }

        XMLElement* secIdElem = normalElem->FirstChildElement("roundid");
        if (secIdElem && secIdElem->GetText()) {
            m_currentSection = atoi(secIdElem->GetText());
        }

        XMLElement* subIdElem = normalElem->FirstChildElement("subroundid");
        if (subIdElem && subIdElem->GetText()) {
            m_currentStage = atoi(subIdElem->GetText());
        }

        if (m_currentChapter > m_maxUnlockedChapter) {
            m_maxUnlockedChapter = m_currentChapter;
        }
    }

    // 2. Đọc node <chaplist>
    XMLElement* listElem = root->FirstChildElement("chaplist");
    if (listElem) {
        XMLElement* chapNode = listElem->FirstChildElement("chap");
        while (chapNode) {
            UnlockedChapterInfo info;
            memset(&info, 0, sizeof(info));

            XMLElement* idElem = chapNode->FirstChildElement("id");
            if (idElem && idElem->GetText()) info.chapterId = atoi(idElem->GetText());

            XMLElement* hardElem = chapNode->FirstChildElement("hardLevel");
            if (hardElem && hardElem->GetText()) info.hardLevel = atoi(hardElem->GetText());

            XMLElement* roundElem = chapNode->FirstChildElement("roundid");
            if (roundElem && roundElem->GetText()) info.currentSection = atoi(roundElem->GetText());

            XMLElement* subElem = chapNode->FirstChildElement("subroundid");
            if (subElem && subElem->GetText()) info.currentSubround = atoi(subElem->GetText());

            XMLElement* maxElem = chapNode->FirstChildElement("maxround");
            if (maxElem && maxElem->GetText()) info.maxSections = atoi(maxElem->GetText());

            if (info.chapterId > 0) {
                m_unlockedChapters[info.chapterId] = info;
                if (info.chapterId > m_maxUnlockedChapter) {
                    m_maxUnlockedChapter = info.chapterId;
                }
            }

            chapNode = chapNode->NextSiblingElement("chap");
        }
    }

    CCLog("[CChapterMgr] Cap nhat tien do: CurChapter=%d, CurSection=%d, CurStage=%d, MaxChapter=%d",
          m_currentChapter, m_currentSection, m_currentStage, m_maxUnlockedChapter);
}

void CChapterMgr::requestBattle(int chapterId, int sectionId, int roundId, bool isBoss) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_pve");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "3");
    req->setParam("Action", "spin");
    req->setParam("ChapID", CCString::createWithFormat("%d", chapterId)->getCString());
    req->setParam("StageID", CCString::createWithFormat("%d", sectionId)->getCString());
    req->setParam("Pos", CCString::createWithFormat("%d", roundId)->getCString());
    if (isBoss) {
        req->setParam("WastCashBoss", "0");
    }

    req->setCallback(this, callfuncND_selector(CChapterMgr::onBattleResp));
    req->send();
    CCLog("[CChapterMgr] Gui lenh vuot ai PVE /rl_w_pve (Chap=%d, Sec=%d, Pos=%d, Boss=%d)",
          chapterId, sectionId, roundId, isBoss ? 1 : 0);
}

void CChapterMgr::onBattleResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CChapterMgr] Loi goi tran dau /rl_w_pve: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        m_lastFightResult.isSuccess = false;
        m_lastFightResult.isWin = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification("kNotificationBattleCompleted", NULL);
        return;
    }

    parseBattleResultXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification("kNotificationBattleCompleted", NULL);
}

void CChapterMgr::parseBattleResultXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CChapterMgr] Loi parse XML tran dau");
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    memset(&m_lastFightResult, 0, sizeof(m_lastFightResult));
    m_lastFightResult.isSuccess = true;
    m_lastFightResult.isWin = true;
    m_lastFightResult.starRating = 3;

    // Kiểm tra ret
    XMLElement* retElem = root->FirstChildElement("ret");
    if (retElem && retElem->GetText()) {
        int ret = atoi(retElem->GetText());
        if (ret != 0) {
            m_lastFightResult.isWin = false;
        }
    }

    // Đọc thẻ <pve>
    XMLElement* pveElem = root->FirstChildElement("pve");
    if (pveElem) {
        const char* curAction = pveElem->Attribute("curaction");
        if (curAction) {
            m_lastFightResult.remainingStamina = atoi(curAction);
            CPlayerDataMgr::sharedManager()->firefly_SetBodyValue(m_lastFightResult.remainingStamina);
        }

        const char* nextChap = pveElem->Attribute("nextchap");
        if (nextChap) m_lastFightResult.nextChapter = atoi(nextChap);

        const char* nextSec = pveElem->Attribute("nextstage");
        if (nextSec) m_lastFightResult.nextSection = atoi(nextSec);

        const char* nextPos = pveElem->Attribute("nextpos");
        if (nextPos) m_lastFightResult.nextStage = atoi(nextPos);

        const char* addExp = pveElem->Attribute("addexp");
        if (addExp) m_lastFightResult.expGained = atoi(addExp);

        const char* addSilver = pveElem->Attribute("addsilver");
        if (addSilver) m_lastFightResult.silverGained = atoi(addSilver);

        const char* addGold = pveElem->Attribute("addgold");
        if (addGold) m_lastFightResult.goldGained = atoi(addGold);
    }

    // Đọc thẻ <award>
    XMLElement* awardElem = root->FirstChildElement("award");
    if (awardElem) {
        const char* awardType = awardElem->Attribute("type");
        const char* awardName = awardElem->Attribute("name");
        if (awardName) {
            m_lastFightResult.awardDesc = awardName;
        } else if (awardType) {
            int at = atoi(awardType);
            if (at == 2) m_lastFightResult.awardDesc = "Bạc x2";
            else if (at == 3) m_lastFightResult.awardDesc = "EXP x2";
            else if (at == 1) m_lastFightResult.awardDesc = "Rơi Thẻ Ninja";
            else if (at == 5) m_lastFightResult.awardDesc = "Vàng +10";
            else m_lastFightResult.awardDesc = "Hoàn thành ải";
        }
    }

    // Cập nhật PlayerDataMgr
    CPlayerDataMgr* pPlayer = CPlayerDataMgr::sharedManager();
    if (pPlayer) {
        pPlayer->firefly_SetSilver(pPlayer->firefly_GetSilver() + m_lastFightResult.silverGained);
        pPlayer->firefly_SetGold(pPlayer->firefly_GetGold() + m_lastFightResult.goldGained);
    }

    // Tiến độ tiếp theo
    if (m_lastFightResult.nextChapter > 0) {
        m_currentChapter = m_lastFightResult.nextChapter;
        if (m_currentChapter > m_maxUnlockedChapter) {
            m_maxUnlockedChapter = m_currentChapter;
        }
    }
    if (m_lastFightResult.nextSection > 0) {
        m_currentSection = m_lastFightResult.nextSection;
    }
    if (m_lastFightResult.nextStage > 0) {
        m_currentStage = m_lastFightResult.nextStage;
    }

    CCLog("[CChapterMgr] Ket qua tran dau: Thang=%d, Sao=%d, EXP+=%d, Bac+=%d, TheLucConLai=%d",
          m_lastFightResult.isWin ? 1 : 0, m_lastFightResult.starRating,
          m_lastFightResult.expGained, m_lastFightResult.silverGained,
          m_lastFightResult.remainingStamina);
}
