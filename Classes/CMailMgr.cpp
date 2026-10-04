#include "CMailMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CMailMgr* CMailMgr::s_instance = NULL;

CMailMgr::CMailMgr()
    : m_currentTab(3)
{
}

CMailMgr::~CMailMgr() {
    m_mailList.clear();
}

CMailMgr* CMailMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CMailMgr();
    }
    return s_instance;
}

void CMailMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CMailMgr::requestMailList(int tab) {
    m_currentTab = tab;
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_msg");
    req->setMethod(CRLRequest::METHOD_POST);

    int cmd = 1802; // Mặc định: Hệ thống
    if (tab == 1) cmd = 1800; // Chiến báo
    else if (tab == 2) cmd = 1801; // Bạn bè

    req->setParam("cmd", CCString::createWithFormat("%d", cmd)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CMailMgr::onMailListResp));
    req->send();
    CCLog("[CMailMgr] Gui yeu cau danh sach hop thu (/rl_r_msg - Cmd %d)", cmd);
}

void CMailMgr::onMailListResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CMailMgr] Loi goi /rl_r_msg: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseMailListXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationMailListUpdated, NULL);
}

void CMailMgr::parseMailListXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CMailMgr] Loi parse XML hop thu!");
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    m_mailList.clear();

    XMLElement* msgElem = root->FirstChildElement("msg");
    while (msgElem) {
        MailItemData mail;
        XMLElement* idElem = msgElem->FirstChildElement("ID");
        if (idElem && idElem->GetText()) mail.id = atoi(idElem->GetText());

        XMLElement* subElem = msgElem->FirstChildElement("subtype");
        if (subElem && subElem->GetText()) mail.subType = atoi(subElem->GetText());

        XMLElement* fromNickElem = msgElem->FirstChildElement("fromnick");
        if (fromNickElem && fromNickElem->GetText()) mail.senderName = fromNickElem->GetText();

        XMLElement* textElem = msgElem->FirstChildElement("text");
        if (textElem && textElem->GetText()) mail.content = textElem->GetText();

        XMLElement* coinElem = msgElem->FirstChildElement("iCoin");
        if (coinElem && coinElem->GetText()) mail.silverReward = atoi(coinElem->GetText());

        XMLElement* statusElem = msgElem->FirstChildElement("iStatus");
        if (statusElem && statusElem->GetText()) mail.status = atoi(statusElem->GetText());

        XMLElement* takeElem = msgElem->FirstChildElement("iTakeRes");
        if (takeElem && takeElem->GetText()) {
            int taken = atoi(takeElem->GetText());
            if (taken > 0) mail.status = 2; // Đã nhận
        }

        if (mail.silverReward > 0 || mail.goldReward > 0) {
            mail.hasReward = true;
        }

        m_mailList.push_back(mail);
        msgElem = msgElem->NextSiblingElement("msg");
    }

    CCLog("[CMailMgr] Dong bo thanh cong %lu thu tu may chu!", (unsigned long)m_mailList.size());
}

void CMailMgr::requestClaimMail(int mailId) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_msg");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "2901");
    req->setParam("mailid", CCString::createWithFormat("%d", mailId)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CMailMgr::onClaimMailResp));
    req->send();
    CCLog("[CMailMgr] Gui yeu cau nhan thuong thu ID=%d", mailId);
}

void CMailMgr::requestDeleteMail(int mailId) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_msg");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "2901");
    req->setParam("Op", "1");
    req->setParam("mailid", CCString::createWithFormat("%d", mailId)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->send();
    CCLog("[CMailMgr] Gui yeu cau xoa thu ID=%d", mailId);
}

void CMailMgr::onClaimMailResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CMailMgr] Loi nhan thu: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseClaimMailXml(pRequest->getResponseString(), 0);
}

void CMailMgr::parseClaimMailXml(const std::string& xmlStr, int mailId) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* awardElem = root->FirstChildElement("award");
    if (awardElem) {
        int coin = awardElem->IntAttribute("coin");
        int cash = awardElem->IntAttribute("cash");

        if (CPlayerDataMgr::sharedManager()) {
            if (coin > 0) CPlayerDataMgr::sharedManager()->firefly_AddSilver(coin);
            if (cash > 0) CPlayerDataMgr::sharedManager()->firefly_AddGold(cash);
        }
        CCLog("[CMailMgr] Nhan thuong thu thanh cong: +%d Bac, +%d Vang!", coin, cash);
    }

    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationMailClaimed, NULL);
    requestMailList(m_currentTab);
}

int CMailMgr::getUnreadCount() const {
    int count = 0;
    for (size_t i = 0; i < m_mailList.size(); ++i) {
        if (m_mailList[i].status == 0) {
            count++;
        }
    }
    return count;
}
