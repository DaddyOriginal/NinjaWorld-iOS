#ifndef _CITEM_TABLE_MGR_H_
#define _CITEM_TABLE_MGR_H_

#include "cocos2d.h"
#include <string>
#include <map>

USING_NS_CC;

// Thông số mẫu Trang bị (equipmentinfo.bin - 84 trang bị)
struct EquipTableEntry {
    int id;
    std::string name;
    std::string icon;
    int type;           // 1: Vũ khí, 2: Giáp, 3: Trang sức (hoặc dựa trên icon)
    int star;
    int atkLow;
    int atkHigh;
    int defLow;
    int defHigh;
    int chaLow;
    int chaHigh;
    int atkGrowth;
    int defGrowth;
    int chaGrowth;
    int maxLevel;
    std::string desc;

    int getSlotType() const {
        if (icon.find("armor") != std::string::npos) return 2;      // Áo giáp
        if (icon.find("ornament") != std::string::npos) return 3;   // Trang sức
        if (icon.find("access") != std::string::npos) return 3;     // Trang sức
        return 1; // Vũ khí
    }
};

// Thông số mẫu Ấn Ký Vĩ Thú (markinfo.bin - 91 ấn ký)
struct MarkTableEntry {
    int id;
    std::string name;
    std::string icon;
    int quality;
    int maxLevel;
    float atkRate;
    float defRate;
    float chaRate;
    std::string desc;
};

// Thông số mẫu Mảnh Ghép (piece_info.bin - 424 loại mảnh)
struct PieceTableEntry {
    int pieceId;
    std::string name;
    int type;           // 1: Ninja, 2: Equip, 4: Ninjutsu, 5: Pet
    int subType;
    int star;
    int targetId;
    int requireCount;
    int consumePropId;
};

/**
 * CItemTableMgr: Quản lý nạp và tra cứu các bảng vật phẩm game
 * (Trang bị, Ấn ký, Mảnh ghép) nạp trực tiếp từ binary cache data/
 */
class CItemTableMgr : public CCObject {
private:
    static CItemTableMgr* s_instance;
    std::map<int, EquipTableEntry> m_equips;
    std::map<int, MarkTableEntry> m_marks;
    std::map<int, PieceTableEntry> m_pieces;
    bool m_isLoaded;

    CItemTableMgr();
    virtual ~CItemTableMgr();

public:
    static CItemTableMgr* sharedManager();
    static void purge();

    bool loadTables();

    const EquipTableEntry* getEquipEntry(int equipId);
    const MarkTableEntry* getMarkEntry(int markId);
    const PieceTableEntry* getPieceEntry(int pieceId);

    size_t getEquipCount() const { return m_equips.size(); }
    size_t getMarkCount() const { return m_marks.size(); }
    size_t getPieceCount() const { return m_pieces.size(); }
};

#endif // _CITEM_TABLE_MGR_H_
