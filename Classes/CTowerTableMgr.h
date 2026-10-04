#ifndef __C_TOWER_TABLE_MGR_H__
#define __C_TOWER_TABLE_MGR_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>

USING_NS_CC;

struct TowerChapterEntry {
    int id;
    std::string name;
    int silverReward;
    int goldReward;
    int expReward;
    std::string desc;
    std::string icon;
    int fillDrop1;
    int fillDrop2;
    int fillDrop3;
    int fillDrop4;
};

struct TowerFloorEntry {
    int seq;
    int chapterId;
    std::string bkIcon;
    int npcs[15];
    int needBody;
    int expGain;
    int silverGain;
    int chestId;
    std::string desc;
    int attackType;
};

class CTowerTableMgr : public CCObject {
private:
    CTowerTableMgr();
    virtual ~CTowerTableMgr();

    static CTowerTableMgr* s_instance;
    bool m_isLoaded;

    std::map<int, TowerChapterEntry> m_chapters;
    std::map<int, TowerFloorEntry> m_floors;
    std::map<int, std::vector<TowerFloorEntry> > m_floorsByChapter;

public:
    static CTowerTableMgr* sharedManager();
    static void purge();

    bool loadTables();

    const TowerChapterEntry* getChapter(int chapterId);
    int getChapterCount() const { return (int)m_chapters.size(); }
    const std::map<int, TowerChapterEntry>& getAllChapters() const { return m_chapters; }

    const TowerFloorEntry* getFloor(int floorSeq);
    std::vector<TowerFloorEntry> getFloorsForChapter(int chapterId);
    int getFloorCount() const { return (int)m_floors.size(); }
};

#endif // __C_TOWER_TABLE_MGR_H__
