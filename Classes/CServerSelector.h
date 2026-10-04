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

class CServerSelector : public CCLayerColor {
private:
    std::vector<ServerItemInfo> m_serverList;
    ServerSelectDelegate* m_pDelegate;
    CCMenu* m_pServerMenu;

public:
    CServerSelector();
    virtual ~CServerSelector();

    static CServerSelector* create(ServerSelectDelegate* pDelegate);
    virtual bool init(ServerSelectDelegate* pDelegate);

    void setServerList(const std::vector<ServerItemInfo>& list);
    void refreshUI();

    void onBtnSelectServer(CCObject* pSender);
    void onBtnClose(CCObject* pSender);
};

#endif // _CSERVER_SELECTOR_H_
