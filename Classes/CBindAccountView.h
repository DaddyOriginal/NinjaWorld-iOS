#ifndef _CBIND_ACCOUNT_VIEW_H_
#define _CBIND_ACCOUNT_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

class CLoginScene;

/**
 * CBindAccountView: Màn hình Đăng Nhập Tài Khoản nguyên bản từ BindAccountView.ccbi
 * Cho phép người chơi nhập Username & Mật khẩu để đăng nhập hoặc chuyển sang Đăng ký
 */
class CBindAccountView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCEditBoxDelegate
{
private:
    CLoginScene* m_pLoginScene;

    // CCB Variable Binders từ BindAccountView.ccbi
    CCNode* m_pFixNode;
    CCNode* m_pSpriteName;
    CCNode* m_pSpritePwd;
    CCControlButton* m_pBtnLogin;
    CCControlButton* m_pBtnRegist;
    CCControlButton* m_pBtnClose;
    CCLabelTTF* m_pLabelTitle;
    CCLabelTTF* m_pLabelTip;

    // EditBox nhập liệu
    CCEditBox* m_pEditUser;
    CCEditBox* m_pEditPwd;

public:
    CBindAccountView();
    virtual ~CBindAccountView();

    static CBindAccountView* create(CLoginScene* pScene);
    bool init(CLoginScene* pScene);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnLogin(CCObject* pSender, CCControlEvent pEvent);
    void onBtnRegist(CCObject* pSender, CCControlEvent pEvent);
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);

    void onBtnLoginMenu(CCObject* pSender);
    void onBtnRegistMenu(CCObject* pSender);
    void onBtnCloseMenu(CCObject* pSender);

    // CCEditBoxDelegate
    virtual void editBoxEditingDidBegin(CCEditBox* editBox) {}
    virtual void editBoxEditingDidEnd(CCEditBox* editBox) {}
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text) {}
    virtual void editBoxReturn(CCEditBox* editBox) {}
};

#endif // _CBIND_ACCOUNT_VIEW_H_
