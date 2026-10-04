#ifndef __C_TOWER_MGR_H__
#define __C_TOWER_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include "CTowerTableMgr.h"
#include <string>
#include <vector>

USING_NS_CC;

struct TowerFightResult {
    bool isSuccess;
    bool isWin;
    int expGained;
    int silverGained;
    int nextChapter;
    int nextProcess;
    int nextRound;
    std::string bossName;
    std::string dropItemName;
};

class CTowerMgr : public CCObject {
private:
    CTowerMgr();
    virtual ~CTowerMgr();

    static CTowerMgr* s_instance;

    int m_selectedChapter;
    int m_currentChapter;
    int m_currentRound;
    int m_currentProcess;
    int m_maxChap;
    int m_remainReset;

    TowerFightResult m_lastFightResult;

    void onTowerInfoResp(CRLRequest* pRequest);
    void onFightFloorResp(CRLRequest* pRequest);
    void onSweepResp(CRLRequest* pRequest);

public:
    static CTowerMgr* sharedManager();
    static void purge();

    // Getters & Setters
    int getSelectedChapter() const { return m_selectedChapter; }
    void setSelectedChapter(int chap) { m_selectedChapter = chap; }

    int getCurrentChapter() const { return m_currentChapter; }
    int getCurrentRound() const { return m_currentRound; }
    int getCurrentProcess() const { return m_currentProcess; }
    int getMaxChap() const { return m_maxChap; }
    int getRemainReset() const { return m_remainReset; }

    bool isChapterUnlocked(int chapId) const { return chapId <= m_maxChap; }
    bool isStageCleared(int stageIndex) const { return stageIndex < m_currentProcess; }
    bool isStageActive(int stageIndex) const { return stageIndex == m_currentProcess; }

    const TowerFightResult& getLastFightResult() const { return m_lastFightResult; }

    // Network APIs
    void requestTowerInfo();
    void requestFightFloor(int chapterId, int roundId);
    void requestSweepTower(int toChapter);
    void requestResetTower();

    // XML parsing helpers
    void parseTowerInfoXml(const std::string& xmlStr);
    void parseFightResultXml(const std::string& xmlStr);
};

#endif // __C_TOWER_MGR_H__
