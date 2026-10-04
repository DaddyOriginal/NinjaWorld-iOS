#ifndef _CGROWTH_FUND_VIEW_H_
#define _CGROWTH_FUND_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CGrowthFundMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CGrowthFundView;

/**
 * CGrowthFundCell: Cell hiển thị từng mốc thưởng cấp độ
 * Gắn kết với activity/growth_fund_cell.ccbi hoặc growth_fund_cell.ccbi
 */
class CGrowthFundCell
    : public CCTableViewCell
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CGrowthFundView* m_pParentView;
    SGrowthFundItem m_item;

    CCNode* m_pNodePropsIcon;
    CCLabelBMFont* m_pLabelPropsCount;
    CCSprite* m_pSpriteHasGet;
    CCNode* m_pSpriteBtnGetFund;
    CCLabelTTF* m_pLabelFundTitle;
    CCLabelTTF* m_pLabelPropsDesc;
    CCLabelTTF* m_pLabelGrowthFundCell0;

public:
    CGrowthFundCell();
    virtual ~CGrowthFundCell();

    static CGrowthFundCell* create(const SGrowthFundItem& item, CGrowthFundView* pParentView);
    bool init(const SGrowthFundItem& item, CGrowthFundView* pParentView);

    void setItemData(const SGrowthFundItem& item);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    void onBtnGetFund(CCObject* pSender, CCControlEvent pEvent);
    void onClickCell();
};

/**
 * CGrowthFundView: Giao diện Quỹ Trưởng Thành
 * Gắn kết với activity/growth_fund.ccbi hoặc growth_fund.ccbi
 */
class CGrowthFundView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCTableViewDataSource
    , public CCTableViewDelegate
{
private:
    CCNode* m_pNodeGiftCellNode;
    CCNode* m_pNodeTableContent;
    CCControlButton* m_pBtnCharge;
    CCControlButton* m_pBtnBuyFund;

    CCLabelTTF* m_pLabelFundDesc1;
    CCLabelTTF* m_pLabelFundDesc2;
    CCLabelTTF* m_pLabelFundDesc3;
    CCLabelTTF* m_pLabelFundDesc4;
    CCLabelTTF* m_pLabelFundDesc5;

    CCLabelBMFont* m_pLabelVal1;
    CCLabelBMFont* m_pLabelVal2;
    CCLabelBMFont* m_pLabelVal3;

    CCLabelTTF* m_pLabelGrowthFund0;
    CCLabelTTF* m_pLabelGrowthFund1;

    CCTableView* m_pTableView;
    CCSize m_cellSize;

public:
    CGrowthFundView();
    virtual ~CGrowthFundView();

    CREATE_FUNC(CGrowthFundView);

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

    // Callbacks nút
    void onBtnBuyFund(CCObject* pSender, CCControlEvent pEvent);
    void onBtnCharge(CCObject* pSender, CCControlEvent pEvent);
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void dialogClose(CCObject* pSender);

    void onFundDataLoaded(CCObject* pData);

    // Nuốt chạm nền
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
};

#endif // _CGROWTH_FUND_VIEW_H_
