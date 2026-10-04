#ifndef _CREGISTER_VIEW_H_
#define _CREGISTER_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

class CLoginScene;

/**
 * CRegisterView: Màn hình Đăng Ký Tài Khoản nguyên bản từ RegisterView.ccbi
 * Cho phép người chơi tạo tài khoản mới với mật khẩu và xác nhận mật khẩu
 */
class CRegisterView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCEditBoxDelegate
{
private:
    CLoginScene* m_pLoginScene;

    // CCB Variable Binders từ RegisterView.ccbi
    CCNode* m_pSpriteName;
    CCNode* m_pSpritePwd;
    CCNode* m_pSpritePwd1;
    CCControlButton* m_pBtnReg;
    CCControlButton* m_pBtnBack;
    CCControlButton* m_pBtnClose;
    CCLabelTTF* m_pLabelTip;

    // EditBox nhập liệu
    CCEditBox* m_pEditUser;
    CCEditBox* m_pEditPwd;
    CCEditBox* m_pEditPwd1;

public:
    CRegisterView();
    virtual ~CRegisterView();

    static CRegisterView* create(CLoginScene* pScene);
    bool init(CLoginScene* pScene);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnReg(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBack(CCObject* pSender, CCControlEvent pEvent);
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);

    void onBtnRegMenu(CCObject* pSender);
    void onBtnBackMenu(CCObject* pSender);
    void onBtnCloseMenu(CCObject* pSender);

    // CCEditBoxDelegate
    virtual void editBoxEditingDidBegin(CCEditBox* editBox) {}
    virtual void editBoxEditingDidEnd(CCEditBox* editBox) {}
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text) {}
    virtual void editBoxReturn(CCEditBox* editBox) {}
};

#endif // _CREGISTER_VIEW_H_
