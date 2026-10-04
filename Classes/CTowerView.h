#ifndef __C_TOWER_VIEW_H__
#define __C_TOWER_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CTowerMgr.h"
#include "CTowerTableMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CTowerView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    CCControlButton* m_pBtnReset;
    CCControlButton* m_pBtnRank;
    CCControlButton* m_pBtnSweep;
    CCControlButton* m_pBtnBack;
    CCLabelTTF* m_pLabelLeftTimes;
    CCScrollView* m_pScrollTower;
    CCNode* m_pNodeFight;

    CCNode* m_pChapterListContainer;

    void buildTowerChapterList();
    void onChapterClicked(CCObject* pSender);
    void updateTowerHUD();

public:
    CTowerView();
    virtual ~CTowerView();

    static CTowerView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnResetClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnRankClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnSweepClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBackClicked(CCObject* pSender, CCControlEvent pEvent);

    void refreshView();
};

#endif // __C_TOWER_VIEW_H__
