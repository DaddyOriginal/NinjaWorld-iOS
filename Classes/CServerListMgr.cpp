#include "CServerListMgr.h"
#include <sstream>

CServerListMgr* CServerListMgr::s_instance = NULL;

CServerListMgr::CServerListMgr()
    : m_gatewayUrl("http://160.22.123.62:8088")
{
    m_selectedServer.id = 1;
    m_selectedServer.name = "S1 - Làng Lá";
    m_selectedServer.domain = "http://160.22.123.62:8088";
    m_selectedServer.port = 8088;
    m_selectedServer.state = 1;
    m_selectedServer.recommend = 1;
    loadCacheServer();
}

CServerListMgr::~CServerListMgr() {
}

CServerListMgr* CServerListMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CServerListMgr();
    }
    return s_instance;
}

void CServerListMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CServerListMgr::clearServerList() {
    m_serverList.clear();
}

void CServerListMgr::addServer(const ServerInfoData& server) {
    m_serverList.push_back(server);
}

bool CServerListMgr::setSelectConfigById(int serverId) {
    for (size_t i = 0; i < m_serverList.size(); ++i) {
        if (m_serverList[i].id == serverId) {
            setSelectConfig(m_serverList[i]);
            return true;
        }
    }
    return false;
}

void CServerListMgr::setSelectConfig(const ServerInfoData& server) {
    m_selectedServer = server;
    saveCacheServer();
    CCLog("[CServerListMgr] Chon Server ID=%d, Name='%s', Domain='%s'", 
          m_selectedServer.id, m_selectedServer.name.c_str(), m_selectedServer.domain.c_str());
}

ServerInfoData CServerListMgr::getRecommendServer() const {
    if (m_serverList.empty()) {
        return ServerInfoData();
    }

    // 1. Tìm server có cờ recommend cao nhất
    for (size_t i = 0; i < m_serverList.size(); ++i) {
        if (m_serverList[i].recommend > 0) {
            return m_serverList[i];
        }
    }

    // 2. Fallback: Lấy server mới nhất (cuối danh sách)
    return m_serverList.back();
}

void CServerListMgr::saveCacheServer() {
    CCUserDefault::sharedUserDefault()->setIntegerForKey("last_selected_server_id", m_selectedServer.id);
    CCUserDefault::sharedUserDefault()->setStringForKey("last_selected_server_name", m_selectedServer.name);
    CCUserDefault::sharedUserDefault()->setStringForKey("last_selected_server_domain", m_selectedServer.domain);
    CCUserDefault::sharedUserDefault()->flush();
}

void CServerListMgr::loadCacheServer() {
    int savedId = CCUserDefault::sharedUserDefault()->getIntegerForKey("last_selected_server_id", 0);
    std::string savedName = CCUserDefault::sharedUserDefault()->getStringForKey("last_selected_server_name", "");
    std::string savedDomain = CCUserDefault::sharedUserDefault()->getStringForKey("last_selected_server_domain", "");

    if (savedId > 0 && !savedName.empty()) {
        m_selectedServer.id = savedId;
        m_selectedServer.name = savedName;
        if (!savedDomain.empty()) {
            m_selectedServer.domain = savedDomain;
        }
    }
}

// Hàm bổ trợ trích xuất nội dung giữa cặp thẻ XML
static std::string extractSubTag(const std::string& xml, const std::string& tag) {
    std::string openTag = "<" + tag + ">";
    std::string closeTag = "</" + tag + ">";
    size_t start = xml.find(openTag);
    if (start == std::string::npos) return "";
    start += openTag.length();
    size_t end = xml.find(closeTag, start);
    if (end == std::string::npos) return "";
    return xml.substr(start, end - start);
}

// Phân tích danh sách máy chủ trả về từ /xk_r_dir
bool CServerListMgr::parseServerListXml(const std::string& xmlData) {
    if (xmlData.empty()) return false;

    // Tìm tất cả các khối <server>...</server> hoặc <svr>...</svr>
    std::vector<std::string> serverBlocks;
    
    const char* openTags[2] = {"<server", "<svr"};
    const char* closeTags[2] = {"</server>", "</svr>"};

    for (int t = 0; t < 2; ++t) {
        size_t pos = 0;
        while ((pos = xmlData.find(openTags[t], pos)) != std::string::npos) {
            size_t endPos = xmlData.find(closeTags[t], pos);
            if (endPos != std::string::npos) {
                endPos += strlen(closeTags[t]);
                serverBlocks.push_back(xmlData.substr(pos, endPos - pos));
                pos = endPos;
            } else {
                break;
            }
        }
    }

    if (serverBlocks.empty()) {
        CCLog("[CServerListMgr] Khong tim thay the server trong XML");
        return false;
    }

    m_serverList.clear();

    for (size_t i = 0; i < serverBlocks.size(); ++i) {
        const std::string& block = serverBlocks[i];
        ServerInfoData info;

        std::string idStr = extractSubTag(block, "id");
        if (idStr.empty()) idStr = extractSubTag(block, "areaid");
        if (!idStr.empty()) info.id = atoi(idStr.c_str());

        std::string nameStr = extractSubTag(block, "name");
        if (!nameStr.empty()) info.name = nameStr;

        std::string domainStr = extractSubTag(block, "domain");
        if (domainStr.empty()) domainStr = extractSubTag(block, "url");
        if (!domainStr.empty()) info.domain = domainStr;

        // Trích xuất trạng thái & recommend
        std::string stateStr = extractSubTag(block, "state");
        if (!stateStr.empty()) info.state = atoi(stateStr.c_str());

        std::string recStr = extractSubTag(block, "recommend");
        if (!recStr.empty()) info.recommend = atoi(recStr.c_str());

        bool exists = false;
        for (size_t s = 0; s < m_serverList.size(); ++s) {
            if (m_serverList[s].id == info.id) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            m_serverList.push_back(info);
            CCLog("[CServerListMgr] Da nap Server [%d]: %s (%s)", info.id, info.name.c_str(), info.domain.c_str());
        }
    }

    // Nếu server đã lưu trong cache không còn trong danh sách -> Chọn server đề cử
    bool foundCached = false;
    for (size_t i = 0; i < m_serverList.size(); ++i) {
        if (m_serverList[i].id == m_selectedServer.id) {
            m_selectedServer = m_serverList[i]; // cập nhật lại IP/tên mới nhất
            foundCached = true;
            break;
        }
    }

    if (!foundCached) {
        setSelectConfig(getRecommendServer());
    }

    return true;
}
