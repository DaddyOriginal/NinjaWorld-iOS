#ifndef __C_PLAYER_ARENA_VIEW_H__
#define __C_PLAYER_ARENA_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CRLRequest.h"
#include "CNinjaTableMgr.h"
#include "CPlayerDataMgr.h"
#include <string>
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

struct ArenaOpponentData {
    int playerId;
    std::string nickName;
    int rank;
    int level;
    int leaderNinjaId;
    int rewardCoin;
    int rewardRenown;
    bool isMe;
};

class CPlayerArenaView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    int m_myRank;
    int m_myRenown;
    int m_remainingFights;
    int m_maxFights;
    int m_cdRemaining;

    std::vector<ArenaOpponentData> m_opponents;

    // CCB members for ArenaView.ccbi
    CCNode* m_pNodeTableContent;
    CCNode* m_pNodeCardContent;
    CCControlButton* m_pBtnRank;
    CCControlButton* m_pBtnChest;
    CCControlButton* m_pBtnMore;

    CCLabelTTF* m_pLabelRank;
    CCLabelTTF* m_pLabelRemainTimes;
    CCLabelTTF* m_pLabelRestTime;
    CCLabelTTF* m_pLabelAwardType;

    void requestArenaInfo();
    void onArenaInfoResp(CRLRequest* pRequest);
    void buildOpponentList();
    void setupOpponentCell(CCNode* cellNode, const ArenaOpponentData& opp, float posY);

    void sendChallengeRequest(int targetRank);
    void onChallengeResp(CRLRequest* pRequest);

    void showRuleDialog();
    void showRewardDialog();
    void showRankListDialog();

public:
    CPlayerArenaView();
    virtual ~CPlayerArenaView();

    static CPlayerArenaView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // CCB Callbacks
    void onClickRankList(CCObject* pSender, CCControlEvent pEvent);
    void onClickChest(CCObject* pSender, CCControlEvent pEvent);
    void onClickAwardRule(CCObject* pSender, CCControlEvent pEvent);

    void onFightClicked(CCObject* pSender);

    void refreshView();
};

#endif // __C_PLAYER_ARENA_VIEW_H__
