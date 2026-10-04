#ifndef __C_ROULETTE_TURN_DIALOG_VIEW_H__
#define __C_ROULETTE_TURN_DIALOG_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CRouletteMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CRouletteTurnDialogView: Popup hiển thị kết quả 10 lần quay Vòng Quay May Mắn (RouletteTurnDialogView.ccbi)
 */
class CRouletteTurnDialogView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    std::vector<RouletteSpinItem> m_items;

    CCControlButton* m_pBtnClose;
    CCLabelTTF* m_pLabelTitle;

    CCSprite* m_pSprIcon[10];
    CCLabelTTF* m_pLabelName[10];
    CCLabelTTF* m_pLabelCount[10];
    CCControlButton* m_pBtnCard[10];

    void updateUI();

public:
    CRouletteTurnDialogView();
    virtual ~CRouletteTurnDialogView();

    static CRouletteTurnDialogView* createWithItems(const std::vector<RouletteSpinItem>& items);
    bool initWithItems(const std::vector<RouletteSpinItem>& items);

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void Show(CCNode* pParent, int zOrder = 100);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
};

#endif // __C_ROULETTE_TURN_DIALOG_VIEW_H__
