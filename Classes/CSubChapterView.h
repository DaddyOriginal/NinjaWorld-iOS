#ifndef __C_SUB_CHAPTER_VIEW_H__
#define __C_SUB_CHAPTER_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CStageTableMgr.h"
#include "CChapterMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CSubChapterView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    int m_chapterId;
    int m_selectedStageId;
    int m_selectedRoundIndex;

    // SubChapterListView.ccbi members
    CCControlButton* m_pBtnBack;
    CCNode* m_pNodeBtnListContainer;
    CCNode* m_pNodeSectionContainer;
    CCNode* m_pNodeCardContent;

    // SubChapter.ccbi (loaded inside m_pNodeSectionContainer)
    CCNode* m_pSubChapterDetailNode;
    CCLabelTTF* m_pLabelTitle;
    CCLabelTTF* m_pLabelSectionNo;
    CCLabelBMFont* m_pLabelNeedEnergy;
    CCLabelBMFont* m_pLabelExp;
    CCLabelBMFont* m_pLabelSilver;
    CCLabelTTF* m_pLabelDesc;
    CCNode* m_pSectionNormalContainer;
    CCNode* m_pSectionBossContainer;
    CCNode* m_pNodeAwardContainer;

    std::vector<int> m_stages;
    std::vector<RoundTableEntry> m_roundsInStage;

    void buildStageTabs();
    void loadStageDetail(int stageId);
    void updateDetailUI(const RoundTableEntry& round);

public:
    CSubChapterView();
    virtual ~CSubChapterView();

    static CSubChapterView* createWithChapter(int chapterId);
    bool initWithChapter(int chapterId);
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onClickBack(CCObject* pSender, CCControlEvent pEvent);
    void onSelectStageTab(CCObject* pSender);
    void onStartBattleClicked(CCObject* pSender);

    void refreshView();
};

#endif // __C_SUB_CHAPTER_VIEW_H__
