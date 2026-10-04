#ifndef _CPLAYER_DATA_MGR_H_
#define _CPLAYER_DATA_MGR_H_

#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>
#include "CPlayerNinja.h"
#include "CActiveTeamMgr.h"
#include "CGameCardEquipment.h"
#include "CPlayerNinjaPiece.h"
#include "CGameCardMark.h"

USING_NS_CC;

/**
 * CPlayerDataMgr: Singleton quản lý toàn bộ dữ liệu & thông số người chơi (Firefly MMO)
 * Lưu trữ thông tin tài khoản, danh sách thẻ Nhẫn Giả, Trang bị, Ấn ký, Mảnh ghép và Đội hình chiến đấu.
 */
class CPlayerDataMgr : public CCObject {
private:
    static CPlayerDataMgr* s_instance;

    // Thông tin tài khoản & phiên
    int m_userId;
    std::string m_username;
    std::string m_nickname;
    std::string m_sessionToken;
    int m_serverId;
    std::string m_serverName;

    // Chỉ số người chơi
    int m_level;
    int m_exp;
    int m_maxExp;
    int m_gold;          // Vàng
    int m_silver;        // Bạc / Xu
    int m_bodyValue;     // Thể lực (Stamina)
    int m_maxBodyValue;
    int m_vipLevel;
    int m_combatPower;   // Tổng lực chiến
    int m_countryType;   // Làng (1: Hỏa, 2: Phong, 3: Lôi, 4: Thổ, 5: Thủy)
    int m_avatarId;

    // Quản lý Thẻ Nhẫn Giả và Đội hình ra trận
    std::vector<CPlayerNinja*> m_ninjas;
    CActiveTeamMgr* m_pActiveTeam;

    // Quản lý Túi đồ (Trang bị, Mảnh ghép, Ấn ký)
    std::vector<CGameCardEquipment*> m_equipments;
    std::vector<CPlayerNinjaPiece*> m_pieces;
    std::vector<CGameCardMark*> m_marks;

    CPlayerDataMgr();
    virtual ~CPlayerDataMgr();

public:
    static CPlayerDataMgr* sharedManager();
    static void purge();

    // Phân tích XML phản hồi từ máy chủ sau khi đăng nhập / đổi đội hình
    bool parseLoginXml(const std::string& xmlData);

    // ========================================================
    // QUẢN LÝ THẺ NHẪN GIẢ & ĐỘI HÌNH
    // ========================================================
    CActiveTeamMgr* getActiveTeam() const { return m_pActiveTeam; }
    const std::vector<CPlayerNinja*>& getAllNinjas() const { return m_ninjas; }

    // ========================================================
    // QUẢN LÝ TÚI ĐỒ (TRANG BỊ, MẢNH GHÉP, ẤN KÝ)
    // ========================================================
    const std::vector<CGameCardEquipment*>& getAllEquipments() const { return m_equipments; }
    std::vector<CGameCardEquipment*> getEquipmentsBySlot(int slotType) const; // 1: Vũ khí, 2: Giáp, 3: Trang sức
    CGameCardEquipment* getEquipmentBySeq(int seq);

    const std::vector<CPlayerNinjaPiece*>& getAllPieces() const { return m_pieces; }
    std::vector<CPlayerNinjaPiece*> getPiecesByType(int pieceType) const; // 1: Ninja, 2: Equip, 4: Ninjutsu, 5: Pet
    CPlayerNinjaPiece* getPieceById(int pieceId);

    const std::vector<CGameCardMark*>& getAllMarks() const { return m_marks; }
    CGameCardMark* getMarkBySeq(int seq);

    // Tìm Ninja theo Runtime Sequence ID
    CPlayerNinja* getNinjaBySeq(int seq);

    // Thêm hoặc cập nhật Thẻ Ninja
    void addOrUpdateNinja(CPlayerNinja* pNinja);
    void addPlayerNinja(CPlayerNinja* pNinja) { addOrUpdateNinja(pNinja); }

    // Xóa thẻ Ninja
    void removeNinja(int seq);

    // Số ô đội hình tối đa mở khóa theo cấp độ
    int firefly_GetMaxTeamMembers() const;

    // Tổng sức tấn công của toàn bộ đội hình
    int firefly_GetPlayerAttack();

    // ========================================================
    // GETTERS & SETTERS CƠ BẢN
    // ========================================================
    int getUserId() const { return m_userId; }
    void setUserId(int uid) { m_userId = uid; }

    const std::string& getUsername() const { return m_username; }
    void setUsername(const std::string& name) { m_username = name; }

    const std::string& getNickname() const { return m_nickname; }
    void setNickname(const std::string& name) { m_nickname = name; }

    const std::string& getSessionToken() const { return m_sessionToken; }
    void setSessionToken(const std::string& token) { m_sessionToken = token; }

    int getServerId() const { return m_serverId; }
    void setServerId(int sid) { m_serverId = sid; }

    const std::string& getServerName() const { return m_serverName; }
    void setServerName(const std::string& sname) { m_serverName = sname; }

    int firefly_GetGold() const { return m_gold; }
    void firefly_SetGold(int val) { m_gold = val; }
    void firefly_AddGold(int val) { m_gold += val; }
    int getGold() const { return firefly_GetGold(); }
    void setGold(int val) { firefly_SetGold(val); }
    void addGold(int val) { firefly_AddGold(val); }

    int firefly_GetSilver() const { return m_silver; }
    void firefly_SetSilver(int val) { m_silver = val; }
    void firefly_AddSilver(int val) { m_silver += val; }
    int getSilver() const { return firefly_GetSilver(); }
    void setSilver(int val) { firefly_SetSilver(val); }
    void addSilver(int val) { firefly_AddSilver(val); }

    int firefly_GetBodyValue() const { return m_bodyValue; }
    void firefly_SetBodyValue(int val) { m_bodyValue = val; }

    int firefly_GetVipLevel() const { return m_vipLevel; }
    void firefly_SetVipLevel(int val) { m_vipLevel = val; }

    int getLevel() const { return m_level; }
    void setLevel(int lvl) { m_level = lvl; }

    int getCombatPower() const { return m_combatPower; }
    void setCombatPower(int cp) { m_combatPower = cp; }

    int firefly_GetCountryType() const { return m_countryType; }
    void firefly_SetCountryType(int c) { m_countryType = c; }

    int getAvatarId() const { return m_avatarId; }
    void setAvatarId(int id) { m_avatarId = id; }
};

#endif // _CPLAYER_DATA_MGR_H_
