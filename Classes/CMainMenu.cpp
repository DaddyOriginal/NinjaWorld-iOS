#include "CMainMenu.h"
#include "CCBManager.h"
#include "CDefaultMainMenu.h"
#include "CLoginScene.h"
#include "CMyGroupCardView.h"
#include "CMyBackpackCardView.h"
#include "CChapterView.h"
#include "CPlayerNinjaRecruitView.h"
#include "CPlayerArenaView.h"
#include "CTowerView.h"
#include "CEightGateView.h"
#include "CMoneyTreeView.h"
#include "CRouletteView.h"
#include "CFriendView.h"
#include "CMailView.h"
#include "CAwardCenterView.h"
#include "CGrowthFundView.h"
#include "CSaveTimeView.h"
#include "CDailyTaskView.h"
#include "CDailyRewardView.h"
#include <sstream>

CMainMenu* CMainMenu::s_instance = NULL;

static void updateLabelValue(CCNode* pNode, const char* str) {
    if (!pNode || !str) return;
    if (CCLabelBMFont* bm = dynamic_cast<CCLabelBMFont*>(pNode)) {
        bm->setString(str);
    } else if (CCLabelTTF* ttf = dynamic_cast<CCLabelTTF*>(pNode)) {
        ttf->setString(str);
    }
}

CMainMenu::CMainMenu()
    : m_pNodeContent(NULL)
    , m_pNodeForLua(NULL)
    , m_currentSubMenu(SUBMENU_HOME)
    , m_pCurrentView(NULL)
    , m_pDefaultHomeView(NULL)
    , m_pLayerBuildingContent(NULL)
    , m_pNodeCountryBk(NULL)
    , m_pNodeSlgContent(NULL)
    , m_pMountainBg(NULL)
    , m_pVillageNode(NULL)
    , m_pLabelNickname(NULL)
    , m_pLabelLevel(NULL)
    , m_pLabelGold(NULL)
    , m_pLabelSilver(NULL)
    , m_pLabelBody(NULL)
    , m_pLabelCombatPower(NULL)
{
    s_instance = this;
}

CMainMenu::~CMainMenu() {
    if (s_instance == this) {
        s_instance = NULL;
    }
    CC_SAFE_RELEASE_NULL(m_pNodeContent);
    CC_SAFE_RELEASE_NULL(m_pNodeForLua);
    CC_SAFE_RELEASE_NULL(m_pLayerBuildingContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCountryBk);
    CC_SAFE_RELEASE_NULL(m_pNodeSlgContent);
    CC_SAFE_RELEASE_NULL(m_pLabelNickname);
    CC_SAFE_RELEASE_NULL(m_pLabelLevel);
    CC_SAFE_RELEASE_NULL(m_pLabelGold);
    CC_SAFE_RELEASE_NULL(m_pLabelSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelBody);
    CC_SAFE_RELEASE_NULL(m_pLabelCombatPower);
}

CMainMenu* CMainMenu::sharedMainMenu() {
    return s_instance;
}

CCScene* CMainMenu::scene() {
    CCScene* pScene = CCScene::create();
    CMainMenu* pLayer = CMainMenu::create();
    if (pScene && pLayer) {
        pScene->addChild(pLayer);
        return pScene;
    }
    return NULL;
}

