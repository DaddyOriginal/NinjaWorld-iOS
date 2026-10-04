#ifndef __C_DAILY_TASK_VIEW_H__
#define __C_DAILY_TASK_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CDailyTaskMgr.h"
#include "CDailyTaskTableMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CDailyTaskView;

/**
 * CDailyTaskCell: View cho 1 dòng nhiệm vụ hàng ngày (DailyTaskCell.ccbi)
 */
class CDailyTaskCell 
    : public CCNode
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner 
{
private:
    DailyTaskItem m_item;
    CDailyTaskView* m_pParentView;

    CCSprite* m_pSprTaskIcon;
    CCLabelTTF* m_pLabelName;
    CCLabelTTF* m_pLabelProgress;
    CCLabelTTF* m_pLabelDesc;
    CCLabelTTF* m_pLabelScore;
    CCControlButton* m_pBtnGoto;
    CCSprite* m_pSprTaskDone;

public:
    CDailyTaskCell();
    virtual ~CDailyTaskCell();

    static CDailyTaskCell* create(const DailyTaskItem& item, CDailyTaskView* pParentView);
    bool init(const DailyTaskItem& item, CDailyTaskView* pParentView);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    void onBtnGotoClicked(CCObject* pSender, CCControlEvent pEvent);
    int getTaskId() const { return m_item.id; }
};

/**
 * CDailyTaskAwardCell: View cho 1 ô rương mốc năng động (DailyTaskAwardCell.ccbi)
 */
class CDailyTaskAwardCell 
    : public CCNode
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner 
{
private:
    DailyTaskAwardBox m_box;
    CDailyTaskView* m_pParentView;

    CCSprite* m_pSprBox;
    CCLabelTTF* m_pLabelNeedScore;

public:
    CDailyTaskAwardCell();
    virtual ~CDailyTaskAwardCell();

    static CDailyTaskAwardCell* create(const DailyTaskAwardBox& box, CDailyTaskView* pParentView);
    bool init(const DailyTaskAwardBox& box, CDailyTaskView* pParentView);

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    void onBoxClicked(CCObject* pSender, CCControlEvent pEvent);
    int getBoxId() const { return m_box.id; }
    const DailyTaskAwardBox& getBoxData() const { return m_box; }
    CCSprite* getBoxSprite() const { return m_pSprBox; }
};

/**
 * CDailyTaskView: Giao diện chính Nhiệm Vụ Hàng Ngày & Rương Năng Động (DailyTaskView.ccbi)
 */
class CDailyTaskView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCLabelTTF* m_pLabelCurScore;
    CCLabelTTF* m_pLabelTitle;
    CCNode* m_pNodeContent;
    CCNode* m_pNodeCell;
    CCNode* m_pNodeAwardContent;
    CCNode* m_pNodeAwardCell;
    CCControlButton* m_pBtnClose;
    CCControlButton* m_pBtnPreAward;
    CCControlButton* m_pBtnNextAward;
    CCNode* m_pSprProgress;

    CCScrollView* m_pScrollTasks;
    CCScrollView* m_pScrollAwards;
    std::vector<CDailyTaskAwardCell*> m_awardCells;
    int m_curAwardIndex;

    void buildAwardBoxList();
    void buildTaskList();
    void updateScoreHeader();

public:
    CDailyTaskView();
    virtual ~CDailyTaskView();

    static CDailyTaskView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void Show(CCNode* pParent, int zOrder = 60);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void onBtnPreAward(CCObject* pSender, CCControlEvent pEvent);
    void onBtnNextAward(CCObject* pSender, CCControlEvent pEvent);

    void onDailyTasksUpdated(CCObject* pObj);
    void onDailyBoxClaimed(CCObject* pObj);

    void onGotoTask(int taskId);
    void onOpenAwardBox(int boxId);
};

#endif // __C_DAILY_TASK_VIEW_H__
