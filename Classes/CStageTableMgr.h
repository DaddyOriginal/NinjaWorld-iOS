#ifndef __C_STAGE_TABLE_MGR_H__
#define __C_STAGE_TABLE_MGR_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>

USING_NS_CC;

struct StoryTableEntry {
    int id;
    std::string name;
    int icon;
    int rewardGold[15];
    int rewardSilver[15];
    int rewardPower[15];
    int rewardChest[15];
};

struct RoundTableEntry {
    int roundId;
    int subroundId;
    int storyId;
    int orderIdx;
    std::string name;
    std::string icon;
    int needPower;
    int addExp;
    int addSilver;
    int dropId;
    std::string desc;
    int isBoss;
    int stageId;
    std::string stageName;
    int limitTimes;
};

class CStageTableMgr : public CCObject {
private:
    CStageTableMgr();
    virtual ~CStageTableMgr();

    static CStageTableMgr* s_instance;
    bool m_isLoaded;

    std::map<int, StoryTableEntry> m_stories;
    std::map<int, RoundTableEntry> m_rounds;
    std::map<int, std::vector<RoundTableEntry> > m_roundsByStory;

public:
    static CStageTableMgr* sharedManager();
    static void purge();

    bool loadTables();

    // Story / Chapter queries
    const StoryTableEntry* getStoryEntry(int storyId);
    int getStoryCount() const { return (int)m_stories.size(); }
    const std::map<int, StoryTableEntry>& getAllStories() const { return m_stories; }

    // Round / Stage queries
    const RoundTableEntry* getRoundEntry(int roundId);
    std::vector<RoundTableEntry> getRoundsForStory(int storyId);
    std::vector<RoundTableEntry> getRoundsForStoryAndStage(int storyId, int stageId);
    std::vector<int> getDistinctStagesForStory(int storyId);
    std::string getStageName(int storyId, int stageId);
    int getMaxStageForStory(int storyId);
};

#endif // __C_STAGE_TABLE_MGR_H__
