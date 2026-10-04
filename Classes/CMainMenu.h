#ifndef _CMAIN_MENU_H_
#define _CMAIN_MENU_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CPlayerDataMgr.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * Các phân hệ màn hình con (Sub Menu) trong Làng Lá
 */
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

/**
 * CMainMenu: Khung xương Scene Sảnh Chính điều phối toàn bộ các View và tầng Lua (Firefly MMO)
 * Gắn kết với MainMenu.ccbi
 */
class CMainMenu 
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    static CMainMenu* s_instance;

    // Các container node từ MainMenu.ccbi
    CCNode* m_pNodeContent;   // Container chứa các SubView (Trang chủ, Túi đồ, Tướng)
    CCNode* m_pNodeForLua;    // Container dành riêng cho các popup và kịch bản Lua

    // Quản lý View con hiện tại
    SUBMENUTYPE m_currentSubMenu;
    CCNode* m_pCurrentView;
    CDefaultMainMenu* m_pDefaultHomeView;

    // Top HUD Labels (Cập nhật từ CPlayerDataMgr)
    CCLabelTTF* m_pLabelNickname;
    CCLabelTTF* m_pLabelLevel;
    CCLabelTTF* m_pLabelGold;
    CCLabelTTF* m_pLabelSilver;
    CCLabelTTF* m_pLabelBody;
    CCLabelTTF* m_pLabelServer;
    CCLabelTTF* m_pLabelCombatPower;

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

    // CCB Binders
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Chuyển đổi giữa các phân hệ màn hình (Home, Túi đồ, Tướng, Phụ bản)
    void changeToSub(SUBMENUTYPE subType);
    SUBMENUTYPE getCurrentSubMenuType() const { return m_currentSubMenu; }

    // Cập nhật chỉ số trên Top HUD từ CPlayerDataMgr
    void refreshTopHUD();

    // Node dành cho tầng Lua
    CCNode* getNodeForLua() const { return m_pNodeForLua; }

    // Sự kiện điều hướng Bottom Bar
    void onBtnHome(CCObject* pSender, CCControlEvent pEvent);
    void onBtnNinja(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBackpack(CCObject* pSender, CCControlEvent pEvent);
    void onBtnDungeon(CCObject* pSender, CCControlEvent pEvent);
    void onBtnActivity(CCObject* pSender, CCControlEvent pEvent);
    void onBtnLogout(CCObject* pSender, CCControlEvent pEvent);
};

#endif // _CMAIN_MENU_H_
