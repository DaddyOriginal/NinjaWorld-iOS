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

    // 2. Nạp thanh thông số đỉnh màn hình nguyên bản (NormalTopBar.ccbi)
    CCNode* pTopBar = CCBManager::sharedManager()->loadNodeFromCCBI("NormalTopBar.ccbi", this);
    if (!pTopBar) {
        pTopBar = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/NormalTopBar.ccbi", this);
    }
    if (pTopBar) {
        pTopBar->setPosition(ccp(winSize.width * 0.5f, winSize.height - 40.0f));
        this->addChild(pTopBar, 10);
        CCLog("[CMainMenu] Nạp NormalTopBar.ccbi thành công!");
    }

    // 3. Nếu node_content chưa có View, thêm CDefaultMainMenu ban đầu
    if (m_pNodeContent && m_pNodeContent->getChildrenCount() == 0) {
        m_pDefaultHomeView = CDefaultMainMenu::create();
        if (m_pDefaultHomeView) {
            m_pNodeContent->addChild(m_pDefaultHomeView);
            m_pCurrentView = m_pDefaultHomeView;
        }
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
    return NULL;
}

SEL_CCControlHandler CMainMenu::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnHome") == 0) return cccontrol_selector(CMainMenu::onBtnHome);
    if (strcmp(pSelectorName, "onBtnMyTeam") == 0) return cccontrol_selector(CMainMenu::onBtnMyTeam);
    if (strcmp(pSelectorName, "onBtnBackpack") == 0) return cccontrol_selector(CMainMenu::onBtnBackpack);
    if (strcmp(pSelectorName, "onBtnFight") == 0) return cccontrol_selector(CMainMenu::onBtnFight);
    if (strcmp(pSelectorName, "onBtnTower") == 0) return cccontrol_selector(CMainMenu::onBtnTower);
    if (strcmp(pSelectorName, "onBtnStore") == 0) return cccontrol_selector(CMainMenu::onBtnStore);
    if (strcmp(pSelectorName, "onBtnFriends") == 0) return cccontrol_selector(CMainMenu::onBtnFriends);
    if (strcmp(pSelectorName, "onBtnMessage") == 0) return cccontrol_selector(CMainMenu::onBtnMessage);
    if (strcmp(pSelectorName, "onBtnExp") == 0) return cccontrol_selector(CMainMenu::onBtnExp);
    if (strcmp(pSelectorName, "onBtnDefault") == 0) return cccontrol_selector(CMainMenu::onBtnDefault);
    return NULL;
}

bool CMainMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_forlua", CCNode*, this->m_pNodeForLua);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name", CCNode*, this->m_pLabelNickname);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_level", CCNode*, this->m_pLabelLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_goldval", CCNode*, this->m_pLabelGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silverval", CCNode*, this->m_pLabelSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_bodyval", CCNode*, this->m_pLabelBody);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_attackval", CCNode*, this->m_pLabelCombatPower);
    return false;
}

void CMainMenu::changeToSub(SUBMENUTYPE subType) {
    if (m_currentSubMenu == subType) return;
    m_currentSubMenu = subType;

    if (!m_pNodeContent) return;

    if (m_pCurrentView) {
        m_pCurrentView->removeFromParentAndCleanup(true);
        m_pCurrentView = NULL;
    }

    switch (subType) {
        case SUBMENU_HOME: {
            m_pDefaultHomeView = CDefaultMainMenu::create();
            m_pNodeContent->addChild(m_pDefaultHomeView);
            m_pCurrentView = m_pDefaultHomeView;
            CCLog("[CMainMenu] Chuyển về Trang chủ Làng");
            break;
        }
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
