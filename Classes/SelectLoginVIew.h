#ifndef _SELECT_LOGIN_VIEW_H_
#define _SELECT_LOGIN_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

class CLoginScene;

class SelectLoginVIew 
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CLoginScene* m_pLoginDelegate;
    CCControlButton* m_pBtnLoginAccount;
    CCControlButton* m_pBtnLoginGuest;
    CCControlButton* m_pBtnLoginFB;
    CCControlButton* m_pBtnClose;

public:
    SelectLoginVIew();
    virtual ~SelectLoginVIew();

    static SelectLoginVIew* create(CLoginScene* pDelegate);
    virtual bool init(CLoginScene* pDelegate);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);

    // CCBSelectorResolver Callbacks
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);

    // CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Button Callbacks
    void onBtnLoginAccount(CCObject* pSender, CCControlEvent pEvent);
    void onBtnLoginGuest(CCObject* pSender, CCControlEvent pEvent);
    void onBtnLoginFB(CCObject* pSender, CCControlEvent pEvent);
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);

    void onBtnLoginAccountMenu(CCObject* pSender);
    void onBtnLoginGuestMenu(CCObject* pSender);
    void onBtnLoginFBMenu(CCObject* pSender);
    void onBtnCloseMenu(CCObject* pSender);
};

#endif // _SELECT_LOGIN_VIEW_H_
