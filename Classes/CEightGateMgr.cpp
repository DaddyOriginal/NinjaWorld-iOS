#include "CEightGateMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CEightGateMgr* CEightGateMgr::s_instance = NULL;

CEightGateMgr::CEightGateMgr()
    : m_soul(0)
    , m_gateLevel(1)
    , m_gateId(1)
    , m_costSoul(30)
    , m_costSilver(15000)
    , m_addAttack(0)
    , m_addDefense(0)
    , m_addChakra(0)
    , m_addAttackPer(0)
    , m_addDefensePer(0)
    , m_addChakraPer(0)
    , m_normalCostSilver(10000)
    , m_specialCostGold(35)
    , m_normalTimesLeft(3)
    , m_specialTimesLeft(20)
    , m_freeSpecialMultiTimes(2)
    , m_baseSoulGainNormal(50)
    , m_baseSoulGainSpecial(70)
    , m_pendingSoul(0)
    , m_currentMultiplier(1)
    , m_isTraining(false)
{
}

CEightGateMgr::~CEightGateMgr() {
    m_currentGateBonuses.clear();
}

CEightGateMgr* CEightGateMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CEightGateMgr();
    }
    return s_instance;
}

void CEightGateMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CEightGateMgr::requestGateInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_eight_gate");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CEightGateMgr::onEightGateResp));
    req->send();
    CCLog("[CEightGateMgr] Gui yeu cau thong tin Bat Mon Don Giap (/rl_w_eight_gate - Cmd 1)");
}

void CEightGateMgr::requestOpenGate() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_eight_gate");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "2");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CEightGateMgr::onOpenGateResp));
    req->send();
    CCLog("[CEightGateMgr] Gui yeu cau khai mo huyet Bat Mon (Cap %d, Huyet %d)", m_gateLevel, m_gateId);
}

void CEightGateMgr::onEightGateResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CEightGateMgr] Loi goi /rl_w_eight_gate: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseGateInfoXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationEightGateUpdated, NULL);
}

void CEightGateMgr::onOpenGateResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CEightGateMgr] Loi khai mo huyet: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseOpenGateXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationEightGateOpened, NULL);
}

void CEightGateMgr::parseGateInfoXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CEightGateMgr] Loi parse XML Bat Mon!");
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    // 1. Basic node
    XMLElement* basicElem = root->FirstChildElement("basic");
    if (basicElem) {
        XMLElement* soulElem = basicElem->FirstChildElement("soul");
        if (soulElem && soulElem->GetText()) m_soul = atoi(soulElem->GetText());

        XMLElement* gateLvElem = basicElem->FirstChildElement("gate_level");
        if (gateLvElem && gateLvElem->GetText()) m_gateLevel = atoi(gateLvElem->GetText());

        XMLElement* costSoulElem = basicElem->FirstChildElement("cost_soul");
        if (costSoulElem && costSoulElem->GetText()) m_costSoul = atoi(costSoulElem->GetText());

        XMLElement* costCoinElem = basicElem->FirstChildElement("cost_coin");
        if (costCoinElem && costCoinElem->GetText()) m_costSilver = atoi(costCoinElem->GetText());

        XMLElement* gateIdElem = basicElem->FirstChildElement("gate_id");
        if (gateIdElem) {
            m_gateId = gateIdElem->IntAttribute("id");
            m_currentGateBonuses.clear();
            XMLElement* itemElem = gateIdElem->FirstChildElement("item");
            while (itemElem) {
                int type = itemElem->IntAttribute("type");
                int val = itemElem->IntAttribute("value");
                m_currentGateBonuses.push_back(EightGateBonusItem(type, val));
                itemElem = itemElem->NextSiblingElement("item");
            }
        }
    }

    // 2. Attribute node
    XMLElement* attrElem = root->FirstChildElement("attribute");
    if (attrElem) {
        XMLElement* eAtt = attrElem->FirstChildElement("add_attack");
        if (eAtt && eAtt->GetText()) m_addAttack = atoi(eAtt->GetText());

        XMLElement* eDef = attrElem->FirstChildElement("add_defense");
        if (eDef && eDef->GetText()) m_addDefense = atoi(eDef->GetText());

        XMLElement* eCha = attrElem->FirstChildElement("add_chakala");
        if (eCha && eCha->GetText()) m_addChakra = atoi(eCha->GetText());

        XMLElement* eAttP = attrElem->FirstChildElement("add_attack_per");
        if (eAttP && eAttP->GetText()) m_addAttackPer = atoi(eAttP->GetText());

        XMLElement* eDefP = attrElem->FirstChildElement("add_defense_per");
        if (eDefP && eDefP->GetText()) m_addDefensePer = atoi(eDefP->GetText());

        XMLElement* eChaP = attrElem->FirstChildElement("add_chakala_per");
        if (eChaP && eChaP->GetText()) m_addChakraPer = atoi(eChaP->GetText());
    }

    CCLog("[CEightGateMgr] Parse thanh cong Bat Mon: Cap %d Huyet %d, Soul=%d, Cong=+%d, Thu=+%d, Chakra=+%d",
          m_gateLevel, m_gateId, m_soul, m_addAttack, m_addDefense, m_addChakra);
}

