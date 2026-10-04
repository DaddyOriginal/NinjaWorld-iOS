#include "CNinjaTableMgr.h"
#include <cstdio>
#include <cstring>

#pragma pack(push, 1)
struct RawBinaryRecord {
    int id;
    char name[64];
    char icon[32];
    int star;
    int atkMin;
    int atkMax;
    int defMin;
    int defMax;
    int chaMin;
    int chaMax;
    int atkUpgMin;
    int atkUpgMax;
    int defUpgMin;
    int defUpgMax;
    int chaUpgMin;
    int chaUpgMax;
    char desc[256];
};
#pragma pack(pop)

CNinjaTableMgr* CNinjaTableMgr::s_instance = NULL;

CNinjaTableMgr::CNinjaTableMgr()
    : m_isLoaded(false)
{
}

CNinjaTableMgr::~CNinjaTableMgr() {
    m_entries.clear();
}

CNinjaTableMgr* CNinjaTableMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CNinjaTableMgr();
        s_instance->loadTable();
    }
    return s_instance;
}

void CNinjaTableMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

bool CNinjaTableMgr::loadTable() {
    if (m_isLoaded) return true;

    // 1. Tìm đường dẫn file nhị phân tốc độ cao data/ninjainfo.bin
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/ninjainfo.bin");
    if (fullPath.empty()) {
        fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("ninjainfo.bin");
    }

    FILE* fp = NULL;
    if (!fullPath.empty()) {
        fp = fopen(fullPath.c_str(), "rb");
    }

    if (fp) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fp) == 4 && memcmp(magic, "NINJ", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fp) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawBinaryRecord raw;
                    if (fread(&raw, sizeof(RawBinaryRecord), 1, fp) == 1) {
                        NinjaTableEntry entry;
                        entry.id = raw.id;
                        entry.name = std::string(raw.name);
                        entry.icon = std::string(raw.icon);
                        entry.star = raw.star;
                        entry.atkMin = raw.atkMin;
                        entry.atkMax = raw.atkMax;
                        entry.defMin = raw.defMin;
                        entry.defMax = raw.defMax;
                        entry.chaMin = raw.chaMin;
                        entry.chaMax = raw.chaMax;
                        entry.atkUpgMin = raw.atkUpgMin;
                        entry.atkUpgMax = raw.atkUpgMax;
                        entry.defUpgMin = raw.defUpgMin;
                        entry.defUpgMax = raw.defUpgMax;
                        entry.chaUpgMin = raw.chaUpgMin;
                        entry.chaUpgMax = raw.chaUpgMax;
                        entry.desc = std::string(raw.desc);

                        m_entries[entry.id] = entry;
                    }
                }
                fclose(fp);
                m_isLoaded = true;
                CCLog("[CNinjaTableMgr] Da nap thanh cong %d tuong tu ninjainfo.bin!", (int)m_entries.size());
                return true;
            }
        }
        fclose(fp);
    }

    CCLog("[CNinjaTableMgr] Canh bao: Khong the mo data/ninjainfo.bin");
    return false;
}

const NinjaTableEntry* CNinjaTableMgr::getNinjaEntry(int ninjaId) {
    if (!m_isLoaded) {
        loadTable();
    }

    std::map<int, NinjaTableEntry>::const_iterator it = m_entries.find(ninjaId);
    if (it != m_entries.end()) {
        return &(it->second);
    }
    return NULL;
}
