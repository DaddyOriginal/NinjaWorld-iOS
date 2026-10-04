#ifndef __C_TOWER_LEVEL_VIEW_H__
#define __C_TOWER_LEVEL_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CTowerMgr.h"
#include "CTowerTableMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CTowerLevelView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    int m_chapterId;

    // TowerLevelView.ccbi members
    CCControlButton* m_pBtnBack;
    CCLabelTTF* m_pLabelLevelInfo;

    CCControlButton* m_pBtnStages[7];
    CCLabelTTF* m_pLabelNames[7];
    CCSprite* m_pSpriteBeats[7];
    CCNode* m_pNodeIcons[7];

    void updateStagesDisplay();
    void onStageClicked(int stageIndex);

public:
    CTowerLevelView();
    virtual ~CTowerLevelView();

    static CTowerLevelView* createWithChapter(int chapterId);
    bool initWithChapter(int chapterId);
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBackClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick01(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick02(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick03(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick04(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick05(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick06(CCObject* pSender, CCControlEvent pEvent);
    void onBtnIconClick07(CCObject* pSender, CCControlEvent pEvent);
};

#endif // __C_TOWER_LEVEL_VIEW_H__
