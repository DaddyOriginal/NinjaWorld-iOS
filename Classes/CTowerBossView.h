#ifndef __C_TOWER_BOSS_VIEW_H__
#define __C_TOWER_BOSS_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CTowerMgr.h"
#include "CTowerTableMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CTowerBossView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    int m_chapterId;
    int m_roundId;
    int m_floorSeq;

    // TowerBossView.ccbi members
    CCLabelTTF* m_pLabelBossName;
    CCLabelTTF* m_pLabelTalkWords;
    CCLabelTTF* m_pLabelExp;
    CCLabelTTF* m_pLabelSilver;
    CCControlButton* m_pBtnFightBoss;
    CCControlButton* m_pBtnGiveup;
    CCNode* m_pNodeIcon;

    void updateBossUI();
    void onBattleNotificationReceived(CCObject* pObj);

public:
    CTowerBossView();
    virtual ~CTowerBossView();

    static CTowerBossView* createWithFloor(int chapterId, int roundId);
    bool initWithFloor(int chapterId, int roundId);
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onFightClicked(CCObject* pSender, CCControlEvent pEvent);
    void onGiveupClicked(CCObject* pSender, CCControlEvent pEvent);
};

#endif // __C_TOWER_BOSS_VIEW_H__
