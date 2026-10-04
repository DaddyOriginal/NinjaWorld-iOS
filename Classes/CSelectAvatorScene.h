#ifndef _CSELECT_AVATOR_SCENE_H_
#define _CSELECT_AVATOR_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CSelectAvatorScene: Màn hình chọn Quốc gia khởi đầu (SelectorCountryAvatar.ccbi)
 * Cho phép người chơi lựa chọn 1 trong 5 Quốc gia / Làng lớn của Thế giới Ninja:
 * 1: Hỏa Quốc (Làng Lá - Konoha)
 * 2: Thủy Quốc (Làng Sương Mù - Kiri)
 * 3: Phong Quốc (Làng Cát - Suna)
 * 4: Thổ Quốc (Làng Đá - Iwa)
 * 5: Lôi Quốc (Làng Mây - Kumo)
 */
class CSelectAvatorScene : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    int m_selectedCountry; // 1: Fire, 2: Water, 3: Wind, 4: Earth, 5: Mine

    CCControlButton* m_pBtnNext;
    CCNode* m_pCountryNode;

    CCSprite* m_pIconMapNow;
    CCSprite* m_pIconNow;
    CCSprite* m_pIconMapPre;
    CCSprite* m_pIconMapNext;

    CCSprite* m_pIcons[5];
    CCSprite* m_pIconMaps[5];

    CCSprite* m_pCountryEmblem;
    CCSprite* m_pCountryMap;
    CCMenuItemFont* m_pCountryBtnItems[5];

    CCLabelTTF* m_pLabelCountryName;
    CCLabelTTF* m_pLabelCountryDesc;

public:
    CSelectAvatorScene();
    virtual ~CSelectAvatorScene();

    static CCScene* scene();
    CREATE_FUNC(CSelectAvatorScene);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void updateCountryDisplay();
    void selectCountry(int countryIndex);
    void nextCountry();
    void prevCountry();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnNext(CCObject* pSender, CCControlEvent pEvent);
    void onBtnNextMenu(CCObject* pSender);
    void onBtnNextCountry(CCObject* pSender, CCControlEvent pEvent);
    void onBtnNextCountryMenu(CCObject* pSender);
    void onBtnPreCountry(CCObject* pSender, CCControlEvent pEvent);
    void onBtnPreCountryMenu(CCObject* pSender);
    void onSelectCountryTab(CCObject* pSender);

    // Touch
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

private:
    CCPoint m_touchStart;
};

#endif // _CSELECT_AVATOR_SCENE_H_
