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
    , m_pLabelServer(NULL)
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
        CCLog("[CMainMenu] Nap MainMenu.ccbi thanh cong!");
    } else {
        CCLog("[CMainMenu] Fallback root container");
        m_pNodeContent = CCNode::create();
        this->addChild(m_pNodeContent, 1);

        m_pNodeForLua = CCNode::create();
        this->addChild(m_pNodeForLua, 10);
    }

    // 2. Nạp View mặc định ban đầu: CDefaultMainMenu (Trang chủ Làng Lá)
    m_pDefaultHomeView = CDefaultMainMenu::create();
    if (m_pDefaultHomeView) {
        if (m_pNodeContent) {
            m_pNodeContent->addChild(m_pDefaultHomeView);
        } else {
            this->addChild(m_pDefaultHomeView, 1);
        }
        m_pCurrentView = m_pDefaultHomeView;
    }

    // 3. Khởi tạo Top HUD (Thanh thông số đỉnh màn hình)
    CCLayerColor* pTopBar = CCLayerColor::create(ccc4(10, 15, 26, 230), winSize.width, 90.0f);
    pTopBar->setPosition(ccp(0, winSize.height - 90.0f));
    this->addChild(pTopBar, 20);

    // Tên nhân vật & Cấp độ
    m_pLabelNickname = CCLabelTTF::create("Ninja", "Helvetica-Bold", 22.0f);
    m_pLabelNickname->setPosition(ccp(110.0f, winSize.height - 30.0f));
    m_pLabelNickname->setColor(ccc3(255, 255, 255));
    this->addChild(m_pLabelNickname, 21);

    m_pLabelLevel = CCLabelTTF::create("Lv.1", "Helvetica-Bold", 18.0f);
    m_pLabelLevel->setPosition(ccp(110.0f, winSize.height - 60.0f));
    m_pLabelLevel->setColor(ccc3(245, 158, 11)); // Vàng cam
    this->addChild(m_pLabelLevel, 21);

    // Vàng
    m_pLabelGold = CCLabelTTF::create("Vàng: 0", "Helvetica-Bold", 18.0f);
    m_pLabelGold->setPosition(ccp(260.0f, winSize.height - 45.0f));
    m_pLabelGold->setColor(ccc3(251, 191, 36));
    this->addChild(m_pLabelGold, 21);

    // Bạc
    m_pLabelSilver = CCLabelTTF::create("Bạc: 0", "Helvetica-Bold", 18.0f);
    m_pLabelSilver->setPosition(ccp(400.0f, winSize.height - 45.0f));
    m_pLabelSilver->setColor(ccc3(226, 232, 240));
    this->addChild(m_pLabelSilver, 21);

    // Thể lực
    m_pLabelBody = CCLabelTTF::create("Thể lực: 120/120", "Helvetica-Bold", 18.0f);
    m_pLabelBody->setPosition(ccp(540.0f, winSize.height - 45.0f));
    m_pLabelBody->setColor(ccc3(52, 211, 153)); // Xanh lục
    this->addChild(m_pLabelBody, 21);

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

    if (m_pLabelNickname) {
        m_pLabelNickname->setString(pData->getNickname().c_str());
    }

    if (m_pLabelLevel) {
        std::stringstream ss;
        ss << "Lv." << pData->getLevel();
        m_pLabelLevel->setString(ss.str().c_str());
    }

    if (m_pLabelGold) {
        std::stringstream ss;
        ss << "Vàng: " << pData->firefly_GetGold();
        m_pLabelGold->setString(ss.str().c_str());
    }

    if (m_pLabelSilver) {
        std::stringstream ss;
        ss << "Bạc: " << pData->firefly_GetSilver();
        m_pLabelSilver->setString(ss.str().c_str());
    }

    if (m_pLabelBody) {
        std::stringstream ss;
        ss << "Thể lực: " << pData->firefly_GetBodyValue() << "/120";
        m_pLabelBody->setString(ss.str().c_str());
    }

    CCLog("[CMainMenu] Da cap nhat Top HUD cho nhan vat: %s (Lv.%d)", 
          pData->getNickname().c_str(), pData->getLevel());
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO MainMenu.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CMainMenu::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CMainMenu::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

bool CMainMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_forlua", CCNode*, this->m_pNodeForLua);
    return false;
}

