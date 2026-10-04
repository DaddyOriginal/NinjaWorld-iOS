#include "CLimitTrainSoulView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CLimitTrainSoulView::CLimitTrainSoulView()
    : m_pLabelCurSoul(NULL)
    , m_pLabelTrainSoul(NULL)
    , m_pLabelTrainSoulAll(NULL)
    , m_pLabelMultiple(NULL)
    , m_pLabelNormalCostSilver(NULL)
    , m_pLabelSpecialCostGold(NULL)
    , m_pLabelCostSilver(NULL)
    , m_pLabelCostGold(NULL)
    , m_pLabelCostDescNormal(NULL)
    , m_pLabelCostDescSpecial(NULL)
    , m_pLabelFreeMutiTimes(NULL)
    , m_pLabelCanGetSoulNormal(NULL)
    , m_pLabelCanGetSoulSpecial(NULL)
    , m_pLabelGold(NULL)
    , m_pLabelSilver(NULL)
    , m_pBtnBack(NULL)
    , m_pBtnGet(NULL)
    , m_pBtnGoldMuti(NULL)
    , m_pBtnSilverMuti(NULL)
    , m_pBtnTrainNormal(NULL)
    , m_pBtnTrainSpecial(NULL)
    , m_pNodePreTrain(NULL)
    , m_pNodeAfterTrain(NULL)
    , m_pSprGoldIcon(NULL)
{
}

CLimitTrainSoulView::~CLimitTrainSoulView() {
    CC_SAFE_RELEASE_NULL(m_pLabelCurSoul);
    CC_SAFE_RELEASE_NULL(m_pLabelTrainSoul);
    CC_SAFE_RELEASE_NULL(m_pLabelTrainSoulAll);
    CC_SAFE_RELEASE_NULL(m_pLabelMultiple);
    CC_SAFE_RELEASE_NULL(m_pLabelNormalCostSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelSpecialCostGold);
    CC_SAFE_RELEASE_NULL(m_pLabelCostSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelCostGold);
    CC_SAFE_RELEASE_NULL(m_pLabelCostDescNormal);
    CC_SAFE_RELEASE_NULL(m_pLabelCostDescSpecial);
    CC_SAFE_RELEASE_NULL(m_pLabelFreeMutiTimes);
    CC_SAFE_RELEASE_NULL(m_pLabelCanGetSoulNormal);
    CC_SAFE_RELEASE_NULL(m_pLabelCanGetSoulSpecial);
    CC_SAFE_RELEASE_NULL(m_pLabelGold);
    CC_SAFE_RELEASE_NULL(m_pLabelSilver);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pBtnGet);
    CC_SAFE_RELEASE_NULL(m_pBtnGoldMuti);
    CC_SAFE_RELEASE_NULL(m_pBtnSilverMuti);
    CC_SAFE_RELEASE_NULL(m_pBtnTrainNormal);
    CC_SAFE_RELEASE_NULL(m_pBtnTrainSpecial);
    CC_SAFE_RELEASE_NULL(m_pNodePreTrain);
    CC_SAFE_RELEASE_NULL(m_pNodeAfterTrain);
    CC_SAFE_RELEASE_NULL(m_pSprGoldIcon);

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationTrainSoulUpdated);
}

