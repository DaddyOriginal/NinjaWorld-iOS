#ifndef __C_FRIEND_VIEW_H__
#define __C_FRIEND_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CFriendMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CFriendView: Giao diện Bạn Bè (FriendListView.ccbi & FriendMainView.ccbi)
 */
class CFriendView
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CCNode* m_pNodeTableContent;
    CCNode* m_pNodeCardContent;
    CCControlButton* m_pBtnMakeFriend;
    CCControlButton* m_pBtnMoreFriends;
    CCLabelTTF* m_pLabelTitle;

    CCScrollView* m_pScrollFriends;
    int m_selectedFriendUid;

    void buildFriendList();

public:
    CFriendView();
    virtual ~CFriendView();

    static CFriendView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    void Show(CCNode* pParent, int zOrder = 50);

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onClickMakeFriend(CCObject* pSender, CCControlEvent pEvent);
    void onClickMoreFriends(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBack(CCObject* pSender, CCControlEvent pEvent);

    void onFriendListUpdated(CCObject* pObj);
    void onFriendSearchUpdated(CCObject* pObj);
};

#endif // __C_FRIEND_VIEW_H__
