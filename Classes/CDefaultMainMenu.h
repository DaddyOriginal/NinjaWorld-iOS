#ifndef _CDEFAULT_MAIN_MENU_H_
#define _CDEFAULT_MAIN_MENU_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CDefaultMainMenu: Màn hình Trang Chủ Làng Lá (chứa các tòa nhà công trình và hoạt động)
 * Gắn kết với DefaultMainMenu.ccbi
 */
class CDefaultMainMenu
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCNode* m_pLayerBuildingContent; // Container các công trình trong Làng
    CCNode* m_pNodeCountryBk;        // Nền làng theo quốc gia (Hỏa, Phong, Thủy, Thổ, Lôi)
    CCNode* m_pNodeMenuBtns;         // Container các nút phụ

    // Nhãn thông số
    CCLabelBMFont* m_pLabelMaxAttack; // Lực chiến cao nhất
    CCLabelBMFont* m_pLabelMaxHonor;  // Danh vọng / Công trạng
    CCLabelBMFont* m_pLabelMsgNews;   // Chấm đỏ thông báo

public:
    CDefaultMainMenu();
    virtual ~CDefaultMainMenu();

    CREATE_FUNC(CDefaultMainMenu);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Nạp dữ liệu người chơi lên màn hình Làng Lá
    void firefly_LoadUserInfo();

    // Callbacks các công trình & tính năng
    void onBtnTower(CCObject* pSender, CCControlEvent pEvent);
    void onBtnActivity(CCObject* pSender, CCControlEvent pEvent);
    void onClickNaruto(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBuyFund(CCObject* pSender, CCControlEvent pEvent);
    void onBtnSaveTime(CCObject* pSender, CCControlEvent pEvent);
    void onClickAwardCenter(CCObject* pSender, CCControlEvent pEvent);
    void onBtnArena(CCObject* pSender, CCControlEvent pEvent);
    void onBtnDailyTask(CCObject* pSender, CCControlEvent pEvent);
};

#endif // _CDEFAULT_MAIN_MENU_H_