bool CMainMenu::init() {
    if (!CCLayer::init()) {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Nạp khung sườn chính MainMenu.ccbi
    CCNode* pMainNode = CCBManager::sharedManager()->loadNodeFromCCBI("MainMenu.ccbi", this);
    if (pMainNode) {
        pMainNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pMainNode, 0);
        CCLog("[CMainMenu] Nạp MainMenu.ccbi thành công!");
    } else {
        m_pNodeContent = CCNode::create();
        this->addChild(m_pNodeContent, 1);
        m_pNodeForLua = CCNode::create();
        this->addChild(m_pNodeForLua, 10);
    }

    // 2. Nạp bối cảnh Núi Hokage & Làng Lá (home/earth_mountainbk.png + EarthCountryDefaultMenu.ccbi)
    int countryType = CPlayerDataMgr::sharedManager()->getCountryType();
    std::string countryName = "Earth";
    if (countryType == 1) countryName = "Fire";
    else if (countryType == 2) countryName = "Water";
    else if (countryType == 3) countryName = "Wind";
    else if (countryType == 4) countryName = "Earth";
    else if (countryType == 5) countryName = "Mine";

    std::string mountainFile = "home/" + countryName + "_mountainbk.png";
    m_pMountainBg = CCSprite::create(mountainFile.c_str());
    if (!m_pMountainBg) {
        m_pMountainBg = CCSprite::create("home/earth_mountainbk.png");
    }
    if (!m_pMountainBg) {
        m_pMountainBg = CCSprite::create("earth_mountainbk.png");
    }
    if (m_pMountainBg) {
        float scaleX = winSize.width / m_pMountainBg->getContentSize().width;
        float scaleY = (winSize.height * 0.7f) / m_pMountainBg->getContentSize().height;
        float scale = (scaleX > scaleY) ? scaleX : scaleY;
        m_pMountainBg->setScale(scale);
        if (m_pNodeCountryBk) {
            m_pMountainBg->setPosition(CCPointZero);
            m_pNodeCountryBk->addChild(m_pMountainBg);
            CCLog("[CMainMenu] Nạp thành công Núi Hokage (%s) vào node_country_bk", mountainFile.c_str());
        } else {
            m_pMountainBg->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
            this->addChild(m_pMountainBg, -2);
            CCLog("[CMainMenu] Nạp thành công Núi Hokage (%s) tại z=-2", mountainFile.c_str());
        }
    }

    std::string ccbiName = "sub_ui/" + countryName + "CountryDefaultMenu.ccbi";
    m_pVillageNode = CCBManager::sharedManager()->loadNodeFromCCBI(ccbiName.c_str(), this);
    if (!m_pVillageNode) {
        m_pVillageNode = CCBManager::sharedManager()->loadNodeFromCCBI((countryName + "CountryDefaultMenu.ccbi").c_str(), this);
    }
    if (m_pVillageNode) {
        if (m_pLayerBuildingContent) {
            m_pVillageNode->setPosition(CCPointZero);
            m_pLayerBuildingContent->addChild(m_pVillageNode);
            CCLog("[CMainMenu] Gắn mô hình Làng %s vào m_pLayerBuildingContent", countryName.c_str());
        } else {
            m_pVillageNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
            this->addChild(m_pVillageNode, -1);
            CCLog("[CMainMenu] Gắn mô hình Làng %s vào CMainMenu (z=-1)", countryName.c_str());
        }
    }

    // 3. Nạp thanh thông số đỉnh màn hình nguyên bản (NormalTopBar.ccbi)
    CCNode* pTopBar = CCBManager::sharedManager()->loadNodeFromCCBI("NormalTopBar.ccbi", this);
    if (!pTopBar) {
        pTopBar = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/NormalTopBar.ccbi", this);
    }
    if (pTopBar) {
        pTopBar->setPosition(ccp(winSize.width * 0.5f, winSize.height - 40.0f));
        this->addChild(pTopBar, 10);
        CCLog("[CMainMenu] Nạp NormalTopBar.ccbi thành công!");
    }

    return true;
}

void CMainMenu::onEnter() {
    CCLayer::onEnter();
    refreshTopHUD();
}

void CMainMenu::onExit() {
    CCLayer::onExit();
}

void CMainMenu::refreshTopHUD() {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    updateLabelValue(m_pLabelNickname, pData->getNickname().c_str());

    char buf[64];
    snprintf(buf, sizeof(buf), "%d", pData->getLevel());
    updateLabelValue(m_pLabelLevel, buf);

    snprintf(buf, sizeof(buf), "%d", pData->firefly_GetGold());
    updateLabelValue(m_pLabelGold, buf);

    snprintf(buf, sizeof(buf), "%d", pData->firefly_GetSilver());
    updateLabelValue(m_pLabelSilver, buf);

    snprintf(buf, sizeof(buf), "%d/120", pData->firefly_GetBodyValue());
    updateLabelValue(m_pLabelBody, buf);

    snprintf(buf, sizeof(buf), "%d", pData->getCombatPower());
    updateLabelValue(m_pLabelCombatPower, buf);
}

