#include "CFriendMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CFriendMgr* CFriendMgr::s_instance = NULL;

CFriendMgr::CFriendMgr() {
}

CFriendMgr::~CFriendMgr() {
    m_friendList.clear();
    m_searchResult.clear();
}

CFriendMgr* CFriendMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CFriendMgr();
    }
    return s_instance;
}

void CFriendMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CFriendMgr::requestFriendList(int page) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_friend");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Type", "1");
    req->setParam("Page", CCString::createWithFormat("%d", page)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CFriendMgr::onFriendListResp));
    req->send();
    CCLog("[CFriendMgr] Gui yeu cau danh sach ban be (/rl_r_friend)");
}

void CFriendMgr::requestSearchFriends(const std::string& keyword) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_friend");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Type", "1");
    req->setParam("Search", keyword);
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CFriendMgr::onSearchFriendResp));
    req->send();
    CCLog("[CFriendMgr] Tim kiem ban be: %s", keyword.c_str());
}

void CFriendMgr::requestAddFriend(int toUid) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_friend");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Type", "2");
    req->setParam("ToUid", CCString::createWithFormat("%d", toUid)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CFriendMgr::onFriendActionResp));
    req->send();
    CCLog("[CFriendMgr] Gui loi moi ket ban toi UID=%d", toUid);
}

void CFriendMgr::requestDeleteFriend(int toUid) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_friend");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Type", "1");
    req->setParam("ToUid", CCString::createWithFormat("%d", toUid)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CFriendMgr::onFriendActionResp));
    req->send();
    CCLog("[CFriendMgr] Xoa ban be UID=%d", toUid);
}

void CFriendMgr::requestAcceptFriend(int toUid) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_friend");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Type", "3");
    req->setParam("ToUid", CCString::createWithFormat("%d", toUid)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CFriendMgr::onFriendActionResp));
    req->send();
}

void CFriendMgr::requestRejectFriend(int toUid) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_friend");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("Type", "4");
    req->setParam("ToUid", CCString::createWithFormat("%d", toUid)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CFriendMgr::onFriendActionResp));
    req->send();
}

void CFriendMgr::onFriendListResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CFriendMgr] Loi danh sach ban be: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseFriendListXml(pRequest->getResponseString(), m_friendList);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationFriendListUpdated, NULL);
}

void CFriendMgr::onSearchFriendResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        return;
    }

    parseFriendListXml(pRequest->getResponseString(), m_searchResult);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationFriendSearchUpdated, NULL);
}

void CFriendMgr::onFriendActionResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        return;
    }

    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationFriendActionSuccess, NULL);
    requestFriendList(1);
}

void CFriendMgr::parseFriendListXml(const std::string& xmlStr, std::vector<FriendInfo>& outList) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    outList.clear();

    XMLElement* listElem = root->FirstChildElement("userlist");
    if (!listElem) return;

    XMLElement* userElem = listElem->FirstChildElement("user");
    while (userElem) {
        FriendInfo f;
        XMLElement* idElem = userElem->FirstChildElement("id");
        if (idElem && idElem->GetText()) f.playerId = atoi(idElem->GetText());

        XMLElement* nickElem = userElem->FirstChildElement("nick");
        if (nickElem && nickElem->GetText()) f.name = nickElem->GetText();

        XMLElement* lvElem = userElem->FirstChildElement("level");
        if (lvElem && lvElem->GetText()) f.level = atoi(lvElem->GetText());

        XMLElement* cpElem = userElem->FirstChildElement("attackhigh");
        if (cpElem && cpElem->GetText()) f.combatPower = atoi(cpElem->GetText());

        XMLElement* cElem = userElem->FirstChildElement("country");
        if (cElem && cElem->GetText()) f.country = atoi(cElem->GetText());

        XMLElement* ninjaElem = userElem->FirstChildElement("firstninja");
        if (ninjaElem && ninjaElem->GetText()) f.firstNinja = atoi(ninjaElem->GetText());

        XMLElement* starElem = userElem->FirstChildElement("firstninjastar");
        if (starElem && starElem->GetText()) f.firstNinjaStar = atoi(starElem->GetText());

        outList.push_back(f);
        userElem = userElem->NextSiblingElement("user");
    }

    CCLog("[CFriendMgr] Parse thanh cong %lu ban be!", (unsigned long)outList.size());
}
