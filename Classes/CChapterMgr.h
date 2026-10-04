#ifndef __C_CHAPTER_MGR_H__
#define __C_CHAPTER_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>
#include <vector>
#include <map>

USING_NS_CC;

struct UnlockedChapterInfo {
    int chapterId;
    int hardLevel;
    int currentSection;
    int currentSubround;
    int maxSections;
};

struct PVEFightResult {
    bool isSuccess;
    bool isWin;
    int starRating;
    int expGained;
    int silverGained;
    int goldGained;
    int remainingStamina;
    int nextChapter;
    int nextSection;
    int nextStage;
    std::string awardDesc;
    std::string enemyName;
    int enemyNinjaId;
};

class CChapterMgr : public CCObject {
private:
    CChapterMgr();
    virtual ~CChapterMgr();

    static CChapterMgr* s_instance;

    int m_selectedChapter;
    int m_selectedSection;
    int m_selectedRound;

    int m_currentChapter;
    int m_currentSection;
    int m_currentStage;
    int m_maxUnlockedChapter;

    std::map<int, UnlockedChapterInfo> m_unlockedChapters;
    PVEFightResult m_lastFightResult;

    void onDungeonInfoResp(CRLRequest* pRequest);
    void onBattleResp(CRLRequest* pRequest);

public:
    static CChapterMgr* sharedManager();
    static void purge();

    // Getters & Setters
    int getSelectedChapter() const { return m_selectedChapter; }
    void setSelectedChapter(int chap);

    int getSelectedSection() const { return m_selectedSection; }
    void setSelectedSection(int sec);

    int getSelectedRound() const { return m_selectedRound; }
    void setSelectedRound(int rnd) { m_selectedRound = rnd; }

    int getCurrentChapter() const { return m_currentChapter; }
    void setCurrentChapter(int chap) { m_currentChapter = chap; }

    int getCurrentSection() const { return m_currentSection; }
    void setCurrentSection(int sec) { m_currentSection = sec; }

    int getCurrentStage() const { return m_currentStage; }
    void setCurrentStage(int stg) { m_currentStage = stg; }

    int getMaxUnlockedChapter() const { return m_maxUnlockedChapter; }

    bool isChapterUnlocked(int chapterId) const;
    bool isSectionUnlocked(int chapterId, int sectionId) const;

    const PVEFightResult& getLastFightResult() const { return m_lastFightResult; }

    // Network APIs
    void requestDungeonInfo();
    void requestBattle(int chapterId, int sectionId, int roundId, bool isBoss = false);

    // Parsing helpers
    void parseDungeonInfoXml(const std::string& xmlStr);
    void parseBattleResultXml(const std::string& xmlStr);
};

#endif // __C_CHAPTER_MGR_H__
