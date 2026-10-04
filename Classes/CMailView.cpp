#include "CMailView.h"
#include "CCBManager.h"
#include <sstream>

CMailView::CMailView()
    : m_pLabelTitle(NULL)
    , m_pLabelDescription(NULL)
    , m_pBtnClose(NULL)
    , m_pLabelLeft(NULL)
    , m_pLabelRight(NULL)
    , m_pScrollMails(NULL)
    , m_selectedMailId(0)
{
}

CMailView::~CMailView() {
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);
    CC_SAFE_RELEASE_NULL(m_pLabelDescription);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pLabelLeft);
    CC_SAFE_RELEASE_NULL(m_pLabelRight);

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationMailListUpdated);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationMailClaimed);
}

CMailView* CMailView::create() {
    CMailView* pView = new CMailView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CMailView::init() {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("CommonSystemMsgView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/CommonSystemMsgView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    if (m_pLabelTitle) {
        m_pLabelTitle->setString("Hòm Thư Hệ Thống");
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CMailView::onMailListUpdated), kNotificationMailListUpdated, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CMailView::onMailClaimed), kNotificationMailClaimed, NULL);

    return true;
}

void CMailView::onEnter() {
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

    CMailMgr::sharedManager()->requestMailList(3);
}

void CMailView::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool CMailView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void CMailView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CMailView::buildMailList() {
    const std::vector<MailItemData>& mails = CMailMgr::sharedManager()->getMailList();
    if (mails.empty()) {
        if (m_pLabelDescription) {
            m_pLabelDescription->setString("Hòm thư hiện tại không có tin nhắn mới.");
        }
        return;
    }

    std::stringstream ss;
    for (size_t i = 0; i < mails.size(); ++i) {
        const MailItemData& m = mails[i];
        ss << "[" << m.senderName << "]: " << m.content;
        if (m.hasReward && m.status != 2) {
            ss << " (Phần thưởng: " << m.silverReward << " Bạc)";
        }
        ss << "\n--------------------\n";
    }

    if (m_pLabelDescription) {
        m_pLabelDescription->setString(ss.str().c_str());
    }
}

void CMailView::onMailListUpdated(CCObject* pObj) {
    buildMailList();
}

void CMailView::onMailClaimed(CCObject* pObj) {
    buildMailList();
}

void CMailView::onSelectMail(int mailId) {
    m_selectedMailId = mailId;
    CMailMgr::sharedManager()->requestClaimMail(mailId);
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CMailView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CMailView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "dialogClose") == 0 || strcmp(pSelectorName, "closeButton") == 0) {
        return cccontrol_selector(CMailView::onBtnClose);
    }
    return NULL;
}

bool CMailView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelTitle", CCLabelTTF*, this->m_pLabelTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelDescription", CCLabelTTF*, this->m_pLabelDescription);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "closeButton", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelLeft", CCLabelTTF*, this->m_pLabelLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelRight", CCLabelTTF*, this->m_pLabelRight);
    return false;
}

void CMailView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}
