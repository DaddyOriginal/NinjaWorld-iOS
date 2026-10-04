#ifndef __C_ROULETTE_VIEW_H__
#define __C_ROULETTE_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CRouletteMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CRouletteView: Giao diện chính Vòng Quay May Mắn (RouletteView.ccbi)
 * 12 ô phần thưởng, Jackpot hũ vàng, hiệu ứng quay ô đèn LED và nhận quà 1x / 10x.
 */
class CRouletteView
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCLabelTTF* m_pLabelGoldPool;
    CCLabelTTF* m_pLabelScore;
    CCLabelTTF* m_pLabelFree;
    CCLabelTTF* m_pLabelGoldOnce;
    CCLabelBMFont* m_pLabelGold;
    CCLabelBMFont* m_pLabelSilver;
    CCLabelTTF* m_pLabelEndDesc;

    CCControlButton* m_pBtnRollOnce;
    CCControlButton* m_pBtnRollTentimes;
    CCControlButton* m_pBtnRank;
    CCControlButton* m_pBtnBack;

    CCControlButton* m_pBtnCard[12];
    CCSprite* m_pSprIcon[12];
    CCSprite* m_pSprHL[12];
    CCLabelTTF* m_pLabelNum[12];

    bool m_isSpinning;
    int m_targetSlot;
    int m_currentHLSlot;
    int m_spinStepsLeft;
    bool m_isTenSpinPending;

    void updateUI();
    void startSpinAnimation(int targetSlot, bool isTen);
    void updateSpinStep(float dt);
    void highlightSlot(int slotIndex);

public:
    CRouletteView();
    virtual ~CRouletteView();

    static CRouletteView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void Show(CCNode* pParent, int zOrder = 50);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onClickRollOnce(CCObject* pSender, CCControlEvent pEvent);
    void onClickRollTentimes(CCObject* pSender, CCControlEvent pEvent);
    void onClickRank(CCObject* pSender, CCControlEvent pEvent);
    void onClickBack(CCObject* pSender, CCControlEvent pEvent);

    void onWheelInfoUpdated(CCObject* pObj);
    void onWheelSpinResult(CCObject* pObj);
};

#endif // __C_ROULETTE_VIEW_H__