SEL_MenuHandler CMainMenu::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnMainMenu") == 0 || strcmp(pSelectorName, "btn_mainpage") == 0) return menu_selector(CMainMenu::onBtnHome);
    if (strcmp(pSelectorName, "BtnTeam") == 0 || strcmp(pSelectorName, "btn_maingroup") == 0) return menu_selector(CMainMenu::onBtnMyTeam);
    if (strcmp(pSelectorName, "BtnBag") == 0 || strcmp(pSelectorName, "btn_mainpackage") == 0) return menu_selector(CMainMenu::onBtnBackpack);
    if (strcmp(pSelectorName, "BtnFight") == 0 || strcmp(pSelectorName, "btn_mainfight") == 0) return menu_selector(CMainMenu::onBtnFight);
    if (strcmp(pSelectorName, "BtnRound") == 0 || strcmp(pSelectorName, "btn_mainround") == 0) return menu_selector(CMainMenu::onBtnFight);
    if (strcmp(pSelectorName, "BtnStore") == 0 || strcmp(pSelectorName, "btn_mainstore") == 0) return menu_selector(CMainMenu::onBtnStore);
    return NULL;
}

SEL_CCControlHandler CMainMenu::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    // MenuSubBar (Thanh điều hướng dưới)
    if (strcmp(pSelectorName, "BtnMainMenu") == 0 || strcmp(pSelectorName, "btn_mainpage") == 0 || strcmp(pSelectorName, "onBtnHome") == 0) 
        return cccontrol_selector(CMainMenu::onBtnHome);
    if (strcmp(pSelectorName, "BtnTeam") == 0 || strcmp(pSelectorName, "btn_maingroup") == 0 || strcmp(pSelectorName, "onBtnMyTeam") == 0) 
        return cccontrol_selector(CMainMenu::onBtnMyTeam);
    if (strcmp(pSelectorName, "BtnBag") == 0 || strcmp(pSelectorName, "btn_mainpackage") == 0 || strcmp(pSelectorName, "onBtnBackpack") == 0) 
        return cccontrol_selector(CMainMenu::onBtnBackpack);
    if (strcmp(pSelectorName, "BtnFight") == 0 || strcmp(pSelectorName, "btn_mainfight") == 0 || strcmp(pSelectorName, "onBtnFight") == 0) 
        return cccontrol_selector(CMainMenu::onBtnFight);
    if (strcmp(pSelectorName, "BtnRound") == 0 || strcmp(pSelectorName, "btn_mainround") == 0) 
        return cccontrol_selector(CMainMenu::onBtnFight);
    if (strcmp(pSelectorName, "BtnStore") == 0 || strcmp(pSelectorName, "btn_mainstore") == 0 || strcmp(pSelectorName, "onBtnStore") == 0) 
        return cccontrol_selector(CMainMenu::onBtnStore);

    // Hoạt động & Tính năng trên DefaultMainMenu
    if (strcmp(pSelectorName, "BtnDailyTask") == 0 || strcmp(pSelectorName, "btn_daily_task") == 0)
        return cccontrol_selector(CMainMenu::onBtnDailyTask);
    if (strcmp(pSelectorName, "BtnAwardCenter") == 0 || strcmp(pSelectorName, "btn_awardCenter") == 0)
        return cccontrol_selector(CMainMenu::onClickAwardCenter);
    if (strcmp(pSelectorName, "BtnSaveTime") == 0 || strcmp(pSelectorName, "btn_save_time") == 0)
        return cccontrol_selector(CMainMenu::onBtnSaveTime);
    if (strcmp(pSelectorName, "BtnPay") == 0 || strcmp(pSelectorName, "btn_pay") == 0)
        return cccontrol_selector(CMainMenu::onBtnBuyFund);
    if (strcmp(pSelectorName, "BtnRoulette") == 0 || strcmp(pSelectorName, "btn_roulette") == 0 || 
        strcmp(pSelectorName, "BtnMora") == 0 || strcmp(pSelectorName, "btn_mora") == 0)
        return cccontrol_selector(CMainMenu::onBtnRoulette);
    if (strcmp(pSelectorName, "BtnArena") == 0 || strcmp(pSelectorName, "btn_arean") == 0)
        return cccontrol_selector(CMainMenu::onBtnArena);
    if (strcmp(pSelectorName, "BtnTower") == 0 || strcmp(pSelectorName, "btn_tower") == 0)
        return cccontrol_selector(CMainMenu::onBtnTower);
    if (strcmp(pSelectorName, "BtnPlunder") == 0 || strcmp(pSelectorName, "btn_plunder") == 0)
        return cccontrol_selector(CMainMenu::onBtnFight);
    if (strcmp(pSelectorName, "BtnNationWar") == 0 || strcmp(pSelectorName, "btn_countrywar") == 0)
        return cccontrol_selector(CMainMenu::onBtnArena);
    if (strcmp(pSelectorName, "BtnNaruto") == 0 || strcmp(pSelectorName, "btn_naruto") == 0 ||
        strcmp(pSelectorName, "BtnNan") == 0 || strcmp(pSelectorName, "btn_nan") == 0 ||
        strcmp(pSelectorName, "BtnSevenDay") == 0 || strcmp(pSelectorName, "btn_sevenDay") == 0)
        return cccontrol_selector(CMainMenu::onClickNaruto);

    if (strcmp(pSelectorName, "onBtnFriends") == 0) return cccontrol_selector(CMainMenu::onBtnFriends);
    if (strcmp(pSelectorName, "onBtnMessage") == 0) return cccontrol_selector(CMainMenu::onBtnMessage);
    if (strcmp(pSelectorName, "onBtnExp") == 0) return cccontrol_selector(CMainMenu::onBtnExp);
    if (strcmp(pSelectorName, "onBtnDefault") == 0) return cccontrol_selector(CMainMenu::onBtnDefault);
    return NULL;
}

