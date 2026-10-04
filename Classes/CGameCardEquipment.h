#ifndef _CGAME_CARD_EQUIPMENT_H_
#define _CGAME_CARD_EQUIPMENT_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

/**
 * CGameCardEquipment: Thẻ Trang bị của người chơi (Vũ khí, Giáp, Trang sức)
 * Ánh xạ trực tiếp từ thẻ <card type="2|3|4"> của máy chủ gửi về.
 */
class CGameCardEquipment : public CCObject {
private:
    int m_seq;                  // Runtime Sequence ID trong túi người chơi
    int m_equipId;              // Template ID (1 -> 84 trong equipmentinfo.bin)
    int m_level;                // Cấp độ cường hóa
    int m_star;                 // Số sao
    int m_slotType;             // 1: Vũ khí (Type 2), 2: Áo giáp (Type 3), 3: Trang sức (Type 4)
    int m_equippedNinjaSeq;     // 0 nếu đang ở trong túi, > 0 nếu đang được Ninja mặc

    // Thông số chiến đấu thực tế
    int m_attack;
    int m_defense;
    int m_chakra;

    std::string m_name;
    std::string m_icon;
    std::string m_desc;

public:
    CGameCardEquipment();
    virtual ~CGameCardEquipment();

    static CGameCardEquipment* create();
    static CGameCardEquipment* createWithXmlSnippet(const std::string& xmlSnippet);

    bool initFromXmlSnippet(const std::string& xmlSnippet);
    void calculateStats();

    // Getters & Setters
    int getSeq() const { return m_seq; }
    void setSeq(int seq) { m_seq = seq; }

    int getEquipId() const { return m_equipId; }
    void setEquipId(int id) { m_equipId = id; calculateStats(); }

    int getLevel() const { return m_level; }
    void setLevel(int lvl) { m_level = lvl; calculateStats(); }

    int getStar() const { return m_star; }
    void setStar(int star) { m_star = star; }

    int getSlotType() const { return m_slotType; }
    void setSlotType(int slot) { m_slotType = slot; }

    int getEquippedNinjaSeq() const { return m_equippedNinjaSeq; }
    void setEquippedNinjaSeq(int nSeq) { m_equippedNinjaSeq = nSeq; }
    bool isEquipped() const { return m_equippedNinjaSeq > 0; }

    int getAttack() const { return m_attack; }
    int getDefense() const { return m_defense; }
    int getChakra() const { return m_chakra; }

    const std::string& getName() const { return m_name; }
    const std::string& getIcon() const { return m_icon; }
    const std::string& getDesc() const { return m_desc; }

    std::string getIconPath() const;
};

#endif // _CGAME_CARD_EQUIPMENT_H_
