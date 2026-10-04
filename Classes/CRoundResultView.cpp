#include "CRoundResultView.h"
#include "CPlayerDataMgr.h"
#include "CSubChapterView.h"
#include <sstream>

CRoundResultView::CRoundResultView()
    : m_chapterId(1)
    , m_stageId(1)
    , m_pLabelMyName(NULL)
    , m_pLabelEnemyName(NULL)
    , m_pLabelMyAttack(NULL)
    , m_pLabelEnemyDefense(NULL)
    , m_pLabelExpVal(NULL)
    , m_pLabelSilverVal(NULL)
    , m_pLabelDropDesc(NULL)
    , m_pBtnContinue(NULL)
    , m_pBtnUpgrade(NULL)
    , m_pBtnGetCard(NULL)
    , m_pSpriteResultType(NULL)
    , m_pSpriteDot1(NULL)
    , m_pSpriteDot2(NULL)
    , m_pSpriteDot3(NULL)
{
    memset(&m_result, 0, sizeof(m_result));
}

CRoundResultView::~CRoundResultView() {
    CC_SAFE_RELEASE_NULL(m_pLabelMyName);
    CC_SAFE_RELEASE_NULL(m_pLabelEnemyName);
    CC_SAFE_RELEASE_NULL(m_pLabelMyAttack);
    CC_SAFE_RELEASE_NULL(m_pLabelEnemyDefense);
    CC_SAFE_RELEASE_NULL(m_pLabelExpVal);
    CC_SAFE_RELEASE_NULL(m_pLabelSilverVal);
    CC_SAFE_RELEASE_NULL(m_pLabelDropDesc);
    CC_SAFE_RELEASE_NULL(m_pBtnContinue);
    CC_SAFE_RELEASE_NULL(m_pBtnUpgrade);
    CC_SAFE_RELEASE_NULL(m_pBtnGetCard);
    CC_SAFE_RELEASE_NULL(m_pSpriteResultType);
    CC_SAFE_RELEASE_NULL(m_pSpriteDot1);
    CC_SAFE_RELEASE_NULL(m_pSpriteDot2);
    CC_SAFE_RELEASE_NULL(m_pSpriteDot3);
}

CRoundResultView* CRoundResultView::createWithResult(const PVEFightResult& result, int chapterId, int stageId) {
    CRoundResultView* pRet = new CRoundResultView();
    if (pRet && pRet->initWithResult(result, chapterId, stageId)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CRoundResultView::initWithResult(const PVEFightResult& result, int chapterId, int stageId) {
    if (!CCLayer::init()) return false;

    m_result = result;
    m_chapterId = chapterId;
    m_stageId = stageId;

    // Nạp giao diện RoundResultView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("RoundResultView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CRoundResultView] Canh bao: Khong the tai RoundResultView.ccbi");
    }

    updateResultUI();
    return true;
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO RoundResultView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CRoundResultView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CRoundResultView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnContinue", CRoundResultView::onContinueClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnUpgrade", CRoundResultView::onUpgradeClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "OnGetCard", CRoundResultView::onGetCardClicked);
    return NULL;
}

bool CRoundResultView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_myname", CCLabelTTF*, this->m_pLabelMyName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_enemyname", CCLabelTTF*, this->m_pLabelEnemyName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_myattack", CCLabelTTF*, this->m_pLabelMyAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_enemydefense", CCLabelTTF*, this->m_pLabelEnemyDefense);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_expval", CCLabelTTF*, this->m_pLabelExpVal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silverval", CCLabelTTF*, this->m_pLabelSilverVal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_dropdesc", CCLabelTTF*, this->m_pLabelDropDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_continue", CCControlButton*, this->m_pBtnContinue);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_upgrade", CCControlButton*, this->m_pBtnUpgrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_getcard", CCControlButton*, this->m_pBtnGetCard);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_resulttype", CCSprite*, this->m_pSpriteResultType);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_dot1", CCSprite*, this->m_pSpriteDot1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_dot2", CCSprite*, this->m_pSpriteDot2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_dot3", CCSprite*, this->m_pSpriteDot3);
    return false;
}

void CRoundResultView::updateResultUI() {
    CPlayerDataMgr* pPlayer = CPlayerDataMgr::sharedManager();
    if (m_pLabelMyName && pPlayer) {
        m_pLabelMyName->setString(pPlayer->getNickname().c_str());
    }

    if (m_pLabelEnemyName) {
        m_pLabelEnemyName->setString(!m_result.enemyName.empty() ? m_result.enemyName.c_str() : "Doi Thu Ninja");
    }

    if (m_pLabelExpVal) {
        std::stringstream ss;
        ss << "+" << m_result.expGained << " EXP";
        m_pLabelExpVal->setString(ss.str().c_str());
    }

    if (m_pLabelSilverVal) {
        std::stringstream ss;
        ss << "+" << m_result.silverGained << " Bạc";
        m_pLabelSilverVal->setString(ss.str().c_str());
    }

    if (m_pLabelDropDesc) {
        m_pLabelDropDesc->setString(!m_result.awardDesc.empty() ? m_result.awardDesc.c_str() : "Vượt Ải Thành Công!");
    }

    // Hiển thị 3 sao chiến thắng
    if (m_result.isWin) {
        if (m_pSpriteDot1) m_pSpriteDot1->setVisible(m_result.starRating >= 1);
        if (m_pSpriteDot2) m_pSpriteDot2->setVisible(m_result.starRating >= 2);
        if (m_pSpriteDot3) m_pSpriteDot3->setVisible(m_result.starRating >= 3);
    } else {
        if (m_pSpriteDot1) m_pSpriteDot1->setVisible(false);
        if (m_pSpriteDot2) m_pSpriteDot2->setVisible(false);
        if (m_pSpriteDot3) m_pSpriteDot3->setVisible(false);
    }
}

void CRoundResultView::onContinueClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRoundResultView] Nguoi choi bam Tiep Tuc");
    CCNode* pParent = this->getParent();
    if (pParent) {
        // Quay trở lại CSubChapterView
        CSubChapterView* pSub = CSubChapterView::createWithChapter(m_chapterId);
        if (pSub) {
            pParent->addChild(pSub, this->getZOrder());
        }
        this->removeFromParentAndCleanup(true);
    }
}

void CRoundResultView::onUpgradeClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRoundResultView] Nguoi choi bam Nang Cap Nhan Gia");
    onContinueClicked(pSender, pEvent);
}

void CRoundResultView::onGetCardClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRoundResultView] Nguoi choi bam Nhan Thuong The");
    onContinueClicked(pSender, pEvent);
}
