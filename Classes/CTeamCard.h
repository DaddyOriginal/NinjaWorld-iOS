#ifndef _CTEAM_CARD_H_
#define _CTEAM_CARD_H_

#include "cocos2d.h"
#include "CPlayerNinja.h"
#include <string>
#include <vector>

USING_NS_CC;

/**
 * CTeamCard: Ô vị trí triển khai trong Đội hình ra trận (Slot 1 -> 6)
 * Chứa Nhẫn Giả đang đứng vị trí đó, kèm 4 ô Trang bị (Vũ khí, Giáp, Trang sức, Ấn ký)
 * và 4 ô Nhẫn thuật bí truyền, cùng danh sách Duyên phận (Combin).
 */
class CTeamCard : public CCObject {
private:
    int m_seq;                  // Vị trí ra trận (1 -> 6)
    int m_ninjaInstanceSeq;     // Mã seq của thẻ CPlayerNinja gắn vào vị trí này
    CPlayerNinja* m_pNinja;     // Con trỏ tới Thẻ Ninja

    // Các trang bị & ấn ký được lắp trên ô đội hình này
    int m_weaponId;             // Slot 0: Vũ khí (WeaponID)
    int m_armorId;              // Slot 1: Áo giáp (ArmorID)
    int m_decoratorId;          // Slot 2: Trang sức (DecoratorID)
    int m_markId;               // Slot 3: Ấn ký Vĩ thú (MarkID)

    // 4 slot Nhẫn thuật
    int m_ninjutsu1;            // Slot 4: Nhẫn thuật 1
    int m_ninjutsu2;            // Slot 5: Nhẫn thuật 2
    int m_ninjutsu3;            // Slot 6: Nhẫn thuật 3
    int m_ninjutsu4;            // Slot 7: Nhẫn thuật 4

    // Chỉ số chiến đấu thực tế sau khi tính buff duyên phận & trang bị
    int m_attackLow;
    int m_attackHigh;
    int m_defenselow;
    int m_defenseHigh;
    int m_chakraLow;
    int m_chakraHigh;

    // Danh sách Duyên phận (Kích hoạt & Sẵn sàng)
    std::vector<int> m_combinList;
    std::vector<int> m_readyCombinList;

public:
    CTeamCard();
    virtual ~CTeamCard();

    static CTeamCard* create();
    static CTeamCard* createWithXmlSnippet(const std::string& xmlSnippet);

    bool initFromXmlSnippet(const std::string& xmlSnippet);

    // Gán con trỏ Ninja từ kho thẻ người chơi
    void bindNinja(CPlayerNinja* pNinja);

    // Getters & Setters theo chuẩn native firefly_...
    int getSeq() const { return m_seq; }
    void setSeq(int seq) { m_seq = seq; }

    int getNinjaInstanceSeq() const { return m_ninjaInstanceSeq; }
    CPlayerNinja* firefly_GetNinja() const { return m_pNinja; }
    bool firefly_HasNinja() const { return (m_pNinja != NULL && m_ninjaInstanceSeq > 0); }

    int getEquipId(int slot) const;
    void setEquipId(int slot, int equipId);

    int firefly_GetWeaponId() const { return m_weaponId; }
    int firefly_GetArmorId() const { return m_armorId; }
    int firefly_GetDecoratorId() const { return m_decoratorId; }
    int firefly_GetMarkId() const { return m_markId; }

    int firefly_GetNinjutsu(int slot) const;

    // Chỉ số Công/Thủ
    int firefly_GetAttackLow() const { return m_attackLow; }
    int firefly_GetAttackHigh() const { return m_attackHigh; }
    int firefly_GetDefenseLow() const { return m_defenselow; }
    int firefly_GetDefenseHigh() const { return m_defenseHigh; }
    int firefly_GetChakraLow() const { return m_chakraLow; }
    int firefly_GetChakraHigh() const { return m_chakraHigh; }

    int firefly_GetRoundAttack() const { return (m_attackLow + m_attackHigh) / 2; }
    int firefly_GetRoundDefense() const { return (m_defenselow + m_defenseHigh) / 2; }

    const std::vector<int>& firefly_GetCombin() const { return m_combinList; }
    const std::vector<int>& firefly_GetReadyCombin() const { return m_readyCombinList; }
    int firefly_GetCombinCount() const { return (int)m_combinList.size(); }
};

#endif // _CTEAM_CARD_H_
