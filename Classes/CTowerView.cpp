#include "CTowerView.h"
#include "CTowerLevelView.h"
#include "CMainMenu.h"
#include <sstream>

CTowerView::CTowerView()
    : m_pBtnReset(NULL)
    , m_pBtnRank(NULL)
    , m_pBtnSweep(NULL)
    , m_pBtnBack(NULL)
    , m_pLabelLeftTimes(NULL)
    , m_pScrollTower(NULL)
    , m_pNodeFight(NULL)
    , m_pChapterListContainer(NULL)
{
}

CTowerView::~CTowerView() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationTowerInfoUpdated");
    CC_SAFE_RELEASE_NULL(m_pBtnReset);
    CC_SAFE_RELEASE_NULL(m_pBtnRank);
    CC_SAFE_RELEASE_NULL(m_pBtnSweep);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pLabelLeftTimes);
    CC_SAFE_RELEASE_NULL(m_pScrollTower);
    CC_SAFE_RELEASE_NULL(m_pNodeFight);
}

CTowerView* CTowerView::create() {
    CTowerView* pRet = new CTowerView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CTowerView::init() {
    if (!CCLayer::init()) return false;

    // Nạp giao diện TowerView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("TowerView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CTowerView] Canh bao: Khong the tai TowerView.ccbi");
    }

    m_pChapterListContainer = CCNode::create();
    if (m_pScrollTower) {
        m_pScrollTower->setContainer(m_pChapterListContainer);
    } else {
        this->addChild(m_pChapterListContainer);
        m_pChapterListContainer->setPosition(ccp(80.0f, 150.0f));
    }

    buildTowerChapterList();
    updateTowerHUD();

    return true;
}

void CTowerView::onEnter() {
    CCLayer::onEnter();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(
        this,
        callfuncO_selector(CTowerView::refreshView),
        "kNotificationTowerInfoUpdated",
        NULL
    );
    CTowerMgr::sharedManager()->requestTowerInfo();
}

void CTowerView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationTowerInfoUpdated");
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO TowerView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CTowerView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CTowerView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnReset", CTowerView::onBtnResetClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnRank", CTowerView::onBtnRankClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnSweep", CTowerView::onBtnSweepClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnBack", CTowerView::onBtnBackClicked);
    return NULL;
}

bool CTowerView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReset", CCControlButton*, this->m_pBtnReset);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRank", CCControlButton*, this->m_pBtnRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnSweep", CCControlButton*, this->m_pBtnSweep);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBack", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_leftTimes", CCLabelTTF*, this->m_pLabelLeftTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scroll_tower", CCScrollView*, this->m_pScrollTower);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_fight", CCNode*, this->m_pNodeFight);
    return false;
}

void CTowerView::buildTowerChapterList() {
    if (!m_pChapterListContainer) return;
    m_pChapterListContainer->removeAllChildrenWithCleanup(true);

    int maxChap = CTowerMgr::sharedManager()->getMaxChap();
    int curChap = CTowerMgr::sharedManager()->getCurrentChapter();

    CCMenu* pMenu = CCMenu::create();
    pMenu->setPosition(CCPointZero);

    // Bố trí 25 tầng tháp lớn theo dạng cuộn hoặc lưới
    for (int chapId = 1; chapId <= 25; ++chapId) {
        const TowerChapterEntry* pEntry = CTowerTableMgr::sharedManager()->getChapter(chapId);
        std::string chapName = pEntry ? pEntry->name : "Thí Luyện";

        bool isUnlocked = (chapId <= maxChap);
        bool isCurrent = (chapId == curChap);

        std::stringstream ssTitle;
        ssTitle << "Tháp " << chapId << ": " << chapName;
        if (!isUnlocked) {
            ssTitle << " (Khóa)";
        } else if (isCurrent) {
            ssTitle << " [Hiện tại]";
        }

        CCMenuItemFont* pItem = CCMenuItemFont::create(ssTitle.str().c_str(), this, menu_selector(CTowerView::onChapterClicked));
        pItem->setFontName("Helvetica-Bold");
        pItem->setFontSize(16);
        pItem->setTag(chapId);

        float posY = 600.0f - (chapId - 1) * 45.0f;
        pItem->setPosition(ccp(180.0f, posY));

        if (isCurrent) {
            pItem->setColor(ccc3(255, 215, 0)); // Vàng sáng
        } else if (isUnlocked) {
            pItem->setColor(ccc3(230, 230, 230)); // Trắng
        } else {
            pItem->setColor(ccc3(120, 120, 120)); // Xám khóa
        }

        pMenu->addChild(pItem);
    }

    m_pChapterListContainer->addChild(pMenu);

    if (m_pScrollTower) {
        m_pScrollTower->setContentSize(CCSizeMake(400.0f, 25 * 45.0f + 100.0f));
    }
}

void CTowerView::onChapterClicked(CCObject* pSender) {
    CCMenuItem* pItem = dynamic_cast<CCMenuItem*>(pSender);
    if (!pItem) return;

    int chapId = pItem->getTag();
    if (!CTowerMgr::sharedManager()->isChapterUnlocked(chapId)) {
        CCLog("[CTowerView] Thap tang %d chua duoc mo!", chapId);
        return;
    }

    CCLog("[CTowerView] Vao thap tang %d", chapId);
    CTowerMgr::sharedManager()->setSelectedChapter(chapId);

    // Mở giao diện 7 ải CTowerLevelView
    CTowerLevelView* pLevelView = CTowerLevelView::createWithChapter(chapId);
    if (pLevelView) {
        this->addChild(pLevelView, 50);
    }
}

void CTowerView::updateTowerHUD() {
    if (m_pLabelLeftTimes) {
        std::stringstream ss;
        ss << "Lượt làm mới: " << CTowerMgr::sharedManager()->getRemainReset() << "/3";
        m_pLabelLeftTimes->setString(ss.str().c_str());
    }
}

void CTowerView::onBtnResetClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CTowerView] Nguoi choi bam Lam Moi Thap");
    CTowerMgr::sharedManager()->requestResetTower();
}

void CTowerView::onBtnRankClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CTowerView] Nguoi choi bam Bang Xep Hang Thap");
}

void CTowerView::onBtnSweepClicked(CCObject* pSender, CCControlEvent pEvent) {
    int maxChap = CTowerMgr::sharedManager()->getMaxChap();
    CCLog("[CTowerView] Nguoi choi bam Can Quet Thap den tang %d", maxChap);
    CTowerMgr::sharedManager()->requestSweepTower(maxChap);
}

void CTowerView::onBtnBackClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CTowerView] Quay lai sảnh chính Lang La");
    if (CMainMenu::sharedManager()) {
        CMainMenu::sharedManager()->changeToSub(SUBMENU_HOME);
    }
}

void CTowerView::refreshView() {
    buildTowerChapterList();
    updateTowerHUD();
}
