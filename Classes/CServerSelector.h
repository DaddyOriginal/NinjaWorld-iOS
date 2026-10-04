#ifndef _CSERVER_SELECTOR_H_
#define _CSERVER_SELECTOR_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

struct ServerItemInfo {
    int id;
    std::string name;
    std::string ip;
    int port;
    int state; // 1: Mới, 2: Tốt, 3: Đầy
};

class ServerSelectDelegate {
public:
    virtual void onServerSelected(int serverId, const std::string& serverName, const std::string& hostUrl) = 0;
};

class CServerSelector 
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    std::vector<ServerItemInfo> m_serverList;
    ServerSelectDelegate* m_pDelegate;

    CCLabelTTF* m_pLabelServerName1;
    CCLabelTTF* m_pLabelServerName2;
    CCSprite* m_pSpriteServerState1;
    CCSprite* m_pSpriteServerState2;
    CCNode* m_pNodeListContent;
    CCControlButton* m_pBtnClose;
    CCControlButton* m_pBtnServer1;
    CCControlButton* m_pBtnServer2;

public:
    CServerSelector();
    virtual ~CServerSelector();

    static CServerSelector* create(ServerSelectDelegate* pDelegate);
    virtual bool init(ServerSelectDelegate* pDelegate);

    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);

    void setServerList(const std::vector<ServerItemInfo>& list);
    void refreshUI();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void onBtnServer1(CCObject* pSender, CCControlEvent pEvent);
    void onBtnServer2(CCObject* pSender, CCControlEvent pEvent);

    void onBtnCloseMenu(CCObject* pSender);
    void onBtnServer1Menu(CCObject* pSender);
    void onBtnServer2Menu(CCObject* pSender);
};

#endif // _CSERVER_SELECTOR_H_