bool CMainMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_forlua", CCNode*, this->m_pNodeForLua);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_buildingcontent", CCNode*, this->m_pLayerBuildingContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_country_bk", CCNode*, this->m_pNodeCountryBk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_slgcontent", CCNode*, this->m_pNodeSlgContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name", CCNode*, this->m_pLabelNickname);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_level", CCNode*, this->m_pLabelLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_goldval", CCNode*, this->m_pLabelGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silverval", CCNode*, this->m_pLabelSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_bodyval", CCNode*, this->m_pLabelBody);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_attackval", CCNode*, this->m_pLabelCombatPower);
    return false;
}

void CMainMenu::changeToSub(SUBMENUTYPE subType) {
    if (m_currentSubMenu == subType && subType != SUBMENU_HOME && m_pCurrentView != NULL) return;
    m_currentSubMenu = subType;

    if (!m_pNodeContent) return;

    if (m_pCurrentView) {
        m_pCurrentView->removeFromParentAndCleanup(true);
        m_pCurrentView = NULL;
    }

    if (subType == SUBMENU_HOME) {
        if (m_pMountainBg) m_pMountainBg->setVisible(true);
        if (m_pVillageNode) m_pVillageNode->setVisible(true);
        CCLog("[CMainMenu] Chuyển về Trang chủ Làng");
        return;
    } else {
        if (m_pMountainBg) m_pMountainBg->setVisible(false);
        if (m_pVillageNode) m_pVillageNode->setVisible(false);
    }

    switch (subType) {
        case SUBMENU_HOME:
            break;
        case SUBMENU_BAG: {
            CCLog("[CMainMenu] Chuyển sang Túi Đồ (CMyBackpackCardView)");
            CMyBackpackCardView* pBagView = CMyBackpackCardView::create();
            if (pBagView) {
                m_pNodeContent->addChild(pBagView);
                m_pCurrentView = pBagView;
            }
            break;
        }
        case SUBMENU_NINJA: {
            CCLog("[CMainMenu] Chuyển sang Đội Hình Nhẫn Giả (CMyGroupCardView)");
            CMyGroupCardView* pGroupView = CMyGroupCardView::create();
            if (pGroupView) {
                m_pNodeContent->addChild(pGroupView);
                m_pCurrentView = pGroupView;
            }
            break;
        }
        case SUBMENU_DUNGEON: {
            CCLog("[CMainMenu] Chuyển sang Vượt Ải Cốt Truyện (CChapterView)");
            CChapterView* pChapterView = CChapterView::create();
            if (pChapterView) {
                m_pNodeContent->addChild(pChapterView);
                m_pCurrentView = pChapterView;
            }
            break;
        }
        case SUBMENU_ARENA: {
            CCLog("[CMainMenu] Chuyển sang Đấu Trường Lôi Đài (CPlayerArenaView)");
            CPlayerArenaView* pArenaView = CPlayerArenaView::create();
            if (pArenaView) {
                m_pNodeContent->addChild(pArenaView);
                m_pCurrentView = pArenaView;
            }
            break;
        }
        case SUBMENU_SHOP: {
            CCLog("[CMainMenu] Chuyển sang Chiêu Mộ Quán Trà (CPlayerNinjaRecruitView)");
            CPlayerNinjaRecruitView* pRecruitView = CPlayerNinjaRecruitView::create();
            if (pRecruitView) {
                m_pNodeContent->addChild(pRecruitView);
                m_pCurrentView = pRecruitView;
            }
            break;
        }
        case SUBMENU_TOWER: {
            CCLog("[CMainMenu] Chuyển sang Tháp Thí Luyện (CTowerView)");
            CTowerView* pTowerView = CTowerView::create();
            if (pTowerView) {
                m_pNodeContent->addChild(pTowerView);
                m_pCurrentView = pTowerView;
            }
            break;
        }
        case SUBMENU_EIGHTGATE: {
            CEightGateView* pGateView = CEightGateView::create();
            if (pGateView) {
                m_pNodeContent->addChild(pGateView);
                m_pCurrentView = pGateView;
            }
            break;
        }
        case SUBMENU_MONEYTREE: {
            CMoneyTreeView* pTreeView = CMoneyTreeView::create();
            if (pTreeView) {
                m_pNodeContent->addChild(pTreeView);
                m_pCurrentView = pTreeView;
            }
            break;
        }
        case SUBMENU_ROULETTE: {
            CRouletteView* pRouletteView = CRouletteView::create();
            if (pRouletteView) {
                m_pNodeContent->addChild(pRouletteView);
                m_pCurrentView = pRouletteView;
            }
            break;
        }
        case SUBMENU_FRIEND: {
            CFriendView* pFriendView = CFriendView::create();
            if (pFriendView) {
                m_pNodeContent->addChild(pFriendView);
                m_pCurrentView = pFriendView;
            }
            break;
        }
        case SUBMENU_MAIL: {
            CMailView* pMailView = CMailView::create();
            if (pMailView) {
                m_pNodeContent->addChild(pMailView);
                m_pCurrentView = pMailView;
            }
            break;
        }
        default:
            break;
    }
}

