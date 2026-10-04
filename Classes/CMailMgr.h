#ifndef __C_MAIL_MGR_H__
#define __C_MAIL_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>
#include <vector>

USING_NS_CC;

#define kNotificationMailListUpdated "kNotificationMailListUpdated"
#define kNotificationMailClaimed "kNotificationMailClaimed"

/**
 * Cấu trúc dữ liệu 1 bức thư trong Hòm Thư
 */
struct MailItemData {
    int id;
    int subType;          // 1: Chiến báo (Battle), 2: Bạn bè (Friend), 3: Hệ thống (System)
    std::string title;
    std::string content;
    std::string senderName;
    std::string timeStr;
    int silverReward;
    int goldReward;
    int status;           // 0: Chưa đọc, 1: Đã đọc, 2: Đã nhận quà
    bool hasReward;

    MailItemData()
        : id(0), subType(3), silverReward(0), goldReward(0), status(0), hasReward(false) {}
};

/**
 * CMailMgr: Quản lý hòm thư & tin nhắn hệ thống
 * Giao tiếp với /rl_r_msg & /rl_w_msg
 */
class CMailMgr : public CCObject {
private:
    CMailMgr();
    virtual ~CMailMgr();

    static CMailMgr* s_instance;

    std::vector<MailItemData> m_mailList;
    int m_currentTab; // 1, 2, 3

    void onMailListResp(CRLRequest* pRequest);
    void onClaimMailResp(CRLRequest* pRequest);

public:
    static CMailMgr* sharedManager();
    static void purge();

    // Nạp danh sách thư (tab: 1=Chiến báo, 2=Bạn bè, 3=Hệ thống)
    void requestMailList(int tab = 3);

    // Nhận quà đính kèm từ thư
    void requestClaimMail(int mailId);

    // Xóa thư
    void requestDeleteMail(int mailId);

    // Phân tích phản hồi XML
    void parseMailListXml(const std::string& xmlStr);
    void parseClaimMailXml(const std::string& xmlStr, int mailId);

    const std::vector<MailItemData>& getMailList() const { return m_mailList; }
    int getUnreadCount() const;
};

#endif // __C_MAIL_MGR_H__
