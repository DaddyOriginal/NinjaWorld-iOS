#include "CRouletteMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CRouletteMgr* CRouletteMgr::s_instance = NULL;

CRouletteMgr::CRouletteMgr()
    : m_jackpot(100000)
    , m_score(0)
    , m_freeSpins(1)
    , m_timeRemaining(86400)
    , m_costOnce(50)
    , m_costTen(450)
{
    // Mặc định 12 ô phần thưởng
    for (int i = 1; i <= 12; ++i) {
        RouletteSlotConfig slot;
        slot.slotId = i;
        slot.itemId = i;
        slot.count = 1;
        slot.name = "Phần Thưởng";
        m_slots.push_back(slot);
    }
}

CRouletteMgr::~CRouletteMgr() {
    m_slots.clear();
    m_lastSpinResults.clear();
}

CRouletteMgr* CRouletteMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CRouletteMgr();
    }
    return s_instance;
}

void CRouletteMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CRouletteMgr::requestWheelInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_wheel");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "1401");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CRouletteMgr::onWheelInfoResp));
    req->send();
    CCLog("[CRouletteMgr] Gui yeu cau thong tin Vong Quay May Man (/rl_r_wheel)");
}

void CRouletteMgr::requestSpin(bool isTenTimes) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_wheel");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", isTenTimes ? "1401" : "1400");
    req->setParam("Type", isTenTimes ? "10" : "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CRouletteMgr::onSpinResp));
    req->send();
    CCLog("[CRouletteMgr] Gui yeu cau quay vong quay: %s", isTenTimes ? "10 Lan" : "1 Lan");
}

void CRouletteMgr::onWheelInfoResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CRouletteMgr] Loi goi /rl_r_wheel: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseWheelInfoXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationWheelInfoUpdated, NULL);
}

void CRouletteMgr::onSpinResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CRouletteMgr] Loi quay thuong: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseSpinResultXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationWheelSpinResult, NULL);
}

void CRouletteMgr::parseWheelInfoXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* infoElem = root->FirstChildElement("info");
    if (infoElem) {
        XMLElement* lastElem = infoElem->FirstChildElement("lasttime");
        if (lastElem && lastElem->GetText()) m_timeRemaining = atoi(lastElem->GetText());

        XMLElement* bigElem = infoElem->FirstChildElement("bigcash");
        if (bigElem && bigElem->GetText()) m_jackpot = atoi(bigElem->GetText());

        XMLElement* freeElem = infoElem->FirstChildElement("freetimes");
        if (freeElem && freeElem->GetText()) m_freeSpins = atoi(freeElem->GetText());

        XMLElement* jfElem = infoElem->FirstChildElement("jifeng");
        if (jfElem && jfElem->GetText()) m_score = atoi(jfElem->GetText());

        XMLElement* costElem = infoElem->FirstChildElement("costsingle");
        if (costElem && costElem->GetText()) m_costOnce = atoi(costElem->GetText());
    }

    CCLog("[CRouletteMgr] Nạp Vòng Quay: Jackpot=%d, Score=%d, Free=%d, Time=%d",
          m_jackpot, m_score, m_freeSpins, m_timeRemaining);
}

void CRouletteMgr::parseSpinResultXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    m_lastSpinResults.clear();

    XMLElement* awardListElem = root->FirstChildElement("award_list");
    if (awardListElem) {
        XMLElement* awardElem = awardListElem->FirstChildElement("award");
        while (awardElem) {
            RouletteSpinItem item;
            item.slotId = awardElem->IntAttribute("pos");
            item.itemId = awardElem->IntAttribute("dropid");
            item.count = awardElem->IntAttribute("num");
            if (item.count <= 0) item.count = 1;

            m_lastSpinResults.push_back(item);
            awardElem = awardElem->NextSiblingElement("award");
        }
    }

    XMLElement* scoreElem = root->FirstChildElement("newscore");
    if (scoreElem && scoreElem->GetText()) {
        m_score = atoi(scoreElem->GetText());
    }

    XMLElement* cashElem = root->FirstChildElement("bigcash");
    if (cashElem && cashElem->GetText()) {
        m_jackpot = atoi(cashElem->GetText());
    }

    CCLog("[CRouletteMgr] Quay xong! Nhan %lu vat pham, Diem moi=%d, Hu=%d",
          (unsigned long)m_lastSpinResults.size(), m_score, m_jackpot);
}