void CMainMenu::onBtnHome(CCObject* pSender) { changeToSub(SUBMENU_HOME); }
void CMainMenu::onBtnMyTeam(CCObject* pSender) { changeToSub(SUBMENU_NINJA); }
void CMainMenu::onBtnBackpack(CCObject* pSender) { changeToSub(SUBMENU_BAG); }
void CMainMenu::onBtnFight(CCObject* pSender) { changeToSub(SUBMENU_DUNGEON); }
void CMainMenu::onBtnTower(CCObject* pSender) { changeToSub(SUBMENU_TOWER); }
void CMainMenu::onBtnStore(CCObject* pSender) { changeToSub(SUBMENU_SHOP); }
void CMainMenu::onBtnFriends(CCObject* pSender) { changeToSub(SUBMENU_FRIEND); }
void CMainMenu::onBtnMessage(CCObject* pSender) { changeToSub(SUBMENU_MAIL); }
void CMainMenu::onBtnExp(CCObject* pSender) { changeToSub(SUBMENU_ACTIVITY); }
void CMainMenu::onBtnDefault(CCObject* pSender) { changeToSub(SUBMENU_HOME); }

void CMainMenu::onBtnDailyTask(CCObject* pSender) {
    CCLog("[CMainMenu] Mở Nhiệm Vụ Hàng Ngày (CDailyTaskView)");
    if (!m_pNodeContent) return;
    if (m_pCurrentView) { m_pCurrentView->removeFromParentAndCleanup(true); m_pCurrentView = NULL; }
    m_currentSubMenu = SUBMENU_ACTIVITY;
    if (m_pMountainBg) m_pMountainBg->setVisible(false);
    if (m_pVillageNode) m_pVillageNode->setVisible(false);
    CDailyTaskView* pView = CDailyTaskView::create();
    if (pView) {
        m_pNodeContent->addChild(pView);
        m_pCurrentView = pView;
    }
}

