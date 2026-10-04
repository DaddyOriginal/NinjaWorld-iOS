#include "CTowerLevelView.h"
#include "CTowerBossView.h"
#include <sstream>

CTowerLevelView::CTowerLevelView()
    : m_chapterId(1)
    , m_pBtnBack(NULL)
    , m_pLabelLevelInfo(NULL)
{
    for (int i = 0; i < 7; ++i) {
        m_pBtnStages[i] = NULL;
        m_pLabelNames[i] = NULL;
        m_pSpriteBeats[i] = NULL;
        m_pNodeIcons[i] = NULL;
    }
}

CTowerLevelView::~CTowerLevelView() {
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pLabelLevelInfo);
    for (int i = 0; i < 7; ++i) {
        CC_SAFE_RELEASE_NULL(m_pBtnStages[i]);
        CC_SAFE_RELEASE_NULL(m_pLabelNames[i]);
        CC_SAFE_RELEASE_NULL(m_pSpriteBeats[i]);
        CC_SAFE_RELEASE_NULL(m_pNodeIcons[i]);
    }
}

CTowerLevelView* CTowerLevelView::createWithChapter(int chapterId) {
    CTowerLevelView* pRet = new CTowerLevelView();
    if (pRet && pRet->initWithChapter(chapterId)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CTowerLevelView::initWithChapter(int chapterId) {
    if (!CCLayer::init()) return false;

    m_chapterId = chapterId;

    // Nạp giao diện TowerLevelView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("TowerLevelView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CTowerLevelView] Canh bao: Khong the tai TowerLevelView.ccbi");
    }

    updateStagesDisplay();
    return true;
}

void CTowerLevelView::onEnter() {
    CCLayer::onEnter();
    updateStagesDisplay();
}

void CTowerLevelView::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO TowerLevelView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CTowerLevelView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CTowerLevelView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnBack", CTowerLevelView::onBackClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick01", CTowerLevelView::onBtnIconClick01);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick02", CTowerLevelView::onBtnIconClick02);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick03", CTowerLevelView::onBtnIconClick03);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick04", CTowerLevelView::onBtnIconClick04);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick05", CTowerLevelView::onBtnIconClick05);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick06", CTowerLevelView::onBtnIconClick06);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnIconClick07", CTowerLevelView::onBtnIconClick07);
    return NULL;
}

bool CTowerLevelView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBack", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_levelinfo", CCLabelTTF*, this->m_pLabelLevelInfo);

    // 7 ải
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick01", CCControlButton*, this->m_pBtnStages[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick02", CCControlButton*, this->m_pBtnStages[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick03", CCControlButton*, this->m_pBtnStages[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick04", CCControlButton*, this->m_pBtnStages[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick05", CCControlButton*, this->m_pBtnStages[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick06", CCControlButton*, this->m_pBtnStages[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnIconClick07", CCControlButton*, this->m_pBtnStages[6]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name01", CCLabelTTF*, this->m_pLabelNames[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name02", CCLabelTTF*, this->m_pLabelNames[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name03", CCLabelTTF*, this->m_pLabelNames[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name04", CCLabelTTF*, this->m_pLabelNames[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name05", CCLabelTTF*, this->m_pLabelNames[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name06", CCLabelTTF*, this->m_pLabelNames[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name07", CCLabelTTF*, this->m_pLabelNames[6]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat1", CCSprite*, this->m_pSpriteBeats[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat2", CCSprite*, this->m_pSpriteBeats[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat3", CCSprite*, this->m_pSpriteBeats[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat4", CCSprite*, this->m_pSpriteBeats[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat5", CCSprite*, this->m_pSpriteBeats[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat6", CCSprite*, this->m_pSpriteBeats[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_beat7", CCSprite*, this->m_pSpriteBeats[6]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon01", CCNode*, this->m_pNodeIcons[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon02", CCNode*, this->m_pNodeIcons[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon03", CCNode*, this->m_pNodeIcons[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon04", CCNode*, this->m_pNodeIcons[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon05", CCNode*, this->m_pNodeIcons[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon06", CCNode*, this->m_pNodeIcons[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon07", CCNode*, this->m_pNodeIcons[6]);

    return false;
}

void CTowerLevelView::onBackClicked(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CTowerLevelView::onBtnIconClick01(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(1); }
void CTowerLevelView::onBtnIconClick02(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(2); }
void CTowerLevelView::onBtnIconClick03(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(3); }
void CTowerLevelView::onBtnIconClick04(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(4); }
void CTowerLevelView::onBtnIconClick05(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(5); }
void CTowerLevelView::onBtnIconClick06(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(6); }
void CTowerLevelView::onBtnIconClick07(CCObject* pSender, CCControlEvent pEvent) { onStageClicked(7); }

void CTowerLevelView::updateStagesDisplay() {
    const TowerChapterEntry* pChap = CTowerTableMgr::sharedManager()->getChapter(m_chapterId);
    if (m_pLabelLevelInfo) {
        std::stringstream ss;
        ss << "Tháp Tầng " << m_chapterId << ": " << (pChap ? pChap->name : "Thí Luyện");
        m_pLabelLevelInfo->setString(ss.str().c_str());
    }

    int curProc = CTowerMgr::sharedManager()->getCurrentProcess();

    for (int i = 0; i < 7; ++i) {
        int stageIndex = i + 1;
        int floorSeq = (m_chapterId - 1) * 7 + stageIndex;

        // Tên ải
        if (m_pLabelNames[i]) {
            std::stringstream ss;
            ss << "Tầng " << floorSeq;
            if (stageIndex == 7) ss << " [BOSS]";
            m_pLabelNames[i]->setString(ss.str().c_str());
        }

        // Đánh dấu đã vượt qua
        bool isCleared = (stageIndex < curProc);
        if (m_pSpriteBeats[i]) {
            m_pSpriteBeats[i]->setVisible(isCleared);
        }
    }
}

void CTowerLevelView::onStageClicked(int stageIndex) {
    int curProc = CTowerMgr::sharedManager()->getCurrentProcess();

    if (stageIndex < curProc) {
        CCLog("[CTowerLevelView] Ai %d da danh roi, vui long quay lai ngay mai!", stageIndex);
        return;
    }

    if (stageIndex > curProc) {
        CCLog("[CTowerLevelView] Hay vuot qua ai truoc de mo ai %d!", stageIndex);
        return;
    }

    // Mở màn hình khiêu chiến CTowerBossView
    CTowerBossView* pBossView = CTowerBossView::createWithFloor(m_chapterId, stageIndex);
    if (pBossView) {
        this->addChild(pBossView, 100);
    }
}
