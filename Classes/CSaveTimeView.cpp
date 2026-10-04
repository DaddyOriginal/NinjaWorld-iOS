#include "CSaveTimeView.h"
#include "CCBManager.h"
#include <sstream>

CSaveTimeView::CSaveTimeView()
    : m_pLabelRamenNum(NULL)
    , m_pLabelExp(NULL)
    , m_pLabelSilver(NULL)
    , m_pLabelExchangeNum(NULL)
    , m_pLabelCurChapter(NULL)
    , m_pLabelCost(NULL)
    , m_pLabelTip(NULL)
    , m_pLabelAddDesc(NULL)
    , m_pLabelMaxVip(NULL)
    , m_pBtnClose(NULL)
    , m_pBtnBuy(NULL)
    , m_pBtnExchange(NULL)
{
}

CSaveTimeView::~CSaveTimeView() {
    CC_SAFE_RELEASE_NULL(m_pLabelRamenNum);
    CC_SAFE_RELEASE_NULL(m_pLabelExp);
    CC_SAFE_RELEASE_NULL(m_pLabelSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelExchangeNum);
    CC_SAFE_RELEASE_NULL(m_pLabelCurChapter);
    CC_SAFE_RELEASE_NULL(m_pLabelCost);
    CC_SAFE_RELEASE_NULL(m_pLabelTip);
    CC_SAFE_RELEASE_NULL(m_pLabelAddDesc);
    CC_SAFE_RELEASE_NULL(m_pLabelMaxVip);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnBuy);
    CC_SAFE_RELEASE_NULL(m_pBtnExchange);

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationSaveTimeUpdated);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationSaveTimeExchanged);
}

CSaveTimeView* CSaveTimeView::create() {
    CSaveTimeView* pView = new CSaveTimeView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CSaveTimeView::init() {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("SaveTimeView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/SaveTimeView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CSaveTimeView::onSaveTimeUpdated), kNotificationSaveTimeUpdated, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CSaveTimeView::onSaveTimeUpdated), kNotificationSaveTimeExchanged, NULL);

    return true;
}

void CSaveTimeView::onEnter() {
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

    CSaveTimeMgr::sharedManager()->requestInfo();
}

void CSaveTimeView::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool CSaveTimeView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void CSaveTimeView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CSaveTimeView::updateUI() {
    CSaveTimeMgr* pMgr = CSaveTimeMgr::sharedManager();

    if (m_pLabelRamenNum) {
        std::stringstream ss;
        ss << pMgr->getRamenCount();
        m_pLabelRamenNum->setString(ss.str().c_str());
    }

    if (m_pLabelExp) {
        std::stringstream ss;
        ss << pMgr->getExpPerRamen();
        m_pLabelExp->setString(ss.str().c_str());
    }

    if (m_pLabelSilver) {
        std::stringstream ss;
        ss << pMgr->getSilverPerRamen();
        m_pLabelSilver->setString(ss.str().c_str());
    }

    if (m_pLabelExchangeNum) {
        std::stringstream ss;
        ss << (pMgr->getTotalTrans() - pMgr->getUsedTrans());
        m_pLabelExchangeNum->setString(ss.str().c_str());
    }

    if (m_pLabelCurChapter) {
        std::stringstream ss;
        ss << "Ải cao nhất: " << pMgr->getChapter() << "-" << pMgr->getRound();
        m_pLabelCurChapter->setString(ss.str().c_str());
    }
}

void CSaveTimeView::onSaveTimeUpdated(CCObject* pObj) {
    updateUI();
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CSaveTimeView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CSaveTimeView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "dialogClose") == 0 || strcmp(pSelectorName, "closeButton") == 0) {
        return cccontrol_selector(CSaveTimeView::onBtnClose);
    }
    if (strcmp(pSelectorName, "btn_exchange") == 0) {
        return cccontrol_selector(CSaveTimeView::onBtnExchange);
    }
    if (strcmp(pSelectorName, "btn_buy") == 0) {
        return cccontrol_selector(CSaveTimeView::onBtnBuy);
    }
    return NULL;
}

bool CSaveTimeView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_ramen_num", CCLabelBMFont*, this->m_pLabelRamenNum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_exp", CCLabelBMFont*, this->m_pLabelExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silver", CCLabelBMFont*, this->m_pLabelSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_exchange_num", CCLabelBMFont*, this->m_pLabelExchangeNum);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cur_chapter", CCLabelTTF*, this->m_pLabelCurChapter);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost", CCLabelTTF*, this->m_pLabelCost);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_tip", CCLabelTTF*, this->m_pLabelTip);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_desc", CCLabelTTF*, this->m_pLabelAddDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_max_vip", CCLabelTTF*, this->m_pLabelMaxVip);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "closeButton", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_buy", CCControlButton*, this->m_pBtnBuy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_exchange", CCControlButton*, this->m_pBtnExchange);

    return false;
}

void CSaveTimeView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CSaveTimeView::onBtnExchange(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CSaveTimeView] Bam Doi Ramen");
    CSaveTimeMgr::sharedManager()->requestExchange();
}

void CSaveTimeView::onBtnBuy(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CSaveTimeView] Bam Mua Ramen");
}
