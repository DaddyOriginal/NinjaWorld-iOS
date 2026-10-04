#ifndef __C_DAILY_REWARD_VIEW_H__
#define __C_DAILY_REWARD_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CDailyTaskMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CDailyRewardView: Popup chi tiết phần thưởng Rương Năng Động (DailyRewardView.ccbi)
 * Hiển thị các vật phẩm nhận được (Bạc, Vàng, Ramen, Hoán Cốt Đan) và nút Nhận Thưởng
 */
class CDailyRewardView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    DailyTaskAwardBox m_boxData;

    // Node & Controls từ DailyRewardView.ccbi
    CCLabelTTF* m_pLabelTitle;
    CCSprite* m_pSprGot;
    CCControlButton* m_pBtnClose;
    CCControlButton* m_pBtnGetAward;

    CCSprite* m_pSprGiftIcon[4];
    CCSprite* m_pSprPiece[4];
    CCLabelTTF* m_pLabelItem[4];
    CCControlButton* m_pBtnAward[4];

public:
    CDailyRewardView();
    virtual ~CDailyRewardView();

    static CDailyRewardView* createWithBox(const DailyTaskAwardBox& box);
    bool initWithBox(const DailyTaskAwardBox& box);

    virtual void onEnter();
    virtual void onExit();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void Show(CCNode* pParent, int zOrder = 100);
    void updateUI();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void onBtnGetAward(CCObject* pSender, CCControlEvent pEvent);
    void onBtnAwardClicked(CCObject* pSender, CCControlEvent pEvent);
};

#endif // __C_DAILY_REWARD_VIEW_H__
