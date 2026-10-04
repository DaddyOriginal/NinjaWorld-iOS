#include "CTowerBossView.h"
#include <sstream>

CTowerBossView::CTowerBossView()
    : m_chapterId(1)
    , m_roundId(1)
    , m_floorSeq(1)
    , m_pLabelBossName(NULL)
    , m_pLabelTalkWords(NULL)
    , m_pLabelExp(NULL)
    , m_pLabelSilver(NULL)
    , m_pBtnFightBoss(NULL)
    , m_pBtnGiveup(NULL)
    , m_pNodeIcon(NULL)
{
}

CTowerBossView::~CTowerBossView() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationTowerBattleCompleted");
    CC_SAFE_RELEASE_NULL(m_pLabelBossName);
    CC_SAFE_RELEASE_NULL(m_pLabelTalkWords);
    CC_SAFE_RELEASE_NULL(m_pLabelExp);
    CC_SAFE_RELEASE_NULL(m_pLabelSilver);
    CC_SAFE_RELEASE_NULL(m_pBtnFightBoss);
    CC_SAFE_RELEASE_NULL(m_pBtnGiveup);
    CC_SAFE_RELEASE_NULL(m_pNodeIcon);
}

CTowerBossView* CTowerBossView::createWithFloor(int chapterId, int roundId) {
    CTowerBossView* pRet = new CTowerBossView();
    if (pRet && pRet->initWithFloor(chapterId, roundId)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CTowerBossView::initWithFloor(int chapterId, int roundId) {
    if (!CCLayer::init()) return false;

    m_chapterId = chapterId;
    m_roundId = roundId;
    m_floorSeq = (chapterId - 1) * 7 + roundId;

    // Nạp giao diện TowerBossView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("TowerBossView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CTowerBossView] Canh bao: Khong the tai TowerBossView.ccbi");
    }

    updateBossUI();
    return true;
}

void CTowerBossView::onEnter() {
    CCLayer::onEnter();
}

void CTowerBossView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationTowerBattleCompleted");
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO TowerBossView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CTowerBossView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CTowerBossView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnFightBoss", CTowerBossView::onFightClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnGiveup", CTowerBossView::onGiveupClicked);
    return NULL;
}

bool CTowerBossView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_bossname", CCLabelTTF*, this->m_pLabelBossName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_talkwords", CCLabelTTF*, this->m_pLabelTalkWords);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_exp", CCLabelTTF*, this->m_pLabelExp);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silver", CCLabelTTF*, this->m_pLabelSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnFightBoss", CCControlButton*, this->m_pBtnFightBoss);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnGiveup", CCControlButton*, this->m_pBtnGiveup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon", CCNode*, this->m_pNodeIcon);
    return false;
}

void CTowerBossView::updateBossUI() {
    const TowerFloorEntry* pFloor = CTowerTableMgr::sharedManager()->getFloor(m_floorSeq);
    const TowerChapterEntry* pChap = CTowerTableMgr::sharedManager()->getChapter(m_chapterId);

    if (m_pLabelBossName) {
        std::stringstream ss;
        ss << "Tầng " << m_floorSeq << ": " << (pChap ? pChap->name : "Thủ Vệ Tháp");
        m_pLabelBossName->setString(ss.str().c_str());
    }

    if (m_pLabelTalkWords && pFloor) {
        m_pLabelTalkWords->setString(!pFloor->desc.empty() ? pFloor->desc.c_str() : "Kẻ nào dám làm phiền giấc ngủ của ta!");
    }

    if (m_pLabelExp && pFloor) {
        std::stringstream ss;
        ss << "+" << pFloor->expGain << " EXP";
        m_pLabelExp->setString(ss.str().c_str());
    }

    if (m_pLabelSilver && pFloor) {
        std::stringstream ss;
        ss << "+" << pFloor->silverGain << " Bạc";
        m_pLabelSilver->setString(ss.str().c_str());
    }
}

void CTowerBossView::onFightClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CTowerBossView] Khiêu chiến Boss tầng %d (Chap=%d, Round=%d)", m_floorSeq, m_chapterId, m_roundId);

    CCNotificationCenter::sharedNotificationCenter()->addObserver(
        this,
        callfuncO_selector(CTowerBossView::onBattleNotificationReceived),
        "kNotificationTowerBattleCompleted",
        NULL
    );

    CTowerMgr::sharedManager()->requestFightFloor(m_chapterId, m_roundId);
}

void CTowerBossView::onBattleNotificationReceived(CCObject* pObj) {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationTowerBattleCompleted");

    TowerFightResult res = CTowerMgr::sharedManager()->getLastFightResult();

    std::stringstream ss;
    if (res.isWin) {
        ss << "VƯỢT THÁP THÀNH CÔNG!\n+" << res.expGained << " EXP, +" << res.silverGained << " Bạc";
    } else {
        ss << "KHIÊU CHIẾN THẤT BẠI!\nHãy nâng cấp trang bị và thử lại.";
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));

    CCLabelTTF* pLbl = CCLabelTTF::create(ss.str().c_str(), "Helvetica-Bold", 20.0f, CCSizeMake(360.0f, 120.0f), kCCTextAlignmentCenter);
    pLbl->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f + 20.0f));
    pLbl->setColor(res.isWin ? ccc3(255, 215, 0) : ccc3(239, 68, 68));
    pMask->addChild(pLbl);

    CCMenuItemFont* pClose = CCMenuItemFont::create("Xác Nhận", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(20);
    pClose->setColor(ccc3(255, 235, 120));
    pClose->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f - 70.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);

    // Đóng giao diện Boss view
    this->removeFromParentAndCleanup(true);
}

void CTowerBossView::onGiveupClicked(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}
