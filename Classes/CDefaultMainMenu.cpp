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

    // 1. Nạp giao diện DefaultMainMenu.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("DefaultMainMenu.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/DefaultMainMenu.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode, 0);
        CCLog("[CDefaultMainMenu] Nạp DefaultMainMenu.ccbi thành công!");
    } else {
        CCLog("[CDefaultMainMenu] Failed to load DefaultMainMenu.ccbi, dùng layer nền!");
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg);
    }

    // 2. Nạp bối cảnh ngôi làng nguyên bản của Quốc gia (Earth / Fire / Water / Wind / Mine)
    int countryType = CPlayerDataMgr::sharedManager()->getCountryType();
    std::string countryName = "Earth";
    if (countryType == 1) countryName = "Fire";
    else if (countryType == 2) countryName = "Water";
    else if (countryType == 3) countryName = "Wind";
    else if (countryType == 4) countryName = "Earth";
    else if (countryType == 5) countryName = "Mine";

    std::string ccbiName = countryName + "CountryDefaultMenu.ccbi";
    CCNode* pCountryNode = CCBManager::sharedManager()->loadNodeFromCCBI(ccbiName.c_str(), this);
    if (!pCountryNode) {
        pCountryNode = CCBManager::sharedManager()->loadNodeFromCCBI(("sub_ui/" + ccbiName).c_str(), this);
    }

    if (pCountryNode) {
        if (m_pLayerBuildingContent) {
            m_pLayerBuildingContent->addChild(pCountryNode);
            CCLog("[CDefaultMainMenu] Nạp thành công bối cảnh Làng %s vào layer_buildingcontent!", countryName.c_str());
        } else if (m_pNodeCountryBk) {
            m_pNodeCountryBk->addChild(pCountryNode);
            CCLog("[CDefaultMainMenu] Nạp thành công bối cảnh Làng %s vào node_country_bk!", countryName.c_str());
        } else {
            this->addChild(pCountryNode, -1);
            CCLog("[CDefaultMainMenu] Nạp thành công bối cảnh Làng %s vào this (-1)!", countryName.c_str());
        }
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

    CCLog("[CDefaultMainMenu] Cập nhật thông số Làng: LucChien=%d", pData->getCombatPower());
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CDefaultMainMenu::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CDefaultMainMenu::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnTower") == 0 || strcmp(pSelectorName, "BtnTower") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnTower);
    }
    if (strcmp(pSelectorName, "onBtnActivity") == 0 || strcmp(pSelectorName, "BtnActivity") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnActivity);
    }
    if (strcmp(pSelectorName, "onClickNaruto") == 0 || strcmp(pSelectorName, "BtnNaruto") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onClickNaruto);
    }
    if (strcmp(pSelectorName, "onBtnBuyFund") == 0 || strcmp(pSelectorName, "BtnBuyFund") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnBuyFund);
    }
    if (strcmp(pSelectorName, "onBtnSaveTime") == 0 || strcmp(pSelectorName, "BtnSaveTime") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnSaveTime);
    }
    if (strcmp(pSelectorName, "onClickAwardCenter") == 0 || strcmp(pSelectorName, "BtnAwardCenter") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onClickAwardCenter);
    }
    if (strcmp(pSelectorName, "onBtnArena") == 0 || strcmp(pSelectorName, "BtnArena") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnArena);
    }
    if (strcmp(pSelectorName, "onBtnDailyTask") == 0 || strcmp(pSelectorName, "BtnDailyTask") == 0) {
        return cccontrol_selector(CDefaultMainMenu::onBtnDailyTask);
    }
    return NULL;
}

bool CDefaultMainMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_buildingcontent", CCNode*, this->m_pLayerBuildingContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_country_bk", CCNode*, this->m_pNodeCountryBk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_menubtns", CCNode*, this->m_pNodeMenuBtns);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxattack", CCLabelBMFont*, this->m_pLabelMaxAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxdefense", CCLabelBMFont*, this->m_pLabelMaxHonor);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxhonor", CCLabelBMFont*, this->m_pLabelMaxHonor);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_msg_news", CCLabelBMFont*, this->m_pLabelMsgNews);

    return false;
}

// -------------------------------------------------------------
// SỰ KIỆN CÁC TÒA NHÀ & TÍNH NĂNG
// -------------------------------------------------------------
void CDefaultMainMenu::onBtnTower(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Tháp Thí Luyện (CTowerView)!");
    if (CMainMenu::sharedMainMenu()) {
        CMainMenu::sharedMainMenu()->onBtnTower(pSender);
    }
}

void CDefaultMainMenu::onBtnActivity(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Trung Tâm Hoạt Động (CAwardCenterView)!");
    CAwardCenterView* pView = CAwardCenterView::create();
    if (pView && CMainMenu::sharedMainMenu()) {
        CMainMenu::sharedMainMenu()->changeSubMenu(SUBMENU_HOME);
        this->addChild(pView, 100);
    }
}

void CDefaultMainMenu::onClickNaruto(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Bấm vào Naruto / Nhận thưởng hàng ngày!");
    if (CMainMenu::sharedMainMenu()) {
        CMainMenu::sharedMainMenu()->onBtnFriends(pSender);
    }
}

void CDefaultMainMenu::onBtnBuyFund(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Quỹ Trưởng Thành (CGrowthFundView)!");
    CGrowthFundView* pView = CGrowthFundView::create();
    if (pView) {
        this->addChild(pView, 100);
    }
}

void CDefaultMainMenu::onBtnSaveTime(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Sự Kiện Nạp Tích Lũy (CSaveTimeView)!");
    CSaveTimeView* pView = CSaveTimeView::create();
    if (pView) {
        this->addChild(pView, 100);
    }
}

void CDefaultMainMenu::onClickAwardCenter(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Phúc Lợi Máy Chủ!");
    onBtnActivity(pSender, pEvent);
}

void CDefaultMainMenu::onBtnArena(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Đấu Trường Ninja!");
    if (CMainMenu::sharedMainMenu()) {
        CMainMenu::sharedMainMenu()->onBtnFight(pSender);
    }
}

void CDefaultMainMenu::onBtnDailyTask(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDefaultMainMenu] Mở Nhiệm Vụ Hàng Ngày (CDailyTaskView)!");
    CDailyTaskView* pView = CDailyTaskView::create();
    if (pView) {
        this->addChild(pView, 100);
    }
}
