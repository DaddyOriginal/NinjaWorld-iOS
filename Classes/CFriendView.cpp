#include "CFriendView.h"
#include "CCBManager.h"
#include <sstream>

CFriendView::CFriendView()
    : m_pNodeTableContent(NULL)
    , m_pNodeCardContent(NULL)
    , m_pBtnMakeFriend(NULL)
    , m_pBtnMoreFriends(NULL)
    , m_pLabelTitle(NULL)
    , m_pScrollFriends(NULL)
    , m_selectedFriendUid(0)
{
}

CFriendView::~CFriendView() {
    CC_SAFE_RELEASE_NULL(m_pNodeTableContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCardContent);
    CC_SAFE_RELEASE_NULL(m_pBtnMakeFriend);
    CC_SAFE_RELEASE_NULL(m_pBtnMoreFriends);
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationFriendListUpdated);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationFriendSearchUpdated);
}

CFriendView* CFriendView::create() {
    CFriendView* pView = new CFriendView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CFriendView::init() {
    if (!CCLayer::init()) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("FriendListView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/FriendListView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CFriendView::onFriendListUpdated), kNotificationFriendListUpdated, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CFriendView::onFriendSearchUpdated), kNotificationFriendSearchUpdated, NULL);

    return true;
}

void CFriendView::onEnter() {
    CCLayer::onEnter();
    CFriendMgr::sharedManager()->requestFriendList(1);
}

void CFriendView::onExit() {
    CCLayer::onExit();
}

void CFriendView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CFriendView::buildFriendList() {
    if (!m_pNodeTableContent) return;

    m_pNodeTableContent->removeAllChildrenWithCleanup(true);

    const std::vector<FriendInfo>& friends = CFriendMgr::sharedManager()->getFriendList();
    if (friends.empty()) return;

    CCSize containerSize = m_pNodeTableContent->getContentSize();
    float cellHeight = 90.0f;
    float totalHeight = cellHeight * friends.size();
    if (totalHeight < containerSize.height) totalHeight = containerSize.height;

    CCNode* pScrollContainer = CCNode::create();
    pScrollContainer->setContentSize(CCSizeMake(containerSize.width, totalHeight));

    for (size_t i = 0; i < friends.size(); ++i) {
        const FriendInfo& f = friends[i];
        float yPos = totalHeight - (i + 1) * cellHeight;

        std::stringstream ss;
        ss << f.name << "  (Lv." << f.level << " - LC: " << f.combatPower << ")";
        CCLabelTTF* pLbl = CCLabelTTF::create(ss.str().c_str(), "Helvetica", 20.0f);
        pLbl->setAnchorPoint(ccp(0.0f, 0.5f));
        pLbl->setPosition(ccp(20.0f, yPos + cellHeight * 0.5f));
        pScrollContainer->addChild(pLbl);
    }

    m_pScrollFriends = CCScrollView::create(containerSize, pScrollContainer);
    m_pScrollFriends->setDirection(kCCScrollViewDirectionVertical);
    m_pScrollFriends->setContentOffset(ccp(0, containerSize.height - totalHeight));
    m_pNodeTableContent->addChild(m_pScrollFriends);
}

void CFriendView::onFriendListUpdated(CCObject* pObj) {
    buildFriendList();
}

void CFriendView::onFriendSearchUpdated(CCObject* pObj) {
    buildFriendList();
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CFriendView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CFriendView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "clickMakeFriend") == 0 || strcmp(pSelectorName, "btn_make_friend") == 0) {
        return cccontrol_selector(CFriendView::onClickMakeFriend);
    }
    if (strcmp(pSelectorName, "clickMoreFriends") == 0 || strcmp(pSelectorName, "btn_more_friends") == 0) {
        return cccontrol_selector(CFriendView::onClickMoreFriends);
    }
    return NULL;
}

bool CFriendView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_tablecontent", CCNode*, this->m_pNodeTableContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cardcontent", CCNode*, this->m_pNodeCardContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_make_friend", CCControlButton*, this->m_pBtnMakeFriend);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_more_friends", CCControlButton*, this->m_pBtnMoreFriends);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_FriendListView0", CCLabelTTF*, this->m_pLabelTitle);
    return false;
}

void CFriendView::onClickMakeFriend(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CFriendView] Ket ban moi");
}

void CFriendView::onClickMoreFriends(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CFriendView] Tim them ban be");
}

void CFriendView::onBtnBack(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}
