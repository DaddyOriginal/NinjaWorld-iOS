#ifndef _CSERVER_LIST_MGR_H_
#define _CSERVER_LIST_MGR_H_

#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;

/**
 * Cấu trúc thông tin một Server trong cụm máy chủ đa Server (Multi-Server)
 */
struct ServerInfoData {
    int id;              // ID máy chủ (1, 2, 3...)
    std::string name;    // Tên hiển thị ("S1 - Làng Lá", "S2 - Làng Cát"...)
    std::string domain;  // URL kết nối ("http://160.22.123.62:8088")
    int port;            // Cổng (8088)
    int state;           // 1: Mượt/Tốt, 2: Đông, 3: Đầy, 4: Bảo trì
    int recommend;       // 1000 / 1: Server đề cử cho người chơi mới
    int groupId;         // Cụm server

    ServerInfoData()
        : id(1)
        , name("S1 - Làng Lá")
        , domain("http://160.22.123.62:8088")
        , port(8088)
        , state(1)
        , recommend(1)
        , groupId(1)
    {}
};

/**
 * CServerListMgr: Quản lý danh sách cụm máy chủ động từ API /xk_r_dir
 */
class CServerListMgr : public CCObject {
private:
    static CServerListMgr* s_instance;

    std::vector<ServerInfoData> m_serverList;
    ServerInfoData m_selectedServer;
    std::string m_gatewayUrl;

    CServerListMgr();
    virtual ~CServerListMgr();

public:
    static CServerListMgr* sharedManager();
    static void purge();

    // Nạp & phân tích danh sách Server từ phản hồi XML của /xk_r_dir
    bool parseServerListXml(const std::string& xmlData);

    // Quản lý danh sách Server
    const std::vector<ServerInfoData>& getServerList() const { return m_serverList; }
    void addServer(const ServerInfoData& server);
    void clearServerList();

    // Server đang được chọn hiện tại
    const ServerInfoData& getSelectConfig() const { return m_selectedServer; }
    void setSelectConfig(const ServerInfoData& server);
    bool setSelectConfigById(int serverId);

    // Lấy Server đề cử (recommend) hoặc Server mới nhất
    ServerInfoData getRecommendServer() const;

    // Lưu & Nạp Server người chơi chọn lần gần nhất từ bộ nhớ cache
    void saveCacheServer();
    void loadCacheServer();

    // Gateway URL
    void setGatewayUrl(const std::string& url) { m_gatewayUrl = url; }
    const std::string& getGatewayUrl() const { return m_gatewayUrl; }
};

#endif // _CSERVER_LIST_MGR_H_
