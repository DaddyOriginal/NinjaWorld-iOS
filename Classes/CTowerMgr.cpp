#include "CTowerMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CTowerMgr* CTowerMgr::s_instance = NULL;

CTowerMgr::CTowerMgr()
    : m_selectedChapter(1)
    , m_currentChapter(1)
    , m_currentRound(1)
    , m_currentProcess(1)
    , m_maxChap(1)
    , m_remainReset(3)
{
    memset(&m_lastFightResult, 0, sizeof(m_lastFightResult));
    m_lastFightResult.isSuccess = true;
    m_lastFightResult.isWin = true;
}

CTowerMgr::~CTowerMgr() {
}

CTowerMgr* CTowerMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CTowerMgr();
    }
    return s_instance;
}

void CTowerMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CTowerMgr::requestTowerInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_dup");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "7000");

    req->setCallback(this, callfuncND_selector(CTowerMgr::onTowerInfoResp));
    req->send();
    CCLog("[CTowerMgr] Gui yeu cau thong tin Thap Thi Luyen /rl_r_dup (CMD 7000)");
}

void CTowerMgr::onTowerInfoResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CTowerMgr] Loi goi /rl_r_dup: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseTowerInfoXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification("kNotificationTowerInfoUpdated", NULL);
}

void CTowerMgr::parseTowerInfoXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    // Đọc thẻ <tower> hoặc <dup>
    XMLElement* towerElem = root->FirstChildElement("tower");
    if (!towerElem) {
        towerElem = root->FirstChildElement("dup");
    }

    if (towerElem) {
        const char* maxChapStr = towerElem->Attribute("max_chap");
        if (maxChapStr) m_maxChap = atoi(maxChapStr);

        const char* curChapStr = towerElem->Attribute("cur_chap");
        if (curChapStr) m_currentChapter = atoi(curChapStr);

        const char* curRoundStr = towerElem->Attribute("cur_round");
        if (curRoundStr) m_currentRound = atoi(curRoundStr);

        const char* curProcStr = towerElem->Attribute("cur_process");
        if (curProcStr) m_currentProcess = atoi(curProcStr);

        const char* resetStr = towerElem->Attribute("reset");
        if (resetStr) m_remainReset = atoi(resetStr);
    }

    if (m_maxChap < 1) m_maxChap = 1;
    if (m_currentChapter < 1) m_currentChapter = 1;
    if (m_currentProcess < 1) m_currentProcess = 1;

    CCLog("[CTowerMgr] Cap nhat Thap: MaxChap=%d, CurChap=%d, CurRound=%d, CurProcess=%d, ResetConLai=%d",
          m_maxChap, m_currentChapter, m_currentRound, m_currentProcess, m_remainReset);
}

void CTowerMgr::requestFightFloor(int chapterId, int roundId) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_dup");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "3100");
    req->setParam("ChapID", CCString::createWithFormat("%d", chapterId)->getCString());
    req->setParam("RoundID", CCString::createWithFormat("%d", roundId)->getCString());

    req->setCallback(this, callfuncND_selector(CTowerMgr::onFightFloorResp));
    req->send();
    CCLog("[CTowerMgr] Gui lenh khieu chien tang Thap /rl_w_dup (CMD 3100, Chap=%d, Round=%d)", chapterId, roundId);
}

void CTowerMgr::onFightFloorResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CTowerMgr] Loi goi khieu chien tang thap: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        m_lastFightResult.isSuccess = false;
        m_lastFightResult.isWin = false;
        CCNotificationCenter::sharedNotificationCenter()->postNotification("kNotificationTowerBattleCompleted", NULL);
        return;
    }

    parseFightResultXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification("kNotificationTowerBattleCompleted", NULL);
}

void CTowerMgr::parseFightResultXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    memset(&m_lastFightResult, 0, sizeof(m_lastFightResult));
    m_lastFightResult.isSuccess = true;
    m_lastFightResult.isWin = true;

    XMLElement* retElem = root->FirstChildElement("ret");
    if (retElem && retElem->GetText()) {
        if (atoi(retElem->GetText()) != 0) {
            m_lastFightResult.isWin = false;
        }
    }

    XMLElement* winElem = root->FirstChildElement("win");
    if (winElem && winElem->GetText()) {
        m_lastFightResult.isWin = (atoi(winElem->GetText()) == 1);
    }

    XMLElement* expElem = root->FirstChildElement("exp");
    if (expElem && expElem->GetText()) {
        m_lastFightResult.expGained = atoi(expElem->GetText());
    }

    XMLElement* silverElem = root->FirstChildElement("silver");
    if (silverElem && silverElem->GetText()) {
        m_lastFightResult.silverGained = atoi(silverElem->GetText());
    }

    XMLElement* nextProcElem = root->FirstChildElement("next_process");
    if (nextProcElem && nextProcElem->GetText()) {
        m_lastFightResult.nextProcess = atoi(nextProcElem->GetText());
        m_currentProcess = m_lastFightResult.nextProcess;
    }

    XMLElement* nextChapElem = root->FirstChildElement("next_chap");
    if (nextChapElem && nextChapElem->GetText()) {
        m_lastFightResult.nextChapter = atoi(nextChapElem->GetText());
        m_currentChapter = m_lastFightResult.nextChapter;
        if (m_currentChapter > m_maxChap) {
            m_maxChap = m_currentChapter;
        }
    }

    // Cập nhật ví tiền người chơi
    CPlayerDataMgr* pPlayer = CPlayerDataMgr::sharedManager();
    if (pPlayer) {
        pPlayer->firefly_SetSilver(pPlayer->firefly_GetSilver() + m_lastFightResult.silverGained);
    }

    CCLog("[CTowerMgr] Ket qua vuot thap: Win=%d, EXP+=%d, Bac+=%d, NextProcess=%d",
          m_lastFightResult.isWin ? 1 : 0, m_lastFightResult.expGained,
          m_lastFightResult.silverGained, m_currentProcess);
}

void CTowerMgr::requestSweepTower(int toChapter) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_dup");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "3102");
    req->setParam("ToChap", CCString::createWithFormat("%d", toChapter)->getCString());

    req->setCallback(this, callfuncND_selector(CTowerMgr::onSweepResp));
    req->send();
    CCLog("[CTowerMgr] Gui lenh can quet Thap /rl_w_dup (CMD 3102, ToChap=%d)", toChapter);
}

void CTowerMgr::onSweepResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CTowerMgr] Loi goi can quet thap");
        return;
    }
    requestTowerInfo();
}

void CTowerMgr::requestResetTower() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_dup");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "3103");

    req->setCallback(this, callfuncND_selector(CTowerMgr::onTowerInfoResp));
    req->send();
    CCLog("[CTowerMgr] Gui lenh lam moi Thap /rl_w_dup (CMD 3103)");
}
