#ifndef __C_FRIEND_MGR_H__
#define __C_FRIEND_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>
#include <vector>

USING_NS_CC;

#define kNotificationFriendListUpdated "kNotificationFriendListUpdated"
#define kNotificationFriendSearchUpdated "kNotificationFriendSearchUpdated"
#define kNotificationFriendActionSuccess "kNotificationFriendActionSuccess"

/**
 * Cấu trúc thông tin bạn bè
 */
struct FriendInfo {
    int playerId;
    std::string name;
    int level;
    int combatPower;
    int country;
    int firstNinja;
    int firstNinjaStar;
    bool isOnline;

    FriendInfo()
        : playerId(0), level(1), combatPower(0), country(1), firstNinja(1), firstNinjaStar(4), isOnline(false) {}
};

/**
 * CFriendMgr: Quản lý danh sách bạn bè, tìm kiếm, kết bạn và tương tác bạn bè
 * Giao tiếp với /rl_r_friend (GET/POST) & /rl_w_friend (POST)
 */
class CFriendMgr : public CCObject {
private:
    CFriendMgr();
    virtual ~CFriendMgr();

    static CFriendMgr* s_instance;

    std::vector<FriendInfo> m_friendList;
    std::vector<FriendInfo> m_searchResult;

    void onFriendListResp(CRLRequest* pRequest);
    void onSearchFriendResp(CRLRequest* pRequest);
    void onFriendActionResp(CRLRequest* pRequest);

public:
    static CFriendMgr* sharedManager();
    static void purge();

    // Nạp danh sách bạn bè
    void requestFriendList(int page = 1);

    // Tìm kiếm bạn bè theo tên
    void requestSearchFriends(const std::string& keyword);

    // Thao tác bạn bè (Thêm, Xóa, Đồng ý, Từ chối)
    void requestAddFriend(int toUid);
    void requestDeleteFriend(int toUid);
    void requestAcceptFriend(int toUid);
    void requestRejectFriend(int toUid);

    // XML Parser
    void parseFriendListXml(const std::string& xmlStr, std::vector<FriendInfo>& outList);

    const std::vector<FriendInfo>& getFriendList() const { return m_friendList; }
    const std::vector<FriendInfo>& getSearchResult() const { return m_searchResult; }
};

#endif // __C_FRIEND_MGR_H__
