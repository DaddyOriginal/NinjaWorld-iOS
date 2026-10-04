#ifndef __C_ROUND_TEAM_FIGHT_VIEW_H__
#define __C_ROUND_TEAM_FIGHT_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CStageTableMgr.h"
#include "CChapterMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CRoundTeamFightView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    int m_chapterId;
    int m_stageId;
    int m_roundId;
    bool m_isBoss;

    // CCB member variables for RoundTeamView.ccbi
    CCLabelTTF* m_pLabelEnemyName;
    CCLabelTTF* m_pLabelEnemyTeamCount;
    CCLabelTTF* m_pLabelMyTeamCount;
    CCNode* m_pNodeMyCardList;
    CCNode* m_pNodeEnemyCardList;
    CCControlButton* m_pBtnPass;

    void setupMyTeamDisplay();
    void setupEnemyTeamDisplay(const RoundTableEntry& round);
    void onBattleNotificationReceived(CCObject* pObj);

public:
    CRoundTeamFightView();
    virtual ~CRoundTeamFightView();

    static CRoundTeamFightView* createWithRound(int chapterId, int stageId, int roundId);
    bool initWithRound(int chapterId, int stageId, int roundId);
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onFightClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBackClicked(CCObject* pSender);
};

#endif // __C_ROUND_TEAM_FIGHT_VIEW_H__
