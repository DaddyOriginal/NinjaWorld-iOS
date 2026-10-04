#include "CSaveTimeMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CSaveTimeMgr* CSaveTimeMgr::s_instance = NULL;

CSaveTimeMgr::CSaveTimeMgr()
    : m_chapter(1)
    , m_round(1)
    , m_ramenCount(0)
    , m_totalTrans(10)
    , m_usedTrans(0)
    , m_expPerRamen(500)
    , m_silverPerRamen(2000)
{
}

CSaveTimeMgr::~CSaveTimeMgr() {
}

CSaveTimeMgr* CSaveTimeMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CSaveTimeMgr();
    }
    return s_instance;
}

void CSaveTimeMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CSaveTimeMgr::requestInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_savetime");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CSaveTimeMgr::onSaveTimeInfoResp));
    req->send();
    CCLog("[CSaveTimeMgr] Gui yeu cau thong tin SaveTime (/rl_r_savetime - Cmd 1)");
}

void CSaveTimeMgr::requestExchange() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_savetime");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "2");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CSaveTimeMgr::onExchangeResp));
    req->send();
    CCLog("[CSaveTimeMgr] Gui yeu cau doi Ramen lay EXP & Bac");
}

void CSaveTimeMgr::onSaveTimeInfoResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CSaveTimeMgr] Loi goi /rl_r_savetime: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseInfoXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationSaveTimeUpdated, NULL);
}

void CSaveTimeMgr::onExchangeResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CSaveTimeMgr] Loi doi Ramen: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseExchangeXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationSaveTimeExchanged, NULL);
    requestInfo();
}

void CSaveTimeMgr::parseInfoXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* basicElem = root->FirstChildElement("basic");
    if (basicElem) {
        XMLElement* chapElem = basicElem->FirstChildElement("chapter");
        if (chapElem && chapElem->GetText()) m_chapter = atoi(chapElem->GetText());

        XMLElement* roundElem = basicElem->FirstChildElement("round");
        if (roundElem && roundElem->GetText()) m_round = atoi(roundElem->GetText());

        XMLElement* propElem = basicElem->FirstChildElement("prop_num");
        if (propElem && propElem->GetText()) m_ramenCount = atoi(propElem->GetText());

        XMLElement* totalElem = basicElem->FirstChildElement("total_trans");
        if (totalElem && totalElem->GetText()) m_totalTrans = atoi(totalElem->GetText());

        XMLElement* usedElem = basicElem->FirstChildElement("used_trans");
        if (usedElem && usedElem->GetText()) m_usedTrans = atoi(usedElem->GetText());

        XMLElement* expElem = basicElem->FirstChildElement("single_exp");
        if (expElem && expElem->GetText()) m_expPerRamen = atoi(expElem->GetText());

        XMLElement* coinElem = basicElem->FirstChildElement("single_coin");
        if (coinElem && coinElem->GetText()) m_silverPerRamen = atoi(coinElem->GetText());
    }

    CCLog("[CSaveTimeMgr] Dong bo SaveTime: Ramen=%d, Con lai=%d/%d, Exp/Ramen=%d, Silver/Ramen=%d",
          m_ramenCount, m_totalTrans - m_usedTrans, m_totalTrans, m_expPerRamen, m_silverPerRamen);
}

void CSaveTimeMgr::parseExchangeXml(const std::string& xmlStr) {
    parseInfoXml(xmlStr);
}
