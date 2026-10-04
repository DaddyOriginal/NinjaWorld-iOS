#ifndef _CMAIN_MENU_H_
#define _CMAIN_MENU_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CPlayerDataMgr.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

enum SUBMENUTYPE {
    SUBMENU_HOME = 0,      // Trang chủ Làng Lá (CDefaultMainMenu)
    SUBMENU_NINJA = 1,     // Đội hình & Danh sách Nhẫn Giả
    SUBMENU_BAG = 2,       // Túi đồ & Trang bị
    SUBMENU_DUNGEON = 3,   // Phụ bản cốt truyện
    SUBMENU_ARENA = 4,     // Đấu trường Lôi Đài
    SUBMENU_TOWER = 5,     // Leo tháp Thí Luyện
    SUBMENU_SHOP = 6,      // Cửa hàng & Chiêu mộ
    SUBMENU_ACTIVITY = 7,  // Hoạt động & Sự kiện
    SUBMENU_EIGHTGATE = 8, // Bát Môn Độn Giáp & Luyện Hồn
    SUBMENU_MONEYTREE = 9, // Cây Rung Tiền
    SUBMENU_ROULETTE = 10, // Vòng Quay May Mắn
    SUBMENU_FRIEND = 11,   // Bạn Bè
    SUBMENU_MAIL = 12      // Hòm Thư
};

class CDefaultMainMenu;

class CMainMenu 
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    static CMainMenu* s_instance;

    // Các container node từ MainMenu.ccbi
    CCNode* m_pNodeContent;   // Container chứa các SubView
    CCNode* m_pNodeForLua;    // Container kịch bản Lua

    // Quản lý View con hiện tại
    SUBMENUTYPE m_currentSubMenu;
    CCNode* m_pCurrentView;
    CDefaultMainMenu* m_pDefaultHomeView;

    // Top HUD Labels từ NormalTopBar.ccbi
    CCNode* m_pLabelNickname;
    CCNode* m_pLabelLevel;
    CCNode* m_pLabelGold;
    CCNode* m_pLabelSilver;
    CCNode* m_pLabelBody;
    CCNode* m_pLabelCombatPower;

public:
    CMainMenu();
    virtual ~CMainMenu();

    static CCScene* scene();
    static CMainMenu* sharedMainMenu();
    static CMainMenu* sharedManager() { return sharedMainMenu(); }
    CREATE_FUNC(CMainMenu);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void refreshTopHUD();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Chuyển phân hệ
    void changeToSub(SUBMENUTYPE subType);
    void changeSubMenu(SUBMENUTYPE subType) { changeToSub(subType); }

    // Sự kiện MenuSubBar
    void onBtnHome(CCObject* pSender);
    void onBtnMyTeam(CCObject* pSender);
    void onBtnBackpack(CCObject* pSender);
    void onBtnFight(CCObject* pSender);
    void onBtnTower(CCObject* pSender);
    void onBtnStore(CCObject* pSender);
    void onBtnFriends(CCObject* pSender);
    void onBtnMessage(CCObject* pSender);
    void onBtnExp(CCObject* pSender);
    void onBtnDefault(CCObject* pSender);

    CCNode* getNodeContent() { return m_pNodeContent; }
};

#endif // _CMAIN_MENU_H_
