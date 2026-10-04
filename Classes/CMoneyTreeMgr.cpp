#include "CMoneyTreeMgr.h"
#include "CRLRequest.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CMoneyTreeMgr* CMoneyTreeMgr::m_pSharedMgr = NULL;

CMoneyTreeMgr::CMoneyTreeMgr()
    : m_pTarget(NULL)
    , m_pSelector(NULL)
{
}

CMoneyTreeMgr::~CMoneyTreeMgr() {
}

CMoneyTreeMgr* CMoneyTreeMgr::sharedManager() {
    if (!m_pSharedMgr) {
        m_pSharedMgr = new CMoneyTreeMgr();
    }
    return m_pSharedMgr;
}

void CMoneyTreeMgr::purgeManager() {
    if (m_pSharedMgr) {
        delete m_pSharedMgr;
        m_pSharedMgr = NULL;
    }
}

void CMoneyTreeMgr::requestTreeInfo(CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_mtree");
    req->setMethod(CRLRequest::METHOD_GET);
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CMoneyTreeMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CMoneyTreeMgr] Gui yeu cau thong tin Cay Rung Tien (/rl_r_mtree)");
}

void CMoneyTreeMgr::requestSwingTree(CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_mtree");
    req->setMethod(CRLRequest::METHOD_POST);
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CMoneyTreeMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CMoneyTreeMgr] Gui yeu cau Rung Cay Tien (/rl_w_mtree)");
}

void CMoneyTreeMgr::onHttpRequestCompleted(CCObject* pSender) {
    CRLRequest* pRequest = dynamic_cast<CRLRequest*>(pSender);
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CMoneyTreeMgr] Loi ket noi mang: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    std::string response = pRequest->getResponseString();
    XMLDocument doc;
    if (doc.Parse(response.c_str()) != XML_SUCCESS) {
        CCLog("[CMoneyTreeMgr] Loi parse XML: %s", response.c_str());
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) {
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    const char* code = root->Attribute("code");
    if (code && strcmp(code, "0") != 0) {
        CCLog("[CMoneyTreeMgr] Server tra ve code=%s", code);
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    XMLElement* cfElem = root->FirstChildElement("cf");
    if (cfElem) {
        parseTreeInfoXml(response);
    } else {
        parseSwingXml(response);
    }

    if (m_pTarget && m_pSelector) {
        (m_pTarget->*m_pSelector)(this);
    }
}

void CMoneyTreeMgr::parseTreeInfoXml(const std::string& xmlStr) {
    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* cfElem = root->FirstChildElement("cf");
    if (cfElem) {
        XMLElement* tpd = cfElem->FirstChildElement("timesperday");
        XMLElement* lm = cfElem->FirstChildElement("low_multiple");
        XMLElement* tm = cfElem->FirstChildElement("top_multiple");
        XMLElement* sc = cfElem->FirstChildElement("shake_cost");
        XMLElement* fs = cfElem->FirstChildElement("free_shake");
        XMLElement* ss = cfElem->FirstChildElement("start_silver");

        if (tpd && tpd->GetText()) m_config.timesPerDay = atoi(tpd->GetText());
        if (lm && lm->GetText()) m_config.lowMultiple = atoi(lm->GetText());
        if (tm && tm->GetText()) m_config.topMultiple = atoi(tm->GetText());
        if (sc && sc->GetText()) m_config.shakeCost = atoi(sc->GetText());
        if (fs && fs->GetText()) m_config.freeShake = atoi(fs->GetText());
        if (ss && ss->GetText()) m_config.startSilver = atoi(ss->GetText());
    }

    XMLElement* userElem = root->FirstChildElement("user");
    if (userElem) {
        XMLElement* ut = userElem->FirstChildElement("usedTimes");
        XMLElement* ft = userElem->FirstChildElement("freeTimes");
        XMLElement* ls = userElem->FirstChildElement("latestSilver");
        XMLElement* lt = userElem->FirstChildElement("latestTimest");
        XMLElement* coinElem = userElem->FirstChildElement("coin");
        XMLElement* cashElem = userElem->FirstChildElement("cash");

        if (ut && ut->GetText()) m_user.usedTimes = atoi(ut->GetText());
        if (ft && ft->GetText()) m_user.freeTimes = atoi(ft->GetText());
        if (ls && ls->GetText()) m_user.latestSilver = atoi(ls->GetText());
        if (lt && lt->GetText()) m_user.latestTime = atoll(lt->GetText());
        if (coinElem && coinElem->GetText()) m_user.coin = atoi(coinElem->GetText());
        if (cashElem && cashElem->GetText()) m_user.cash = atoi(cashElem->GetText());

        if (CPlayerDataMgr::sharedManager()) {
            if (m_user.coin > 0) CPlayerDataMgr::sharedManager()->setSilver(m_user.coin);
            if (m_user.cash > 0) CPlayerDataMgr::sharedManager()->setGold(m_user.cash);
        }
    }

    CCLog("[CMoneyTreeMgr] Nap thong tin Cay Tien: Da rung %d/%d, Free: %d", m_user.usedTimes, m_config.timesPerDay, m_user.freeTimes);
}

void CMoneyTreeMgr::parseSwingXml(const std::string& xmlStr) {
    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* userElem = root->FirstChildElement("user");
    if (userElem) {
        XMLElement* ut = userElem->FirstChildElement("usedTimes");
        XMLElement* ft = userElem->FirstChildElement("freeTimes");
        XMLElement* ls = userElem->FirstChildElement("latestSilver");
        XMLElement* coinElem = userElem->FirstChildElement("coin");
        XMLElement* cashElem = userElem->FirstChildElement("cash");

        if (ut && ut->GetText()) m_user.usedTimes = atoi(ut->GetText());
        if (ft && ft->GetText()) m_user.freeTimes = atoi(ft->GetText());
        if (ls && ls->GetText()) m_user.latestSilver = atoi(ls->GetText());
        if (coinElem && coinElem->GetText()) m_user.coin = atoi(coinElem->GetText());
        if (cashElem && cashElem->GetText()) m_user.cash = atoi(cashElem->GetText());

        if (CPlayerDataMgr::sharedManager()) {
            if (m_user.coin > 0) CPlayerDataMgr::sharedManager()->setSilver(m_user.coin);
            if (m_user.cash > 0) CPlayerDataMgr::sharedManager()->setGold(m_user.cash);
        }
    }

    CCLog("[CMoneyTreeMgr] Ket qua rung Cay Tien: +%d Bac! Lượt: %d", m_user.latestSilver, m_user.usedTimes);
}
