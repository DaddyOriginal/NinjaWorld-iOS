#include "CRouletteView.h"
#include "CRouletteTurnDialogView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CRouletteView::CRouletteView()
    : m_pLabelGoldPool(NULL)
    , m_pLabelScore(NULL)
    , m_pLabelFree(NULL)
    , m_pLabelGoldOnce(NULL)
    , m_pLabelGold(NULL)
    , m_pLabelSilver(NULL)
    , m_pLabelEndDesc(NULL)
    , m_pBtnRollOnce(NULL)
    , m_pBtnRollTentimes(NULL)
    , m_pBtnRank(NULL)
    , m_pBtnBack(NULL)
    , m_isSpinning(false)
    , m_targetSlot(1)
    , m_currentHLSlot(0)
    , m_spinStepsLeft(0)
    , m_isTenSpinPending(false)
{
    for (int i = 0; i < 12; ++i) {
        m_pBtnCard[i] = NULL;
        m_pSprIcon[i] = NULL;
        m_pSprHL[i] = NULL;
        m_pLabelNum[i] = NULL;
    }
}

CRouletteView::~CRouletteView() {
    CC_SAFE_RELEASE_NULL(m_pLabelGoldPool);
    CC_SAFE_RELEASE_NULL(m_pLabelScore);
    CC_SAFE_RELEASE_NULL(m_pLabelFree);
    CC_SAFE_RELEASE_NULL(m_pLabelGoldOnce);
    CC_SAFE_RELEASE_NULL(m_pLabelGold);
    CC_SAFE_RELEASE_NULL(m_pLabelSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelEndDesc);
    CC_SAFE_RELEASE_NULL(m_pBtnRollOnce);
    CC_SAFE_RELEASE_NULL(m_pBtnRollTentimes);
    CC_SAFE_RELEASE_NULL(m_pBtnRank);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);

    for (int i = 0; i < 12; ++i) {
        CC_SAFE_RELEASE_NULL(m_pBtnCard[i]);
        CC_SAFE_RELEASE_NULL(m_pSprIcon[i]);
        CC_SAFE_RELEASE_NULL(m_pSprHL[i]);
        CC_SAFE_RELEASE_NULL(m_pLabelNum[i]);
    }

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationWheelInfoUpdated);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationWheelSpinResult);
}

CRouletteView* CRouletteView::create() {
    CRouletteView* pView = new CRouletteView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CRouletteView::init() {
    if (!CCLayer::init()) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("RouletteView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/RouletteView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    // Tắt tất cả viền đèn phát sáng ban đầu
    for (int i = 0; i < 12; ++i) {
        if (m_pSprHL[i]) {
            m_pSprHL[i]->setVisible(false);
        }
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CRouletteView::onWheelInfoUpdated), kNotificationWheelInfoUpdated, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CRouletteView::onWheelSpinResult), kNotificationWheelSpinResult, NULL);

    return true;
}

void CRouletteView::onEnter() {
    CCLayer::onEnter();
    CRouletteMgr::sharedManager()->requestWheelInfo();
}

void CRouletteView::onExit() {
    this->unschedule(schedule_selector(CRouletteView::updateSpinStep));
    CCLayer::onExit();
}

