#ifndef __C_EIGHT_GATE_VIEW_H__
#define __C_EIGHT_GATE_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CEightGateMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CEightGateView: Giao diện chính Bát Môn Độn Giáp (LimitTrainView.ccbi)
 * Quản lý 8 huyệt đạo (Khai, Hưu, Sinh, Thương, Đỗ, Cảnh, Kinh, Tử) và thuộc tính cộng dồn.
 */
class CEightGateView 
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCLabelTTF* m_pLabelCurSoul;
    CCLabelTTF* m_pLabelCostSoul;
    CCLabelTTF* m_pLabelCostSilver;
    CCLabelTTF* m_pLabelGateLv;

    CCLabelTTF* m_pLabelAddAtt;
    CCLabelTTF* m_pLabelAddDef;
    CCLabelTTF* m_pLabelAddChakra;
    CCLabelTTF* m_pLabelAddAttPer;
    CCLabelTTF* m_pLabelAddDefPer;
    CCLabelTTF* m_pLabelAddChakraPer;

    CCLabelBMFont* m_pLabelGold;
    CCLabelBMFont* m_pLabelSilver;

    CCControlButton* m_pBtnBack;
    CCControlButton* m_pBtnLimitTrain;
    CCControlButton* m_pBtnOpen;

    CCSprite* m_pSprGate;
    CCSprite* m_pSprGateNum[8];
    CCSprite* m_pSprLine;

    CCNode* m_pNodeAnimContainer;
    CCNode* m_pNodeContent;
    CCNode* m_pNodeCell;

    void updateUI();

public:
    CEightGateView();
    virtual ~CEightGateView();

    static CEightGateView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void Show(CCNode* pParent, int zOrder = 50);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnBack(CCObject* pSender, CCControlEvent pEvent);
    void onBtnLimitTrain(CCObject* pSender, CCControlEvent pEvent);
    void onBtnOpen(CCObject* pSender, CCControlEvent pEvent);

    void onGateUpdated(CCObject* pObj);
    void onGateOpened(CCObject* pObj);
};

#endif // __C_EIGHT_GATE_VIEW_H__
