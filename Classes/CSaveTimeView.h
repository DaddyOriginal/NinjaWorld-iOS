#ifndef __C_SAVE_TIME_VIEW_H__
#define __C_SAVE_TIME_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CSaveTimeMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CSaveTimeView: Giao diện Tiết Kiệm Thời Gian / Đổi Mì Ramen (SaveTimeView.ccbi)
 */
class CSaveTimeView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCLabelBMFont* m_pLabelRamenNum;
    CCLabelBMFont* m_pLabelExp;
    CCLabelBMFont* m_pLabelSilver;
    CCLabelBMFont* m_pLabelExchangeNum;

    CCLabelTTF* m_pLabelCurChapter;
    CCLabelTTF* m_pLabelCost;
    CCLabelTTF* m_pLabelTip;
    CCLabelTTF* m_pLabelAddDesc;
    CCLabelTTF* m_pLabelMaxVip;

    CCControlButton* m_pBtnClose;
    CCControlButton* m_pBtnBuy;
    CCControlButton* m_pBtnExchange;

    void updateUI();

public:
    CSaveTimeView();
    virtual ~CSaveTimeView();

    static CSaveTimeView* create();
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
    void onBtnExchange(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBuy(CCObject* pSender, CCControlEvent pEvent);

    void onSaveTimeUpdated(CCObject* pObj);
};

#endif // __C_SAVE_TIME_VIEW_H__
