#include "CStageTableMgr.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

#pragma pack(push, 1)
struct RawStoryRecord {
    int id;
    char name[64];
    int icon;
    int rewardGold[15];
    int rewardSilver[15];
    int rewardPower[15];
    int rewardChest[15];
};

struct RawRoundRecord {
    int roundId;
    int subroundId;
    int storyId;
    int orderIdx;
    char name[64];
    char icon[32];
    int needPower;
    int addExp;
    int addSilver;
    int dropId;
    char desc[256];
    int isBoss;
    int stageId;
    char stageName[64];
    int limitTimes;
};
#pragma pack(pop)

CStageTableMgr* CStageTableMgr::s_instance = NULL;

CStageTableMgr::CStageTableMgr()
    : m_isLoaded(false)
{
}

CStageTableMgr::~CStageTableMgr() {
    m_stories.clear();
    m_rounds.clear();
    m_roundsByStory.clear();
}

CStageTableMgr* CStageTableMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CStageTableMgr();
        s_instance->loadTables();
    }
    return s_instance;
}

void CStageTableMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

bool CStageTableMgr::loadTables() {
    if (m_isLoaded) return true;

    // 1. Nạp bảng dữ liệu Chương truyện (data/storyinfo.bin)
    std::string storyPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/storyinfo.bin");
    if (storyPath.empty()) {
        storyPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("storyinfo.bin");
    }

    FILE* fpStory = NULL;
    if (!storyPath.empty()) {
        fpStory = fopen(storyPath.c_str(), "rb");
    }

    if (fpStory) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpStory) == 4 && memcmp(magic, "STRY", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpStory) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawStoryRecord raw;
                    if (fread(&raw, sizeof(RawStoryRecord), 1, fpStory) == 1) {
                        StoryTableEntry entry;
                        entry.id = raw.id;
                        entry.name = std::string(raw.name);
                        entry.icon = raw.icon;
                        for (int k = 0; k < 15; ++k) {
                            entry.rewardGold[k] = raw.rewardGold[k];
                            entry.rewardSilver[k] = raw.rewardSilver[k];
                            entry.rewardPower[k] = raw.rewardPower[k];
                            entry.rewardChest[k] = raw.rewardChest[k];
                        }
                        m_stories[entry.id] = entry;
                    }
                }
            }
        }
        fclose(fpStory);
        CCLog("[CStageTableMgr] Da nap thanh cong %d chuong cot truyen tu storyinfo.bin!", (int)m_stories.size());
    } else {
        CCLog("[CStageTableMgr] Canh bao: Khong tim thay data/storyinfo.bin");
    }

    // 2. Nạp bảng dữ liệu Ải / Vòng chơi (data/roundinfo.bin)
    std::string roundPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/roundinfo.bin");
    if (roundPath.empty()) {
        roundPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("roundinfo.bin");
    }

    FILE* fpRound = NULL;
    if (!roundPath.empty()) {
        fpRound = fopen(roundPath.c_str(), "rb");
    }

    if (fpRound) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fpRound) == 4 && memcmp(magic, "RNDI", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fpRound) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawRoundRecord raw;
                    if (fread(&raw, sizeof(RawRoundRecord), 1, fpRound) == 1) {
                        RoundTableEntry entry;
                        entry.roundId = raw.roundId;
                        entry.subroundId = raw.subroundId;
                        entry.storyId = raw.storyId;
                        entry.orderIdx = raw.orderIdx;
                        entry.name = std::string(raw.name);
                        entry.icon = std::string(raw.icon);
                        entry.needPower = raw.needPower;
                        entry.addExp = raw.addExp;
                        entry.addSilver = raw.addSilver;
                        entry.dropId = raw.dropId;
                        entry.desc = std::string(raw.desc);
                        entry.isBoss = raw.isBoss;
                        entry.stageId = raw.stageId;
                        entry.stageName = std::string(raw.stageName);
                        entry.limitTimes = raw.limitTimes;

                        m_rounds[entry.roundId] = entry;
                        m_roundsByStory[entry.storyId].push_back(entry);
                    }
                }
            }
        }
        fclose(fpRound);
        CCLog("[CStageTableMgr] Da nap thanh cong %d vong ai PVE tu roundinfo.bin!", (int)m_rounds.size());
    } else {
        CCLog("[CStageTableMgr] Canh bao: Khong tim thay data/roundinfo.bin");
    }

    m_isLoaded = true;
    return true;
}

const StoryTableEntry* CStageTableMgr::getStoryEntry(int storyId) {
    if (!m_isLoaded) loadTables();
    std::map<int, StoryTableEntry>::const_iterator it = m_stories.find(storyId);
    if (it != m_stories.end()) {
        return &(it->second);
    }
    return NULL;
}

const RoundTableEntry* CStageTableMgr::getRoundEntry(int roundId) {
    if (!m_isLoaded) loadTables();
    std::map<int, RoundTableEntry>::const_iterator it = m_rounds.find(roundId);
    if (it != m_rounds.end()) {
        return &(it->second);
    }
    return NULL;
}

std::vector<RoundTableEntry> CStageTableMgr::getRoundsForStory(int storyId) {
    if (!m_isLoaded) loadTables();
    std::map<int, std::vector<RoundTableEntry> >::const_iterator it = m_roundsByStory.find(storyId);
    if (it != m_roundsByStory.end()) {
        return it->second;
    }
    return std::vector<RoundTableEntry>();
}

std::vector<RoundTableEntry> CStageTableMgr::getRoundsForStoryAndStage(int storyId, int stageId) {
    if (!m_isLoaded) loadTables();
    std::vector<RoundTableEntry> result;
    std::map<int, std::vector<RoundTableEntry> >::const_iterator it = m_roundsByStory.find(storyId);
    if (it != m_roundsByStory.end()) {
        const std::vector<RoundTableEntry>& list = it->second;
        for (size_t i = 0; i < list.size(); ++i) {
            if (list[i].stageId == stageId) {
                result.push_back(list[i]);
            }
        }
    }
    return result;
}

std::vector<int> CStageTableMgr::getDistinctStagesForStory(int storyId) {
    if (!m_isLoaded) loadTables();
    std::vector<int> stages;
    std::map<int, std::vector<RoundTableEntry> >::const_iterator it = m_roundsByStory.find(storyId);
    if (it != m_roundsByStory.end()) {
        const std::vector<RoundTableEntry>& list = it->second;
        for (size_t i = 0; i < list.size(); ++i) {
            int stg = list[i].stageId;
            if (std::find(stages.begin(), stages.end(), stg) == stages.end()) {
                stages.push_back(stg);
            }
        }
    }
    std::sort(stages.begin(), stages.end());
    return stages;
}

std::string CStageTableMgr::getStageName(int storyId, int stageId) {
    if (!m_isLoaded) loadTables();
    std::map<int, std::vector<RoundTableEntry> >::const_iterator it = m_roundsByStory.find(storyId);
    if (it != m_roundsByStory.end()) {
        const std::vector<RoundTableEntry>& list = it->second;
        for (size_t i = 0; i < list.size(); ++i) {
            if (list[i].stageId == stageId && !list[i].stageName.empty()) {
                return list[i].stageName;
            }
        }
    }
    return "";
}

int CStageTableMgr::getMaxStageForStory(int storyId) {
    std::vector<int> stages = getDistinctStagesForStory(storyId);
    if (!stages.empty()) {
        return stages.back();
    }
    return 1;
}
