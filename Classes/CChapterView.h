#ifndef __C_CHAPTER_VIEW_H__
#define __C_CHAPTER_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CStageTableMgr.h"
#include "CChapterMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CChapterView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    CCNode* m_pNodeSectionContainer;
    CCControlButton* m_pPageBtn1;
    CCControlButton* m_pPageBtn2;
    CCControlButton* m_pPageBtn3;
    CCControlButton* m_pPageBtn4;

    CCLabelTTF* m_pLabelMaxAttack;
    CCLabelTTF* m_pLabelMaxDefense;
    CCLabelTTF* m_pLabelMaxHonor;

    int m_currentPage; // 1..4
    CCNode* m_pCurrentPageNode;

    void updatePageDisplay(int page);
    void setupChapterContainer(CCNode* pageNode, int slotIndex, int chapterId);
    void onChapterClicked(CCObject* pSender);
    void updateCombatPowerLabels();

public:
    CChapterView();
    virtual ~CChapterView();

    static CChapterView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onClickPageBtn1(CCObject* pSender, CCControlEvent pEvent);
    void onClickPageBtn2(CCObject* pSender, CCControlEvent pEvent);
    void onClickPageBtn3(CCObject* pSender, CCControlEvent pEvent);
    void onClickPageBtn4(CCObject* pSender, CCControlEvent pEvent);

    void refreshView();
};

#endif // __C_CHAPTER_VIEW_H__
