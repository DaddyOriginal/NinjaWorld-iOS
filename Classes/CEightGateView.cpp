#include "CEightGateView.h"
#include "CLimitTrainSoulView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CEightGateView::CEightGateView()
    : m_pLabelCurSoul(NULL)
    , m_pLabelCostSoul(NULL)
    , m_pLabelCostSilver(NULL)
    , m_pLabelGateLv(NULL)
    , m_pLabelAddAtt(NULL)
    , m_pLabelAddDef(NULL)
    , m_pLabelAddChakra(NULL)
    , m_pLabelAddAttPer(NULL)
    , m_pLabelAddDefPer(NULL)
    , m_pLabelAddChakraPer(NULL)
    , m_pLabelGold(NULL)
    , m_pLabelSilver(NULL)
    , m_pBtnBack(NULL)
    , m_pBtnLimitTrain(NULL)
    , m_pBtnOpen(NULL)
    , m_pSprGate(NULL)
    , m_pSprLine(NULL)
    , m_pNodeAnimContainer(NULL)
    , m_pNodeContent(NULL)
    , m_pNodeCell(NULL)
{
    for (int i = 0; i < 8; ++i) {
        m_pSprGateNum[i] = NULL;
    }
}

CEightGateView::~CEightGateView() {
    CC_SAFE_RELEASE_NULL(m_pLabelCurSoul);
    CC_SAFE_RELEASE_NULL(m_pLabelCostSoul);
    CC_SAFE_RELEASE_NULL(m_pLabelCostSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelGateLv);
    CC_SAFE_RELEASE_NULL(m_pLabelAddAtt);
    CC_SAFE_RELEASE_NULL(m_pLabelAddDef);
    CC_SAFE_RELEASE_NULL(m_pLabelAddChakra);
    CC_SAFE_RELEASE_NULL(m_pLabelAddAttPer);
    CC_SAFE_RELEASE_NULL(m_pLabelAddDefPer);
    CC_SAFE_RELEASE_NULL(m_pLabelAddChakraPer);
    CC_SAFE_RELEASE_NULL(m_pLabelGold);
    CC_SAFE_RELEASE_NULL(m_pLabelSilver);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pBtnLimitTrain);
    CC_SAFE_RELEASE_NULL(m_pBtnOpen);
    CC_SAFE_RELEASE_NULL(m_pSprGate);
    CC_SAFE_RELEASE_NULL(m_pSprLine);
    CC_SAFE_RELEASE_NULL(m_pNodeAnimContainer);
    CC_SAFE_RELEASE_NULL(m_pNodeContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCell);

    for (int i = 0; i < 8; ++i) {
        CC_SAFE_RELEASE_NULL(m_pSprGateNum[i]);
    }

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationEightGateUpdated);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationEightGateOpened);
}

