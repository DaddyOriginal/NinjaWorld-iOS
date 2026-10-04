#include "CGrowthFundMgr.h"
#include "CRLRequest.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CGrowthFundMgr* CGrowthFundMgr::m_pSharedMgr = NULL;

CGrowthFundMgr::CGrowthFundMgr()
    : m_pTarget(NULL)
    , m_pSelector(NULL)
{
}

CGrowthFundMgr::~CGrowthFundMgr() {
    m_milestones.clear();
}

CGrowthFundMgr* CGrowthFundMgr::sharedManager() {
    if (!m_pSharedMgr) {
        m_pSharedMgr = new CGrowthFundMgr();
    }
    return m_pSharedMgr;
}

void CGrowthFundMgr::purgeManager() {
    if (m_pSharedMgr) {
        delete m_pSharedMgr;
        m_pSharedMgr = NULL;
    }
}

void CGrowthFundMgr::requestFundInfo(CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_x_small_activity");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Cmd", "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CGrowthFundMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CGrowthFundMgr] Gui yeu cau lay thong tin Quy Truong Thanh (/rl_x_small_activity?Cmd=1)");
}

void CGrowthFundMgr::requestBuyFund(CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_x_small_activity");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Cmd", "2");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CGrowthFundMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CGrowthFundMgr] Gui yeu cau mua Quy Truong Thanh (/rl_x_small_activity?Cmd=2)");
}

void CGrowthFundMgr::requestClaimMilestone(int id, CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_x_small_activity");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Cmd", "3");
    req->setParam("Id", CCString::createWithFormat("%d", id)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CGrowthFundMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CGrowthFundMgr] Gui yeu cau nhan thuong moc Quy Id=%d (/rl_x_small_activity?Cmd=3)", id);
}

void CGrowthFundMgr::onHttpRequestCompleted(CCObject* pSender) {
    CRLRequest* pRequest = dynamic_cast<CRLRequest*>(pSender);
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CGrowthFundMgr] Loi ket noi mang: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    std::string response = pRequest->getResponseString();
    parseFundXml(response);

    if (m_pTarget && m_pSelector) {
        (m_pTarget->*m_pSelector)(this);
    }
}

void CGrowthFundMgr::parseFundXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CGrowthFundMgr] Loi parse XML: %s", xmlStr.c_str());
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    const char* code = root->Attribute("code");
    if (code && strcmp(code, "0") != 0) {
        CCLog("[CGrowthFundMgr] Server tra ve ma loi code=%s", code);
        return;
    }

    // Phân tích preview
    XMLElement* prevElem = root->FirstChildElement("preview");
    if (prevElem) {
        if (prevElem->Attribute("buycash")) m_preview.buyCash = prevElem->IntAttribute("buycash");
        if (prevElem->Attribute("multiple")) m_preview.multiple = prevElem->IntAttribute("multiple");
        if (prevElem->Attribute("totalcash")) m_preview.totalCash = prevElem->IntAttribute("totalcash");
        if (prevElem->Attribute("buystate")) m_preview.buyState = prevElem->IntAttribute("buystate");
        if (prevElem->Attribute("needvip")) m_preview.needVip = prevElem->IntAttribute("needvip");

        // Nếu là gói phản hồi nhận thưởng mốc (chứa preview cash và buystate)
        int cashReward = prevElem->IntAttribute("cash");
        if (cashReward > 0 && CPlayerDataMgr::sharedManager()) {
            CPlayerDataMgr::sharedManager()->addGold(cashReward);
        }
    }

    // Phân tích danh sách các mốc
    XMLElement* fundListElem = root->FirstChildElement("fundlist");
    if (fundListElem) {
        m_milestones.clear();
        XMLElement* itemElem = fundListElem->FirstChildElement("item");
        while (itemElem) {
            SGrowthFundItem item;
            item.id = itemElem->IntAttribute("id");
            item.level = itemElem->IntAttribute("level");
            item.cash = itemElem->IntAttribute("cash");
            item.state = itemElem->IntAttribute("state");

            char titleBuf[128];
            snprintf(titleBuf, sizeof(titleBuf), "Cấp %d", item.level);
            item.title = titleBuf;

            char descBuf[256];
            snprintf(descBuf, sizeof(descBuf), "Nhân vật đạt cấp %d nhận %d Vàng", item.level, item.cash);
            item.desc = descBuf;

            m_milestones.push_back(item);
            itemElem = itemElem->NextSiblingElement("item");
        }
        CCLog("[CGrowthFundMgr] Da cap nhat %d moc thuong Quy Truong Thanh", (int)m_milestones.size());
    } else {
        // Sau khi nhận quà mốc đơn lẻ, tự động làm mới danh sách
        requestFundInfo(m_pTarget, m_pSelector);
    }
}
