#include "CTowerTableMgr.h"
#include <cstdio>
#include <cstring>

#pragma pack(push, 1)
struct RawRushRecord {
    int id;
    char name[64];
    int silverReward;
    int goldReward;
    int expReward;
    char desc[256];
    char icon[32];
    int fillDrop1;
    int fillDrop2;
    int fillDrop3;
    int fillDrop4;
};

struct RawFloorRecord {
    int seq;
    int chapterId;
    char bkIcon[32];
    int npcs[15];
    int needBody;
    int expGain;
    int silverGain;
    int chestId;
    char desc[256];
    int attackType;
};
#pragma pack(pop)

CTowerTableMgr* CTowerTableMgr::s_instance = NULL;

CTowerTableMgr::CTowerTableMgr()
    : m_isLoaded(false)
{
}

CTowerTableMgr::~CTowerTableMgr() {
    m_chapters.clear();
    m_floors.clear();
    m_floorsByChapter.clear();
}

CTowerTableMgr* CTowerTableMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CTowerTableMgr();
        s_instance->loadTables();
    }
    return s_instance;
}

void CTowerTableMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

bool CTowerTableMgr::loadTables() {
    if (m_isLoaded) return true;

    // 1. Nạp bảng Chương Tháp (data/rushtower.bin)
    std::string rushPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/rushtower.bin");
    if (rushPath.empty()) {
        rushPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("rushtower.bin");
    }

    FILE* fpRush = NULL;
    if (!rushPath.empty()) {
        fpRush = fopen(rushPath.c_str(), "rb");
    }

    if (fpRush) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpRush) == 4 && memcmp(magic, "RUSH", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpRush) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawRushRecord raw;
                    if (fread(&raw, sizeof(RawRushRecord), 1, fpRush) == 1) {
                        TowerChapterEntry entry;
                        entry.id = raw.id;
                        entry.name = std::string(raw.name);
                        entry.silverReward = raw.silverReward;
                        entry.goldReward = raw.goldReward;
                        entry.expReward = raw.expReward;
                        entry.desc = std::string(raw.desc);
                        entry.icon = std::string(raw.icon);
                        entry.fillDrop1 = raw.fillDrop1;
                        entry.fillDrop2 = raw.fillDrop2;
                        entry.fillDrop3 = raw.fillDrop3;
                        entry.fillDrop4 = raw.fillDrop4;

                        m_chapters[entry.id] = entry;
                    }
                }
            }
        }
        fclose(fpRush);
        CCLog("[CTowerTableMgr] Da nap thanh cong %d chuong thap tu rushtower.bin!", (int)m_chapters.size());
    } else {
        CCLog("[CTowerTableMgr] Canh bao: Khong tim thay data/rushtower.bin");
    }

    // 2. Nạp bảng Tầng Tháp (data/towerfloor.bin)
    std::string floorPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/towerfloor.bin");
    if (floorPath.empty()) {
        floorPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("towerfloor.bin");
    }

    FILE* fpFloor = NULL;
    if (!floorPath.empty()) {
        fpFloor = fopen(floorPath.c_str(), "rb");
    }

    if (fpFloor) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpFloor) == 4 && memcmp(magic, "FLOR", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpFloor) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawFloorRecord raw;
                    if (fread(&raw, sizeof(RawFloorRecord), 1, fpFloor) == 1) {
                        TowerFloorEntry entry;
                        entry.seq = raw.seq;
                        entry.chapterId = raw.chapterId;
                        entry.bkIcon = std::string(raw.bkIcon);
                        for (int k = 0; k < 15; ++k) {
                            entry.npcs[k] = raw.npcs[k];
                        }
                        entry.needBody = raw.needBody;
                        entry.expGain = raw.expGain;
                        entry.silverGain = raw.silverGain;
                        entry.chestId = raw.chestId;
                        entry.desc = std::string(raw.desc);
                        entry.attackType = raw.attackType;

                        m_floors[entry.seq] = entry;
                        m_floorsByChapter[entry.chapterId].push_back(entry);
                    }
                }
            }
        }
        fclose(fpFloor);
        CCLog("[CTowerTableMgr] Da nap thanh cong %d tang thap tu towerfloor.bin!", (int)m_floors.size());
    } else {
        CCLog("[CTowerTableMgr] Canh bao: Khong tim thay data/towerfloor.bin");
    }

    m_isLoaded = true;
    return true;
}

const TowerChapterEntry* CTowerTableMgr::getChapter(int chapterId) {
    if (!m_isLoaded) loadTables();
    std::map<int, TowerChapterEntry>::const_iterator it = m_chapters.find(chapterId);
    if (it != m_chapters.end()) {
        return &(it->second);
    }
    return NULL;
}

const TowerFloorEntry* CTowerTableMgr::getFloor(int floorSeq) {
    if (!m_isLoaded) loadTables();
    std::map<int, TowerFloorEntry>::const_iterator it = m_floors.find(floorSeq);
    if (it != m_floors.end()) {
        return &(it->second);
    }
    return NULL;
}

std::vector<TowerFloorEntry> CTowerTableMgr::getFloorsForChapter(int chapterId) {
    if (!m_isLoaded) loadTables();
    std::map<int, std::vector<TowerFloorEntry> >::const_iterator it = m_floorsByChapter.find(chapterId);
    if (it != m_floorsByChapter.end()) {
        return it->second;
    }
    return std::vector<TowerFloorEntry>();
}