CEightGateView* CEightGateView::create() {
    CEightGateView* pView = new CEightGateView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CEightGateView::init() {
    if (!CCLayer::init()) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("LimitTrainView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/LimitTrainView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CEightGateView::onGateUpdated), kNotificationEightGateUpdated, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CEightGateView::onGateOpened), kNotificationEightGateOpened, NULL);

    return true;
}

void CEightGateView::onEnter() {
    CCLayer::onEnter();
    CEightGateMgr::sharedManager()->requestGateInfo();
}

void CEightGateView::onExit() {
    CCLayer::onExit();
}

void CEightGateView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CEightGateView::updateUI() {
    CEightGateMgr* pMgr = CEightGateMgr::sharedManager();
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();

    if (m_pLabelGateLv) {
        std::stringstream ss;
        ss << "Cấp " << pMgr->getGateLevel();
        m_pLabelGateLv->setString(ss.str().c_str());
    }

    if (m_pLabelCurSoul) {
        std::stringstream ss;
        ss << pMgr->getSoul();
        m_pLabelCurSoul->setString(ss.str().c_str());
    }

    if (m_pLabelCostSoul) {
        std::stringstream ss;
        ss << pMgr->getCostSoul();
        m_pLabelCostSoul->setString(ss.str().c_str());
    }

    if (m_pLabelCostSilver) {
        std::stringstream ss;
        ss << pMgr->getCostSilver();
        m_pLabelCostSilver->setString(ss.str().c_str());
    }

    // Thuộc tính tổng cộng dồn
    if (m_pLabelAddAtt) {
        std::stringstream ss;
        ss << "+" << pMgr->getAddAttack();
        m_pLabelAddAtt->setString(ss.str().c_str());
    }
    if (m_pLabelAddDef) {
        std::stringstream ss;
        ss << "+" << pMgr->getAddDefense();
        m_pLabelAddDef->setString(ss.str().c_str());
    }
    if (m_pLabelAddChakra) {
        std::stringstream ss;
        ss << "+" << pMgr->getAddChakra();
        m_pLabelAddChakra->setString(ss.str().c_str());
    }
    if (m_pLabelAddAttPer) {
        std::stringstream ss;
        ss << "+" << pMgr->getAddAttackPer() << "%";
        m_pLabelAddAttPer->setString(ss.str().c_str());
    }
    if (m_pLabelAddDefPer) {
        std::stringstream ss;
        ss << "+" << pMgr->getAddDefensePer() << "%";
        m_pLabelAddDefPer->setString(ss.str().c_str());
    }
    if (m_pLabelAddChakraPer) {
        std::stringstream ss;
        ss << "+" << pMgr->getAddChakraPer() << "%";
        m_pLabelAddChakraPer->setString(ss.str().c_str());
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

    // Hiển thị trạng thái 8 huyệt Bát Môn
    int curGate = pMgr->getGateId();
    for (int i = 0; i < 8; ++i) {
        if (m_pSprGateNum[i]) {
            if (i + 1 < curGate) {
                // Đã mở
                m_pSprGateNum[i]->setColor(ccc3(255, 255, 255));
                m_pSprGateNum[i]->setOpacity(255);
            } else if (i + 1 == curGate) {
                // Huyệt hiện tại chuẩn bị khai mở
                m_pSprGateNum[i]->setColor(ccc3(255, 240, 100));
                m_pSprGateNum[i]->setOpacity(255);
            } else {
                // Chưa mở
                m_pSprGateNum[i]->setColor(ccc3(120, 120, 120));
                m_pSprGateNum[i]->setOpacity(150);
            }
        }
    }
}

void CEightGateView::onGateUpdated(CCObject* pObj) {
    updateUI();
}

void CEightGateView::onGateOpened(CCObject* pObj) {
    updateUI();
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CEightGateView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CEightGateView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnBack") == 0 || strcmp(pSelectorName, "btn_back") == 0) {
        return cccontrol_selector(CEightGateView::onBtnBack);
    }
    if (strcmp(pSelectorName, "onBtnLimitTrain") == 0 || strcmp(pSelectorName, "btn_limitTrain") == 0) {
        return cccontrol_selector(CEightGateView::onBtnLimitTrain);
    }
    if (strcmp(pSelectorName, "onBtnOpen") == 0 || strcmp(pSelectorName, "btn_open") == 0) {
        return cccontrol_selector(CEightGateView::onBtnOpen);
    }
    return NULL;
}

bool CEightGateView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cur_soul", CCLabelTTF*, this->m_pLabelCurSoul);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost_soul", CCLabelTTF*, this->m_pLabelCostSoul);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cost_silver", CCLabelTTF*, this->m_pLabelCostSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gate_lv", CCLabelTTF*, this->m_pLabelGateLv);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_att", CCLabelTTF*, this->m_pLabelAddAtt);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_def", CCLabelTTF*, this->m_pLabelAddDef);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_chakra", CCLabelTTF*, this->m_pLabelAddChakra);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_att_per", CCLabelTTF*, this->m_pLabelAddAttPer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_def_per", CCLabelTTF*, this->m_pLabelAddDefPer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_add_chakra_per", CCLabelTTF*, this->m_pLabelAddChakraPer);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_gold", CCLabelBMFont*, this->m_pLabelGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silver", CCLabelBMFont*, this->m_pLabelSilver);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_back", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_limitTrain", CCControlButton*, this->m_pBtnLimitTrain);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_open", CCControlButton*, this->m_pBtnOpen);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_gate", CCSprite*, this->m_pSprGate);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_line", CCSprite*, this->m_pSprLine);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_anim_container", CCNode*, this->m_pNodeAnimContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cell", CCNode*, this->m_pNodeCell);

    for (int i = 0; i < 8; ++i) {
        char buf[64];
        snprintf(buf, sizeof(buf), "spr_gate_num_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCSprite*, this->m_pSprGateNum[i]);
    }

    return false;
}

void CEightGateView::onBtnBack(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CEightGateView::onBtnLimitTrain(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CEightGateView] Mo giao dien Luyen Hon (CLimitTrainSoulView)");
    CLimitTrainSoulView* pSoulView = CLimitTrainSoulView::create();
    if (pSoulView) {
        pSoulView->Show(this->getParent() ? this->getParent() : this, 80);
    }
}

void CEightGateView::onBtnOpen(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CEightGateView] Nguoi choi bam Khai Mo Huyet Bat Mon");
    CEightGateMgr::sharedManager()->requestOpenGate();
}
