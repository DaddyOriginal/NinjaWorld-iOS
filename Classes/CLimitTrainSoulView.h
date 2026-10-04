#ifndef __C_LIMIT_TRAIN_SOUL_VIEW_H__
#define __C_LIMIT_TRAIN_SOUL_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CEightGateMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CLimitTrainSoulView: Giao diện Luyện Hồn (LimitTrainSoulView.ccbi)
 * Cho phép luyện Linh Hồn (thường bằng Bạc, cao cấp bằng Vàng), nhân bội số (x2, x4, x8) và thu thập Soul.
 */
class CLimitTrainSoulView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCLabelTTF* m_pLabelCurSoul;
    CCLabelTTF* m_pLabelTrainSoul;
    CCLabelTTF* m_pLabelTrainSoulAll;
    CCLabelTTF* m_pLabelMultiple;

    CCLabelBMFont* m_pLabelNormalCostSilver;
    CCLabelBMFont* m_pLabelSpecialCostGold;

    CCLabelTTF* m_pLabelCostSilver;
    CCLabelTTF* m_pLabelCostGold;
    CCLabelTTF* m_pLabelCostDescNormal;
    CCLabelTTF* m_pLabelCostDescSpecial;
    CCLabelTTF* m_pLabelFreeMutiTimes;
    CCLabelTTF* m_pLabelCanGetSoulNormal;
    CCLabelTTF* m_pLabelCanGetSoulSpecial;

    CCLabelBMFont* m_pLabelGold;
    CCLabelBMFont* m_pLabelSilver;

    CCControlButton* m_pBtnBack;
    CCControlButton* m_pBtnGet;
    CCControlButton* m_pBtnGoldMuti;
    CCControlButton* m_pBtnSilverMuti;
    CCControlButton* m_pBtnTrainNormal;
    CCControlButton* m_pBtnTrainSpecial;

    CCNode* m_pNodePreTrain;
    CCNode* m_pNodeAfterTrain;
    CCSprite* m_pSprGoldIcon;

    void updateUI();

public:
    CLimitTrainSoulView();
    virtual ~CLimitTrainSoulView();

    static CLimitTrainSoulView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

    void Show(CCNode* pParent, int zOrder = 80);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnBack(CCObject* pSender, CCControlEvent pEvent);
    void onBtnTrainNormal(CCObject* pSender, CCControlEvent pEvent);
    void onBtnTrainSpecial(CCObject* pSender, CCControlEvent pEvent);
    void onBtnSilverMuti(CCObject* pSender, CCControlEvent pEvent);
    void onBtnGoldMuti(CCObject* pSender, CCControlEvent pEvent);
    void onBtnGet(CCObject* pSender, CCControlEvent pEvent);

    void onTrainSoulUpdated(CCObject* pObj);
};

#endif // __C_LIMIT_TRAIN_SOUL_VIEW_H__
