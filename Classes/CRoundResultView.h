#ifndef __C_ROUND_RESULT_VIEW_H__
#define __C_ROUND_RESULT_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CChapterMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CRoundResultView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    PVEFightResult m_result;
    int m_chapterId;
    int m_stageId;

    // CCB member variables for RoundResultView.ccbi
    CCLabelTTF* m_pLabelMyName;
    CCLabelTTF* m_pLabelEnemyName;
    CCLabelTTF* m_pLabelMyAttack;
    CCLabelTTF* m_pLabelEnemyDefense;
    CCLabelTTF* m_pLabelExpVal;
    CCLabelTTF* m_pLabelSilverVal;
    CCLabelTTF* m_pLabelDropDesc;

    CCControlButton* m_pBtnContinue;
    CCControlButton* m_pBtnUpgrade;
    CCControlButton* m_pBtnGetCard;

    CCSprite* m_pSpriteResultType;
    CCSprite* m_pSpriteDot1;
    CCSprite* m_pSpriteDot2;
    CCSprite* m_pSpriteDot3;

    void updateResultUI();

public:
    CRoundResultView();
    virtual ~CRoundResultView();

    static CRoundResultView* createWithResult(const PVEFightResult& result, int chapterId, int stageId);
    bool initWithResult(const PVEFightResult& result, int chapterId, int stageId);

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // CCB Callbacks
    void onContinueClicked(CCObject* pSender, CCControlEvent pEvent);
    void onUpgradeClicked(CCObject* pSender, CCControlEvent pEvent);
    void onGetCardClicked(CCObject* pSender, CCControlEvent pEvent);
};

#endif // __C_ROUND_RESULT_VIEW_H__
