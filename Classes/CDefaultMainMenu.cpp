#include "CDefaultMainMenu.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include "CMainMenu.h"
#include "CDailyTaskView.h"
#include "CAwardCenterView.h"
#include "CGrowthFundView.h"
#include "CSaveTimeView.h"
#include <sstream>

CDefaultMainMenu::CDefaultMainMenu()
    : m_pLayerBuildingContent(NULL)
    , m_pNodeCountryBk(NULL)
    , m_pNodeMenuBtns(NULL)
    , m_pLabelMaxAttack(NULL)
    , m_pLabelMaxHonor(NULL)
    , m_pLabelMsgNews(NULL)
{
}

CDefaultMainMenu::~CDefaultMainMenu() {
    CC_SAFE_RELEASE_NULL(m_pLayerBuildingContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCountryBk);
    CC_SAFE_RELEASE_NULL(m_pNodeMenuBtns);
    CC_SAFE_RELEASE_NULL(m_pLabelMaxAttack);
    CC_SAFE_RELEASE_NULL(m_pLabelMaxHonor);
    CC_SAFE_RELEASE_NULL(m_pLabelMsgNews);
}

bool CDefaultMainMenu::init() {
    if (!CCLayer::init()) {
        return false;
    }

    // Nạp giao diện DefaultMainMenu.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("DefaultMainMenu.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/DefaultMainMenu.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCLog("[CDefaultMainMenu] Nap DefaultMainMenu.ccbi thanh cong!");
    } else {
        CCLog("[CDefaultMainMenu] Failed to load DefaultMainMenu.ccbi, dung layer nen!");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg);
    }

    return true;
}

void CDefaultMainMenu::onEnter() {
    CCLayer::onEnter();
    firefly_LoadUserInfo();
}

void CDefaultMainMenu::onExit() {
    CCLayer::onExit();
}

void CDefaultMainMenu::firefly_LoadUserInfo() {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    // Cập nhật Lực chiến
    if (m_pLabelMaxAttack) {
        std::stringstream ss;
        ss << pData->getCombatPower();
        m_pLabelMaxAttack->setString(ss.str().c_str());
    }

    // Cập nhật Danh vọng / Công trạng
    if (m_pLabelMaxHonor) {
        m_pLabelMaxHonor->setString("500");
    }

    // Cập nhật thông báo
    if (m_pLabelMsgNews) {
        m_pLabelMsgNews->setString("!");
    }

    CCLog("[CDefaultMainMenu] Cap nhat thong so Làng Lá: LucChien=%d", pData->getCombatPower());
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CDefaultMainMenu::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CDefaultMainMenu::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnTower") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnTower);
    }
    if (strcmp(pSelectorName, "onBtnActivity") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnActivity);
    }
    if (strcmp(pSelectorName, "onClickNaruto") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onClickNaruto);
    }
    if (strcmp(pSelectorName, "onBtnBuyFund") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnBuyFund);
    }
    if (strcmp(pSelectorName, "onBtnSaveTime") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnSaveTime);
    }
    if (strcmp(pSelectorName, "onClickAwardCenter") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onClickAwardCenter);
    }
    if (strcmp(pSelectorName, "onBtnArena") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnArena);
    }
    if (strcmp(pSelectorName, "onBtnDailyTask") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnDailyTask);
    }
    return NULL;
}

bool CDefaultMainMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_buildingcontent", CCNode*, this->m_pLayerBuildingContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_country_bk", CCNode*, this->m_pNodeCountryBk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_menubtns", CCNode*, this->m_pNodeMenuBtns);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxattack", CCLabelBMFont*, this->m_pLabelMaxAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxhonor", CCLabelBMFont*, this->m_pLabelMaxHonor);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_msg_news", CCLabelBMFont*, this->m_pLabelMsgNews);

    return false;
}

// -------------------------------------------------------------
// SỰ KIỆN CÔNG TRÌNH & NÚT BẤM
// -------------------------------------------------------------
void CDefaultMainMenu::onBtnTower(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo cong trinh: LEO THAP THI LUYEN (SUBMENU_TOWER)");
    if (CMainMenu::sharedManager()) {
        CMainMenu::sharedManager()->changeToSub(SUBMENU_TOWER);
    }
}

void CDefaultMainMenu::onBtnActivity(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo giao dien: HOAT DONG & BAT MON DON GIAP (SUBMENU_ACTIVITY)");
    if (CMainMenu::sharedMainMenu()) {
        CMainMenu::sharedMainMenu()->changeToSub(SUBMENU_ACTIVITY);
    }
}

void CDefaultMainMenu::onClickNaruto(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo giao dien: CHIEU MO NHAN GIA (SUBMENU_SHOP)");
    if (CMainMenu::sharedManager()) {
        CMainMenu::sharedManager()->changeToSub(SUBMENU_SHOP);
    }
}

void CDefaultMainMenu::onBtnBuyFund(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo giao dien: QUY TRUONG THANH");
    CGrowthFundView* pFundView = CGrowthFundView::create();
    if (pFundView) {
        CCNode* pParent = this->getParent() ? this->getParent() : this;
        pFundView->Show(pParent, 60);
    }
}

void CDefaultMainMenu::onBtnSaveTime(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Nhan thuong: TIET KIEM THOI GIAN / ONLINE");
    CSaveTimeView* pSaveTimeView = CSaveTimeView::create();
    if (pSaveTimeView) {
        CCNode* pParent = this->getParent() ? this->getParent() : this;
        pSaveTimeView->Show(pParent, 60);
    }
}

void CDefaultMainMenu::onClickAwardCenter(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo giao dien: TRUNG TAM TRAO THUONG");
    CAwardCenterView* pAwardView = CAwardCenterView::create();
    if (pAwardView) {
        CCNode* pParent = this->getParent() ? this->getParent() : this;
        pAwardView->Show(pParent, 60);
    }
}

void CDefaultMainMenu::onBtnArena(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo giao dien: DAU TRUONG LOI DAI (SUBMENU_ARENA)");
    if (CMainMenu::sharedManager()) {
        CMainMenu::sharedManager()->changeToSub(SUBMENU_ARENA);
    }
}

void CDefaultMainMenu::onBtnDailyTask(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mo giao dien: NHIEM VU HANG NGAY");
    CDailyTaskView* pTaskView = CDailyTaskView::create();
    if (pTaskView) {
        CCNode* pParent = this->getParent() ? this->getParent() : this;
        pTaskView->Show(pParent, 60);
    }
}