CLimitTrainSoulView* CLimitTrainSoulView::create() {
    CLimitTrainSoulView* pView = new CLimitTrainSoulView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CLimitTrainSoulView::init() {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("LimitTrainSoulView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/LimitTrainSoulView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CLimitTrainSoulView::onTrainSoulUpdated), kNotificationTrainSoulUpdated, NULL);

    return true;
}

void CLimitTrainSoulView::onEnter() {
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

    CEightGateMgr::sharedManager()->requestTrainSoulInfo();
}

void CLimitTrainSoulView::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool CLimitTrainSoulView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void CLimitTrainSoulView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CLimitTrainSoulView::updateUI() {
    CEightGateMgr* pMgr = CEightGateMgr::sharedManager();
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();

    if (m_pLabelCurSoul) {
        std::stringstream ss;
        ss << pMgr->getSoul();
        m_pLabelCurSoul->setString(ss.str().c_str());
    }

    if (pData) {
        if (m_pLabelSilver) {
            std::stringstream ss;
            ss << pData->firefly_GetSilver();
            m_pLabelSilver->setString(ss.str().c_str());
        }
        if (m_pLabelGold) {
            std::stringstream ss;
            ss << pData->firefly_GetGold();
            m_pLabelGold->setString(ss.str().c_str());
        }
    }

    if (m_pLabelNormalCostSilver) {
        std::stringstream ss;
        ss << pMgr->getNormalCostSilver();
        m_pLabelNormalCostSilver->setString(ss.str().c_str());
    }
    if (m_pLabelSpecialCostGold) {
        std::stringstream ss;
        ss << pMgr->getSpecialCostGold();
        m_pLabelSpecialCostGold->setString(ss.str().c_str());
    }

    if (m_pLabelCostDescNormal) {
        std::stringstream ss;
        ss << "Còn lại: " << pMgr->getNormalTimesLeft();
        m_pLabelCostDescNormal->setString(ss.str().c_str());
    }
    if (m_pLabelCostDescSpecial) {
        std::stringstream ss;
        ss << "Còn lại: " << pMgr->getSpecialTimesLeft();
        m_pLabelCostDescSpecial->setString(ss.str().c_str());
    }

    if (m_pLabelCanGetSoulNormal) {
        std::stringstream ss;
        ss << "+" << pMgr->getBaseSoulGainNormal();
        m_pLabelCanGetSoulNormal->setString(ss.str().c_str());
    }
    if (m_pLabelCanGetSoulSpecial) {
        std::stringstream ss;
        ss << "+" << pMgr->getBaseSoulGainSpecial();
        m_pLabelCanGetSoulSpecial->setString(ss.str().c_str());
    }

    // Trạng thái: Trước khi luyện hay Đang có Soul chờ nhân bội số
    bool inTraining = pMgr->isTraining() && pMgr->getPendingSoul() > 0;
    if (m_pNodePreTrain) m_pNodePreTrain->setVisible(!inTraining);
    if (m_pNodeAfterTrain) m_pNodeAfterTrain->setVisible(inTraining);

    if (inTraining) {
        int pending = pMgr->getPendingSoul();
        int multi = pMgr->getCurrentMultiplier();
        int total = pending * multi;

        if (m_pLabelTrainSoul) {
            std::stringstream ss;
            ss << pending;
            m_pLabelTrainSoul->setString(ss.str().c_str());
        }
        if (m_pLabelMultiple) {
            std::stringstream ss;
            ss << "x" << multi;
            m_pLabelMultiple->setString(ss.str().c_str());
        }
        if (m_pLabelTrainSoulAll) {
            std::stringstream ss;
            ss << total;
            m_pLabelTrainSoulAll->setString(ss.str().c_str());
        }
        if (m_pLabelFreeMutiTimes) {
            std::stringstream ss;
            ss << "Miễn phí: " << pMgr->getFreeSpecialMultiTimes();
            m_pLabelFreeMutiTimes->setString(ss.str().c_str());
        }
    }
}

void CLimitTrainSoulView::onTrainSoulUpdated(CCObject* pObj) {
    updateUI();
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CLimitTrainSoulView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CLimitTrainSoulView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnBack") == 0 || strcmp(pSelectorName, "btn_back") == 0) {
        return cccontrol_selector(CLimitTrainSoulView::onBtnBack);
    }
    if (strcmp(pSelectorName, "onBtnTrainNormal") == 0 || strcmp(pSelectorName, "btn_train_normal") == 0) {
        return cccontrol_selector(CLimitTrainSoulView::onBtnTrainNormal);
    }
    if (strcmp(pSelectorName, "onBtnTrainSpecial") == 0 || strcmp(pSelectorName, "btn_train_special") == 0) {
        return cccontrol_selector(CLimitTrainSoulView::onBtnTrainSpecial);
    }
    if (strcmp(pSelectorName, "onBtnSilverMuti") == 0 || strcmp(pSelectorName, "btn_silver_muti") == 0) {
        return cccontrol_selector(CLimitTrainSoulView::onBtnSilverMuti);
    }
    if (strcmp(pSelectorName, "onBtnGoldMuti") == 0 || strcmp(pSelectorName, "btn_gold_muti") == 0) {
        return cccontrol_selector(CLimitTrainSoulView::onBtnGoldMuti);
    }
    if (strcmp(pSelectorName, "onBtnGet") == 0 || strcmp(pSelectorName, "btn_get") == 0) {
        return cccontrol_selector(CLimitTrainSoulView::onBtnGet);
    }
    return NULL;
}

bool CLimitTrainSoulView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cur_soul", CCLabelTTF*, this->m_pLabelCurSoul);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_train_soul", CCLabelTTF*, this->m_pLabelTrainSoul);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_train_soul_all", CCLabelTTF*, this->m_pLabelTrainSoulAll);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_multiple", CCLabelTTF*, this->m_pLabelMultiple);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silver_normal", CCLabelBMFont*, this->m_pLabelNormalCostSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gold_special", CCLabelBMFont*, this->m_pLabelSpecialCostGold);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost_silver", CCLabelTTF*, this->m_pLabelCostSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost_gold", CCLabelTTF*, this->m_pLabelCostGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost_desc_normal", CCLabelTTF*, this->m_pLabelCostDescNormal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost_desc_special", CCLabelTTF*, this->m_pLabelCostDescSpecial);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_free_muti_times", CCLabelTTF*, this->m_pLabelFreeMutiTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_can_get_soul_normal", CCLabelTTF*, this->m_pLabelCanGetSoulNormal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_can_get_soul_special", CCLabelTTF*, this->m_pLabelCanGetSoulSpecial);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gold", CCLabelBMFont*, this->m_pLabelGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silver", CCLabelBMFont*, this->m_pLabelSilver);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_back", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_get", CCControlButton*, this->m_pBtnGet);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_gold_muti", CCControlButton*, this->m_pBtnGoldMuti);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_silver_muti", CCControlButton*, this->m_pBtnSilverMuti);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_train_normal", CCControlButton*, this->m_pBtnTrainNormal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_train_special", CCControlButton*, this->m_pBtnTrainSpecial);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_pre_train", CCNode*, this->m_pNodePreTrain);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_after_train", CCNode*, this->m_pNodeAfterTrain);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_gold_icon", CCSprite*, this->m_pSprGoldIcon);

    return false;
}

void CLimitTrainSoulView::onBtnBack(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CLimitTrainSoulView::onBtnTrainNormal(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CLimitTrainSoulView] Luyen Thuong (Bac)");
    CEightGateMgr::sharedManager()->requestDoTrain(false);
}

void CLimitTrainSoulView::onBtnTrainSpecial(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CLimitTrainSoulView] Luyen Cao Cap (Vang)");
    CEightGateMgr::sharedManager()->requestDoTrain(true);
}

void CLimitTrainSoulView::onBtnSilverMuti(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CLimitTrainSoulView] Boi so Bac");
    CEightGateMgr::sharedManager()->requestMultiplySoul(false);
}

void CLimitTrainSoulView::onBtnGoldMuti(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CLimitTrainSoulView] Boi so Vang");
    CEightGateMgr::sharedManager()->requestMultiplySoul(true);
}

void CLimitTrainSoulView::onBtnGet(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CLimitTrainSoulView] Thu thap Linh Hon");
    CEightGateMgr::sharedManager()->requestCollectSoul();
}
