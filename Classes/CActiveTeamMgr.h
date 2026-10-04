#ifndef _CACTIVE_TEAM_MGR_H_
#define _CACTIVE_TEAM_MGR_H_

#include "cocos2d.h"
#include "CTeamCard.h"
#include <vector>
#include <string>

USING_NS_CC;

/**
 * CActiveTeamMgr: Quản lý Đội hình ra trận (Team 1 / Team 2) của người chơi
 * Chứa tối đa 6 vị trí triển khai (CTeamCard), tính toán tổng công/thủ toàn đội.
 */
class CActiveTeamMgr : public CCObject {
private:
    std::vector<CTeamCard*> m_teamCards;    // 6 ô vị trí chiến đấu (Slot 0 -> 5 tương ứng Vị trí 1 -> 6)
    int m_activeTeamIndex;                  // 0: Đội hình 1 (Công), 1: Đội hình 2 (Thủ)
    int m_totalAttack;
    int m_totalDefense;
    int m_totalWarPower;
    bool m_needReload;

public:
    CActiveTeamMgr();
    virtual ~CActiveTeamMgr();

    static CActiveTeamMgr* create();
    static CActiveTeamMgr* sharedManager();

    // Nạp dữ liệu đội hình từ khối XML <ninjalist> hoặc toàn bộ XML phản hồi
    bool initFromXml(const std::string& xmlData);

    // Lấy thẻ vị trí theo chỉ số (index: 0 -> 5 hoặc slot: 1 -> 6)
    CTeamCard* firefly_GetTeamCardByIndex(int index);
    CTeamCard* getTeamCard(int slot) { return firefly_GetTeamCardByIndex(slot - 1); }

    // Số lượng thành viên đang ra trận
    int firefly_GetMemberCount() const;

    // Tổng chỉ số toàn đội
    int firefly_GetAttack() const { return m_totalAttack; }
    int firefly_GetDefense() const { return m_totalDefense; }
    int firefly_GetWarPower() const { return m_totalWarPower; }

    int getActiveTeamIndex() const { return m_activeTeamIndex; }
    void setActiveTeamIndex(int idx) { m_activeTeamIndex = idx; }

    bool firefly_GetNeedReload() const { return m_needReload; }
    void firefly_SetNeedReload(bool val) { m_needReload = val; }

    // Tính toán lại tổng chỉ số toàn đội hình
    void recalculateTeamStats();

    // Xóa sạch dữ liệu đội hình
    void clear();
};

#endif // _CACTIVE_TEAM_MGR_H_