// -------------------------------------------------------------
// ĐIỀU PHỐI CHUYỂN PHÂN HỆ SUBMENU
// -------------------------------------------------------------
void CMainMenu::changeToSub(SUBMENUTYPE subType) {
    if (m_currentSubMenu == subType) return;
    m_currentSubMenu = subType;

    if (!m_pNodeContent) return;

    // Xóa view con hiện tại
    if (m_pCurrentView) {
        m_pCurrentView->removeFromParentAndCleanup(true);
        m_pCurrentView = NULL;
    }

    switch (subType) {
        case SUBMENU_HOME: {
            m_pDefaultHomeView = CDefaultMainMenu::create();
            m_pNodeContent->addChild(m_pDefaultHomeView);
            m_pCurrentView = m_pDefaultHomeView;
            CCLog("[CMainMenu] Chuyen ve Trang chu Lang La");
            break;
        }
        case SUBMENU_BAG: {
            CCLog("[CMainMenu] Chuyen sang Tui Do (CMyBackpackCardView)");
            CMyBackpackCardView* pBagView = CMyBackpackCardView::create();
            if (pBagView) {
                m_pNodeContent->addChild(pBagView);
                m_pCurrentView = pBagView;
            }
            break;
        }
        case SUBMENU_NINJA: {
            CCLog("[CMainMenu] Chuyen sang Doi Hinh Nhan Gia (CMyGroupCardView)");
            CMyGroupCardView* pGroupView = CMyGroupCardView::create();
            if (pGroupView) {
                m_pNodeContent->addChild(pGroupView);
                m_pCurrentView = pGroupView;
            }
            break;
        }
        case SUBMENU_DUNGEON: {
            CCLog("[CMainMenu] Chuyen sang Phu Ban Cot Truyen (CChapterView)");
            CChapterView* pChapView = CChapterView::create();
            if (pChapView) {
                m_pNodeContent->addChild(pChapView);
                m_pCurrentView = pChapView;
            }
            break;
        }
        case SUBMENU_SHOP: {
            CCLog("[CMainMenu] Chuyen sang Chieu Mo & Cua Hang (CPlayerNinjaRecruitView)");
            CPlayerNinjaRecruitView* pRecruitView = CPlayerNinjaRecruitView::create();
            if (pRecruitView) {
                m_pNodeContent->addChild(pRecruitView);
                m_pCurrentView = pRecruitView;
            }
            break;
        }
        case SUBMENU_ARENA: {
            CCLog("[CMainMenu] Chuyen sang Dau Truong Loi Dai (CPlayerArenaView)");
            CPlayerArenaView* pArenaView = CPlayerArenaView::create();
            if (pArenaView) {
                m_pNodeContent->addChild(pArenaView);
                m_pCurrentView = pArenaView;
            }
            break;
        }
        case SUBMENU_TOWER: {
            CCLog("[CMainMenu] Chuyen sang Leo Thap Thi Luyen (CTowerView)");
            CTowerView* pTowerView = CTowerView::create();
            if (pTowerView) {
                m_pNodeContent->addChild(pTowerView);
                m_pCurrentView = pTowerView;
            }
            break;
        }
        case SUBMENU_ACTIVITY:
        case SUBMENU_EIGHTGATE: {
            CCLog("[CMainMenu] Chuyen sang Bat Mon Don Giap (CEightGateView)");
            CEightGateView* pGateView = CEightGateView::create();
            if (pGateView) {
                m_pNodeContent->addChild(pGateView);
                m_pCurrentView = pGateView;
            }
            break;
        }
        case SUBMENU_MONEYTREE: {
            CCLog("[CMainMenu] Chuyen sang Cay Rung Tien (CMoneyTreeView)");
            CMoneyTreeView* pTreeView = CMoneyTreeView::create();
            if (pTreeView) {
                m_pNodeContent->addChild(pTreeView);
                m_pCurrentView = pTreeView;
            }
            break;
        }
        case SUBMENU_ROULETTE: {
            CCLog("[CMainMenu] Chuyen sang Vong Quay May Man (CRouletteView)");
            CRouletteView* pRouletteView = CRouletteView::create();
            if (pRouletteView) {
                m_pNodeContent->addChild(pRouletteView);
                m_pCurrentView = pRouletteView;
            }
            break;
        }
        case SUBMENU_FRIEND: {
            CCLog("[CMainMenu] Chuyen sang Ban Be (CFriendView)");
            CFriendView* pFriendView = CFriendView::create();
            if (pFriendView) {
                m_pNodeContent->addChild(pFriendView);
                m_pCurrentView = pFriendView;
            }
            break;
        }
        case SUBMENU_MAIL: {
            CCLog("[CMainMenu] Chuyen sang Hom Thu (CMailView)");
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

    refreshTopHUD();
}

void CMainMenu::onBtnHome(CCObject* pSender, CCControlEvent pEvent) {
    changeToSub(SUBMENU_HOME);
}

void CMainMenu::onBtnNinja(CCObject* pSender, CCControlEvent pEvent) {
    changeToSub(SUBMENU_NINJA);
}

void CMainMenu::onBtnBackpack(CCObject* pSender, CCControlEvent pEvent) {
    changeToSub(SUBMENU_BAG);
}

void CMainMenu::onBtnDungeon(CCObject* pSender, CCControlEvent pEvent) {
    changeToSub(SUBMENU_DUNGEON);
}

void CMainMenu::onBtnActivity(CCObject* pSender, CCControlEvent pEvent) {
    changeToSub(SUBMENU_ACTIVITY);
}

void CMainMenu::onBtnLogout(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CMainMenu] Dang xuat khoi tai khoan!");
    CCScene* pLogin = CLoginScene::scene();
    if (pLogin) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pLogin));
    }
}
