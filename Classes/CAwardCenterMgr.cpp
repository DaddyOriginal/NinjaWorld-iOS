#include "CAwardCenterMgr.h"
#include "CRLRequest.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CAwardCenterMgr* CAwardCenterMgr::m_pSharedMgr = NULL;

CAwardCenterMgr::CAwardCenterMgr()
    : m_pTarget(NULL)
    , m_pSelector(NULL)
{
}

CAwardCenterMgr::~CAwardCenterMgr() {
    m_awardList.clear();
}

CAwardCenterMgr* CAwardCenterMgr::sharedManager() {
    if (!m_pSharedMgr) {
        m_pSharedMgr = new CAwardCenterMgr();
    }
    return m_pSharedMgr;
}

void CAwardCenterMgr::purgeManager() {
    if (m_pSharedMgr) {
        delete m_pSharedMgr;
        m_pSharedMgr = NULL;
    }
}

void CAwardCenterMgr::requestAwardList(CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_x_award_msg");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Cmd", "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CAwardCenterMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CAwardCenterMgr] Gui yeu cau lay danh sach qua thuong (/rl_x_award_msg?Cmd=1)");
}

void CAwardCenterMgr::requestClaimAward(int msgId, CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_x_award_msg");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Cmd", "2");
    req->setParam("MsgID", CCString::createWithFormat("%d", msgId)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CAwardCenterMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CAwardCenterMgr] Gui yeu cau nhan qua MsgID=%d (/rl_x_award_msg?Cmd=2)", msgId);
}

void CAwardCenterMgr::requestClaimAll(CCObject* pTarget, SEL_CallFuncO pSelector) {
    m_pTarget = pTarget;
    m_pSelector = pSelector;

    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_x_award_msg");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Cmd", "3");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CAwardCenterMgr::onHttpRequestCompleted));
    req->send();
    CCLog("[CAwardCenterMgr] Gui yeu cau nhan tat ca qua (/rl_x_award_msg?Cmd=3)");
}

void CAwardCenterMgr::onHttpRequestCompleted(CCObject* pSender) {
    CRLRequest* pRequest = dynamic_cast<CRLRequest*>(pSender);
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CAwardCenterMgr] Loi ket noi mang: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    std::string response = pRequest->getResponseString();
    XMLDocument doc;
    if (doc.Parse(response.c_str()) != XML_SUCCESS) {
        CCLog("[CAwardCenterMgr] Loi parse XML: %s", response.c_str());
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
        CCLog("[CAwardCenterMgr] Server tra ve code khac 0: %s", code);
        if (m_pTarget && m_pSelector) {
            (m_pTarget->*m_pSelector)(NULL);
        }
        return;
    }

    // Kiểm tra loại gói tin phản hồi
    XMLElement* msgListElem = root->FirstChildElement("msglist");
    XMLElement* awardElem = root->FirstChildElement("award");

    if (msgListElem) {
        // Gói tin nạp danh sách quà
        parseAwardListXml(response);
    } else if (awardElem) {
        // Gói tin nhận quà đơn hoặc nhận tất cả
        int coin = 0;
        int gold = 0;
        XMLElement* coinElem = awardElem->FirstChildElement("coin");
        XMLElement* cashElem = awardElem->FirstChildElement("cash");
        if (coinElem && coinElem->GetText()) coin = atoi(coinElem->GetText());
        if (cashElem && cashElem->GetText()) gold = atoi(cashElem->GetText());

        if (CPlayerDataMgr::sharedManager()) {
            if (coin > 0) CPlayerDataMgr::sharedManager()->addSilver(coin);
            if (gold > 0) CPlayerDataMgr::sharedManager()->addGold(gold);
        }

        // Tự động load lại danh sách sau khi nhận quà thành công
        requestAwardList(m_pTarget, m_pSelector);
        return;
    }

    if (m_pTarget && m_pSelector) {
        (m_pTarget->*m_pSelector)(this);
    }
}

void CAwardCenterMgr::parseAwardListXml(const std::string& xmlStr) {
    m_awardList.clear();

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* expiryElem = root->FirstChildElement("expiry");
    if (expiryElem && expiryElem->GetText()) {
        m_expiry = expiryElem->GetText();
    } else {
        m_expiry = "30 ngày";
    }

    XMLElement* msglistElem = root->FirstChildElement("msglist");
    if (!msglistElem) return;

    XMLElement* msgElem = msglistElem->FirstChildElement("msg");
    while (msgElem) {
        SAwardMsg msg;
        XMLElement* idElem = msgElem->FirstChildElement("id");
        XMLElement* tsElem = msgElem->FirstChildElement("ts");
        XMLElement* titleElem = msgElem->FirstChildElement("title");
        XMLElement* contentElem = msgElem->FirstChildElement("content");

        if (idElem && idElem->GetText()) msg.id = atoi(idElem->GetText());
        if (tsElem && tsElem->GetText()) msg.timestamp = atoll(tsElem->GetText());
        if (titleElem && titleElem->GetText()) msg.title = titleElem->GetText();
        if (contentElem && contentElem->GetText()) msg.content = contentElem->GetText();

        XMLElement* awardsElem = msgElem->FirstChildElement("awards");
        if (awardsElem) {
            XMLElement* itemElem = awardsElem->FirstChildElement("item");
            while (itemElem) {
                int type = itemElem->IntAttribute("type");
                int dropId = itemElem->IntAttribute("dropid");
                int count = itemElem->IntAttribute("count");
                if (count <= 0) count = 1;
                msg.awards.push_back(SAwardItem(type, dropId, count));
                itemElem = itemElem->NextSiblingElement("item");
            }
        }

        m_awardList.push_back(msg);
        msgElem = msgElem->NextSiblingElement("msg");
    }

    CCLog("[CAwardCenterMgr] Da nap %d thu chua phan thuong tu server", (int)m_awardList.size());
}