void CMainMenu::onClickAwardCenter(CCObject* pSender) {
    CCLog("[CMainMenu] Mở Trung Tâm Thưởng (CAwardCenterView)");
    if (!m_pNodeContent) return;
    if (m_pCurrentView) { m_pCurrentView->removeFromParentAndCleanup(true); m_pCurrentView = NULL; }
    m_currentSubMenu = SUBMENU_ACTIVITY;
    if (m_pMountainBg) m_pMountainBg->setVisible(false);
    if (m_pVillageNode) m_pVillageNode->setVisible(false);
    CAwardCenterView* pView = CAwardCenterView::create();
    if (pView) {
        m_pNodeContent->addChild(pView);
        m_pCurrentView = pView;
    }
}

void CMainMenu::onBtnSaveTime(CCObject* pSender) {
    CCLog("[CMainMenu] Mở Tiết Kiệm Thời Gian / Đồng Hồ EXP (CSaveTimeView)");
    if (!m_pNodeContent) return;
    if (m_pCurrentView) { m_pCurrentView->removeFromParentAndCleanup(true); m_pCurrentView = NULL; }
    m_currentSubMenu = SUBMENU_ACTIVITY;
    if (m_pMountainBg) m_pMountainBg->setVisible(false);
    if (m_pVillageNode) m_pVillageNode->setVisible(false);
    CSaveTimeView* pView = CSaveTimeView::create();
    if (pView) {
        m_pNodeContent->addChild(pView);
        m_pCurrentView = pView;
    }
}

void CMainMenu::onBtnBuyFund(CCObject* pSender) {
    CCLog("[CMainMenu] Mở Quỹ Trưởng Thành / Nạp Đầu (CGrowthFundView)");
    if (!m_pNodeContent) return;
    if (m_pCurrentView) { m_pCurrentView->removeFromParentAndCleanup(true); m_pCurrentView = NULL; }
    m_currentSubMenu = SUBMENU_ACTIVITY;
    if (m_pMountainBg) m_pMountainBg->setVisible(false);
    if (m_pVillageNode) m_pVillageNode->setVisible(false);
    CGrowthFundView* pView = CGrowthFundView::create();
    if (pView) {
        m_pNodeContent->addChild(pView);
        m_pCurrentView = pView;
    }
}

void CMainMenu::onBtnRoulette(CCObject* pSender) {
    CCLog("[CMainMenu] Mở Vòng Quay May Mắn (CRouletteView)");
    changeToSub(SUBMENU_ROULETTE);
}

void CMainMenu::onBtnArena(CCObject* pSender) {
    CCLog("[CMainMenu] Mở Đấu Trường (CPlayerArenaView)");
    changeToSub(SUBMENU_ARENA);
}

void CMainMenu::onClickNaruto(CCObject* pSender) {
    CCLog("[CMainMenu] Nhận Quà Điểm Danh (CDailyRewardView)");
    if (!m_pNodeContent) return;
    if (m_pCurrentView) { m_pCurrentView->removeFromParentAndCleanup(true); m_pCurrentView = NULL; }
    m_currentSubMenu = SUBMENU_ACTIVITY;
    if (m_pMountainBg) m_pMountainBg->setVisible(false);
    if (m_pVillageNode) m_pVillageNode->setVisible(false);
    CDailyRewardView* pView = CDailyRewardView::create();
    if (pView) {
        m_pNodeContent->addChild(pView);
        m_pCurrentView = pView;
    }
}
