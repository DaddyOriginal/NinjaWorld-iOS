#ifndef __C_MAIL_VIEW_H__
#define __C_MAIL_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CMailMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CMailView: Giao diện Hòm Thư & Thông Báo Hệ Thống (CommonSystemMsgView.ccbi)
 */
class CMailView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCLabelTTF* m_pLabelTitle;
    CCLabelTTF* m_pLabelDescription;
    CCControlButton* m_pBtnClose;
    CCLabelTTF* m_pLabelLeft;
    CCLabelTTF* m_pLabelRight;

    CCScrollView* m_pScrollMails;
    int m_selectedMailId;

    void buildMailList();

public:
    CMailView();
    virtual ~CMailView();

    static CMailView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void Show(CCNode* pParent, int zOrder = 70);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void onMailListUpdated(CCObject* pObj);
    void onMailClaimed(CCObject* pObj);
    void onSelectMail(int mailId);
};

#endif // __C_MAIL_VIEW_H__
