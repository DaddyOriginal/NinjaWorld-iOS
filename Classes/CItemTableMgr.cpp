#include "CItemTableMgr.h"
#include <cstdio>
#include <cstring>

#pragma pack(push, 1)
struct RawEquipRecord {
    int id;
    char name[64];
    char icon[32];
    int type;
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
    char desc[256];
};

struct RawMarkRecord {
    int id;
    char name[64];
    char icon[32];
    int quality;
    int maxLevel;
    float atkRate;
    float defRate;
    float chaRate;
    char desc[256];
};

struct RawPieceRecord {
    int pieceId;
    char name[64];
    int type;
    int subType;
    int star;
    int targetId;
    int requireCount;
    int consumePropId;
};
#pragma pack(pop)

CItemTableMgr* CItemTableMgr::s_instance = NULL;

CItemTableMgr::CItemTableMgr()
    : m_isLoaded(false)
{
}

CItemTableMgr::~CItemTableMgr() {
    m_equips.clear();
    m_marks.clear();
    m_pieces.clear();
}

CItemTableMgr* CItemTableMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CItemTableMgr();
        s_instance->loadTables();
    }
    return s_instance;
}

void CItemTableMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

bool CItemTableMgr::loadTables() {
    if (m_isLoaded) return true;

    // 1. Nạp equipmentinfo.bin
    std::string eqPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/equipmentinfo.bin");
    if (eqPath.empty()) eqPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("equipmentinfo.bin");
    FILE* fpEq = !eqPath.empty() ? fopen(eqPath.c_str(), "rb") : NULL;
    if (fpEq) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpEq) == 4 && memcmp(magic, "EQIP", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpEq) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawEquipRecord raw;
                    if (fread(&raw, sizeof(RawEquipRecord), 1, fpEq) == 1) {
                        EquipTableEntry e;
                        e.id = raw.id;
                        e.name = std::string(raw.name);
                        e.icon = std::string(raw.icon);
                        e.type = raw.type;
                        e.star = raw.star;
                        e.atkLow = raw.atkLow;
                        e.atkHigh = raw.atkHigh;
                        e.defLow = raw.defLow;
                        e.defHigh = raw.defHigh;
                        e.chaLow = raw.chaLow;
                        e.chaHigh = raw.chaHigh;
                        e.atkGrowth = raw.atkGrowth;
                        e.defGrowth = raw.defGrowth;
                        e.chaGrowth = raw.chaGrowth;
                        e.maxLevel = raw.maxLevel;
                        e.desc = std::string(raw.desc);
                        m_equips[e.id] = e;
                    }
                }
            }
        }
        fclose(fpEq);
    }

    // 2. Nạp markinfo.bin
    std::string mPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/markinfo.bin");
    if (mPath.empty()) mPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("markinfo.bin");
    FILE* fpM = !mPath.empty() ? fopen(mPath.c_str(), "rb") : NULL;
    if (fpM) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpM) == 4 && memcmp(magic, "MARK", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpM) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawMarkRecord raw;
                    if (fread(&raw, sizeof(RawMarkRecord), 1, fpM) == 1) {
                        MarkTableEntry m;
                        m.id = raw.id;
                        m.name = std::string(raw.name);
                        m.icon = std::string(raw.icon);
                        m.quality = raw.quality;
                        m.maxLevel = raw.maxLevel;
                        m.atkRate = raw.atkRate;
                        m.defRate = raw.defRate;
                        m.chaRate = raw.chaRate;
                        m.desc = std::string(raw.desc);
                        m_marks[m.id] = m;
                    }
                }
            }
        }
        fclose(fpM);
    }

    // 3. Nạp piece_info.bin
    std::string pPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/piece_info.bin");
    if (pPath.empty()) pPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("piece_info.bin");
    FILE* fpP = !pPath.empty() ? fopen(pPath.c_str(), "rb") : NULL;
    if (fpP) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpP) == 4 && memcmp(magic, "PIEC", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpP) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawPieceRecord raw;
                    if (fread(&raw, sizeof(RawPieceRecord), 1, fpP) == 1) {
                        PieceTableEntry p;
                        p.pieceId = raw.pieceId;
                        p.name = std::string(raw.name);
                        p.type = raw.type;
                        p.subType = raw.subType;
                        p.star = raw.star;
                        p.targetId = raw.targetId;
                        p.requireCount = raw.requireCount;
                        p.consumePropId = raw.consumePropId;
                        m_pieces[p.pieceId] = p;
                    }
                }
            }
        }
        fclose(fpP);
    }

    m_isLoaded = true;
    CCLog("[CItemTableMgr] Da nap thanh cong: %d Trang bi, %d An ky, %d Manh ghep!",
          (int)m_equips.size(), (int)m_marks.size(), (int)m_pieces.size());
    return true;
}

const EquipTableEntry* CItemTableMgr::getEquipEntry(int equipId) {
    if (!m_isLoaded) loadTables();
    std::map<int, EquipTableEntry>::const_iterator it = m_equips.find(equipId);
    if (it != m_equips.end()) return &(it->second);
    return NULL;
}

const MarkTableEntry* CItemTableMgr::getMarkEntry(int markId) {
    if (!m_isLoaded) loadTables();
    std::map<int, MarkTableEntry>::const_iterator it = m_marks.find(markId);
    if (it != m_marks.end()) return &(it->second);
    return NULL;
}

const PieceTableEntry* CItemTableMgr::getPieceEntry(int pieceId) {
    if (!m_isLoaded) loadTables();
    std::map<int, PieceTableEntry>::const_iterator it = m_pieces.find(pieceId);
    if (it != m_pieces.end()) return &(it->second);
    return NULL;
}
