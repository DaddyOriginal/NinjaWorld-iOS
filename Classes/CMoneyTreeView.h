#ifndef _CMONEY_TREE_VIEW_H_
#define _CMONEY_TREE_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CMoneyTreeMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CMoneyTreeView: Giao diện Cây Rung Tiền
 * Gắn kết với MoneyTreeView.ccbi
 */
class CMoneyTreeView
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCNode* m_pNodeWidth;
    CCNode* m_pNodeTips;
    CCNode* m_pNodeAnim;
    CCNode* m_pNodeContent;
    CCNode* m_pNodeCell;

    CCLabelTTF* m_pLabelGoldVal;
    CCLabelTTF* m_pLabelSilverVal;
    CCLabelTTF* m_pLabelTimes;
    CCLabelTTF* m_pLabelFreeTry;
    CCLabelTTF* m_pLabelTry;
    CCLabelTTF* m_pLabelGetSilver;
    CCLabelTTF* m_pLabelCostGold;
    CCLabelTTF* m_pLabelDesc;

    CCLabelTTF* m_pLabelMoneyTreeView0;
    CCLabelTTF* m_pLabelMoneyTreeView1;
    CCLabelTTF* m_pLabelMoneyTreeView2;
    CCLabelTTF* m_pLabelMoneyTreeView3;
    CCLabelTTF* m_pLabelMoneyTreeView4;

    bool m_bIsSwinging;
    double m_lastShakeTime;

public:
    CMoneyTreeView();
    virtual ~CMoneyTreeView();

    CREATE_FUNC(CMoneyTreeView);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void refreshView();
    void RunSwingTreeAnim();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnBack(CCObject* pSender, CCControlEvent pEvent);
    void onBtnSwing(CCObject* pSender, CCControlEvent pEvent);
    void onTreeDataLoaded(CCObject* pData);
    void onSwingSuccess(CCObject* pData);

    // Cảm biến rung lắc thiết bị
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
};

#endif // _CMONEY_TREE_VIEW_H_