void CEightGateMgr::parseOpenGateXml(const std::string& xmlStr) {
    parseGateInfoXml(xmlStr);
}

// =============================================================
// LUYỆN HỒN (TRAIN SOUL)
// =============================================================
void CEightGateMgr::requestTrainSoulInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_eight_gate");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "3");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CEightGateMgr::onTrainSoulResp));
    req->send();
    CCLog("[CEightGateMgr] Gui yeu cau thong tin Luyen Hon (/rl_w_eight_gate - Cmd 3)");
}

void CEightGateMgr::requestDoTrain(bool isSpecial) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_eight_gate");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "4");
    req->setParam("traintype", isSpecial ? "2" : "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CEightGateMgr::onTrainSoulResp));
    req->send();
    CCLog("[CEightGateMgr] Gui yeu cau thuc hien Luyen Hon (%s)", isSpecial ? "Cao cap" : "Thuong");
}

void CEightGateMgr::requestMultiplySoul(bool isSpecial) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_eight_gate");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "5");
    req->setParam("multitype", isSpecial ? "2" : "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CEightGateMgr::onTrainSoulResp));
    req->send();
    CCLog("[CEightGateMgr] Gui yeu cau boi so Luyen Hon (%s)", isSpecial ? "Cao cap" : "Thuong");
}

void CEightGateMgr::requestCollectSoul() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_eight_gate");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "6");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CEightGateMgr::onTrainSoulResp));
    req->send();
    CCLog("[CEightGateMgr] Gui yeu cau thu thap Linh Hon");
}

void CEightGateMgr::onTrainSoulResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CEightGateMgr] Loi Luyen Hon: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseTrainSoulXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationTrainSoulUpdated, NULL);
}

void CEightGateMgr::parseTrainSoulXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* basicElem = root->FirstChildElement("basic");
    if (basicElem) {
        XMLElement* soulElem = basicElem->FirstChildElement("soul");
        if (soulElem && soulElem->GetText()) m_soul = atoi(soulElem->GetText());

        XMLElement* totalSoulElem = basicElem->FirstChildElement("total_soul");
        if (totalSoulElem && totalSoulElem->GetText()) m_soul = atoi(totalSoulElem->GetText());

        XMLElement* costCoinElem = basicElem->FirstChildElement("cost_coin");
        if (costCoinElem && costCoinElem->GetText()) m_normalCostSilver = atoi(costCoinElem->GetText());

        XMLElement* costCashElem = basicElem->FirstChildElement("cost_cash");
        if (costCashElem && costCashElem->GetText()) m_specialCostGold = atoi(costCashElem->GetText());

        XMLElement* leftNormElem = basicElem->FirstChildElement("left_count");
        if (leftNormElem && leftNormElem->GetText()) m_normalTimesLeft = atoi(leftNormElem->GetText());

        XMLElement* leftSpecElem = basicElem->FirstChildElement("left_count_ad");
        if (leftSpecElem && leftSpecElem->GetText()) m_specialTimesLeft = atoi(leftSpecElem->GetText());

        XMLElement* multiElem = basicElem->FirstChildElement("multiple_time");
        if (multiElem && multiElem->GetText()) m_currentMultiplier = atoi(multiElem->GetText());

        XMLElement* multiTimesElem = basicElem->FirstChildElement("multiple_times");
        if (multiTimesElem && multiTimesElem->GetText()) m_currentMultiplier = atoi(multiTimesElem->GetText());

        XMLElement* uncollectElem = basicElem->FirstChildElement("uncollect_soul");
        if (uncollectElem && uncollectElem->GetText()) {
            m_pendingSoul = atoi(uncollectElem->GetText());
            m_isTraining = (m_pendingSoul > 0);
        }

        XMLElement* soulTrainElem = basicElem->FirstChildElement("soul_train");
        if (soulTrainElem && soulTrainElem->GetText()) {
            m_pendingSoul = atoi(soulTrainElem->GetText());
            m_isTraining = true;
        }

        XMLElement* freeMultiElem = basicElem->FirstChildElement("free_left_count");
        if (freeMultiElem && freeMultiElem->GetText()) m_freeSpecialMultiTimes = atoi(freeMultiElem->GetText());
    }

    CCLog("[CEightGateMgr] Parse thanh cong Luyen Hon: Soul=%d, Pending=%d, Multi=x%d, NormalLeft=%d, SpecLeft=%d",
          m_soul, m_pendingSoul, m_currentMultiplier, m_normalTimesLeft, m_specialTimesLeft);
}
