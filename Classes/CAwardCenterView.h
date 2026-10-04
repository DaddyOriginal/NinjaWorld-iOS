#ifndef _CAWARD_CENTER_VIEW_H_
#define _CAWARD_CENTER_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CAwardCenterMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CAwardCenterView;

/**
 * CAwardCenterCell: Cell hiển thị từng phần quà trong danh sách
 * Gắn kết với activity/AwardCenterCell.ccbi hoặc AwardCenterCell.ccbi
 */
class CAwardCenterCell
    : public CCTableViewCell
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CAwardCenterView* m_pParentView;
    SAwardMsg m_msg;

    CCNode* m_pNodeContent;
    CCNode* m_pNodeCell;
    CCLabelTTF* m_pLabelItemTitle;
    CCLabelTTF* m_pLabelAwardTime;
    CCLabelTTF* m_pLabelDesc;
    CCControlButton* m_pBtnGet;

public:
    CAwardCenterCell();
    virtual ~CAwardCenterCell();

    static CAwardCenterCell* create(const SAwardMsg& msg, CAwardCenterView* pParentView);
    bool init(const SAwardMsg& msg, CAwardCenterView* pParentView);

    void setMsgData(const SAwardMsg& msg);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    void onBtnGet(CCObject* pSender, CCControlEvent pEvent);
};

/**
 * CAwardCenterView: Hộp thoại Trung Tâm Trao Thưởng
 * Gắn kết với activity/AwardCenterView.ccbi hoặc AwardCenterView.ccbi
 */
class CAwardCenterView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
private:
    CCNode* m_pNodeContent;
    CCNode* m_pNodeCell;
    CCLabelTTF* m_pLabelTitle;
    CCLabelTTF* m_pLabelExpiry;
    CCControlButton* m_pBtnClose;
    CCControlButton* m_pBtnGetAll;

    CCTableView* m_pTableView;
    CCSize m_cellSize;

public:
    CAwardCenterView();
    virtual ~CAwardCenterView();

    CREATE_FUNC(CAwardCenterView);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void Show(CCNode* pParent, int zOrder = 50);
    void refreshView();

    // TableView Callbacks
    virtual CCSize cellSizeForTable(CCTableView* table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView* table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView* table);
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view) {}
    virtual void scrollViewDidZoom(CCScrollView* view) {}

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Nút bấm
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void onBtnGetAll(CCObject* pSender, CCControlEvent pEvent);
    void dialogClose(CCObject* pSender);

    void onAwardListLoaded(CCObject* pData);
    void onClaimSuccess(CCObject* pData);

    // Nuốt chạm nền
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
};

#endif // _CAWARD_CENTER_VIEW_H_