void CRouletteView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CRouletteView::updateUI() {
    CRouletteMgr* pMgr = CRouletteMgr::sharedManager();
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();

    if (m_pLabelGoldPool) {
        std::stringstream ss;
        ss << pMgr->getJackpot();
        m_pLabelGoldPool->setString(ss.str().c_str());
    }

    if (m_pLabelScore) {
        std::stringstream ss;
        ss << pMgr->getScore();
        m_pLabelScore->setString(ss.str().c_str());
    }

    if (m_pLabelFree) {
        std::stringstream ss;
        ss << "Miễn phí: " << pMgr->getFreeSpins();
        m_pLabelFree->setString(ss.str().c_str());
    }

    if (m_pLabelGoldOnce) {
        std::stringstream ss;
        ss << pMgr->getCostOnce();
        m_pLabelGoldOnce->setString(ss.str().c_str());
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
}

void CRouletteView::highlightSlot(int slotIndex) {
    for (int i = 0; i < 12; ++i) {
        if (m_pSprHL[i]) {
            m_pSprHL[i]->setVisible(i == slotIndex);
        }
    }
}

void CRouletteView::startSpinAnimation(int targetSlot, bool isTen) {
    m_isSpinning = true;
    m_targetSlot = (targetSlot >= 1 && targetSlot <= 12) ? targetSlot : 1;
    m_isTenSpinPending = isTen;

    // Quay ít nhất 2 vòng đầy đủ (24 bước) rồi dừng tại targetSlot (1-based -> 0-based)
    int targetIdx = m_targetSlot - 1;
    int distance = (targetIdx - m_currentHLSlot + 12) % 12;
    m_spinStepsLeft = 24 + distance;

    this->schedule(schedule_selector(CRouletteView::updateSpinStep), 0.06f);
}

void CRouletteView::updateSpinStep(float dt) {
    m_currentHLSlot = (m_currentHLSlot + 1) % 12;
    highlightSlot(m_currentHLSlot);
    m_spinStepsLeft--;

    if (m_spinStepsLeft <= 0) {
        this->unschedule(schedule_selector(CRouletteView::updateSpinStep));
        m_isSpinning = false;
        highlightSlot(m_targetSlot - 1);

        CCLog("[CRouletteView] Dừng quay tại Ô %d!", m_targetSlot);

        if (m_isTenSpinPending) {
            const std::vector<RouletteSpinItem>& results = CRouletteMgr::sharedManager()->getLastSpinResults();
            CRouletteTurnDialogView* pDialog = CRouletteTurnDialogView::createWithItems(results);
            if (pDialog) {
                pDialog->Show(this->getParent() ? this->getParent() : this, 100);
            }
        }
        updateUI();
    }
}

void CRouletteView::onWheelInfoUpdated(CCObject* pObj) {
    updateUI();
}

void CRouletteView::onWheelSpinResult(CCObject* pObj) {
    const std::vector<RouletteSpinItem>& results = CRouletteMgr::sharedManager()->getLastSpinResults();
    int target = results.empty() ? 1 : results[0].slotId;
    bool isTen = results.size() > 1;
    startSpinAnimation(target, isTen);
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CRouletteView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CRouletteView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "clickRollOnce") == 0 || strcmp(pSelectorName, "btn_roll_once") == 0) {
        return cccontrol_selector(CRouletteView::onClickRollOnce);
    }
    if (strcmp(pSelectorName, "clickRollTentimes") == 0 || strcmp(pSelectorName, "btn_roll_tentimes") == 0) {
        return cccontrol_selector(CRouletteView::onClickRollTentimes);
    }
    if (strcmp(pSelectorName, "clickRank") == 0 || strcmp(pSelectorName, "btn_rank") == 0) {
        return cccontrol_selector(CRouletteView::onClickRank);
    }
    if (strcmp(pSelectorName, "clickBack") == 0 || strcmp(pSelectorName, "btn_back") == 0) {
        return cccontrol_selector(CRouletteView::onClickBack);
    }
    return NULL;
}

bool CRouletteView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gold_pool", CCLabelTTF*, this->m_pLabelGoldPool);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_score", CCLabelTTF*, this->m_pLabelScore);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_free", CCLabelTTF*, this->m_pLabelFree);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gold_once", CCLabelTTF*, this->m_pLabelGoldOnce);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gold", CCLabelBMFont*, this->m_pLabelGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silver", CCLabelBMFont*, this->m_pLabelSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_end_desc", CCLabelTTF*, this->m_pLabelEndDesc);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_roll_once", CCControlButton*, this->m_pBtnRollOnce);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_roll_tentimes", CCControlButton*, this->m_pBtnRollTentimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_rank", CCControlButton*, this->m_pBtnRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_back", CCControlButton*, this->m_pBtnBack);

    for (int i = 0; i < 12; ++i) {
        char buf[64];
        snprintf(buf, sizeof(buf), "btn_card%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCControlButton*, this->m_pBtnCard[i]);

        snprintf(buf, sizeof(buf), "sprite_icon_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCSprite*, this->m_pSprIcon[i]);

        snprintf(buf, sizeof(buf), "sprite_hl_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCSprite*, this->m_pSprHL[i]);

        snprintf(buf, sizeof(buf), "label_num_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCLabelTTF*, this->m_pLabelNum[i]);
    }

    return false;
}

void CRouletteView::onClickRollOnce(CCObject* pSender, CCControlEvent pEvent) {
    if (m_isSpinning) return;
    CCLog("[CRouletteView] Bam quay 1 Lan");
    CRouletteMgr::sharedManager()->requestSpin(false);
}

void CRouletteView::onClickRollTentimes(CCObject* pSender, CCControlEvent pEvent) {
    if (m_isSpinning) return;
    CCLog("[CRouletteView] Bam quay 10 Lan");
    CRouletteMgr::sharedManager()->requestSpin(true);
}

void CRouletteView::onClickRank(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRouletteView] Xem Bang Xep Hang Vong Quay");
}

void CRouletteView::onClickBack(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}
