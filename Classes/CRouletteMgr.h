#ifndef __C_ROULETTE_MGR_H__
#define __C_ROULETTE_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>
#include <vector>

USING_NS_CC;

#define kNotificationWheelInfoUpdated "kNotificationWheelInfoUpdated"
#define kNotificationWheelSpinResult "kNotificationWheelSpinResult"

/**
 * Cấu trúc 1 ô phần thưởng trên Vòng Quay (12 ô)
 */
struct RouletteSlotConfig {
    int slotId;
    int itemId;
    int itemType;
    int count;
    std::string name;

    RouletteSlotConfig()
        : slotId(1), itemId(0), itemType(0), count(1) {}
};

/**
 * Kết quả 1 lần quay từ máy chủ
 */
struct RouletteSpinItem {
    int slotId;
    int itemId;
    int count;
    std::string name;

    RouletteSpinItem() : slotId(1), itemId(0), count(1) {}
};

/**
 * CRouletteMgr: Quản lý Vòng Quay May Mắn (Roulette / Wheel)
 * Giao tiếp với /rl_r_wheel & /rl_w_wheel (CMD 1400, 1401)
 */
class CRouletteMgr : public CCObject {
private:
    CRouletteMgr();
    virtual ~CRouletteMgr();

    static CRouletteMgr* s_instance;

    std::vector<RouletteSlotConfig> m_slots;
    int m_jackpot;
    int m_score;
    int m_freeSpins;
    int m_timeRemaining;
    int m_costOnce;
    int m_costTen;

    std::vector<RouletteSpinItem> m_lastSpinResults;

    void onWheelInfoResp(CRLRequest* pRequest);
    void onSpinResp(CRLRequest* pRequest);

public:
    static CRouletteMgr* sharedManager();
    static void purge();

    // Requests
    void requestWheelInfo();
    void requestSpin(bool isTenTimes);

    // Parsers
    void parseWheelInfoXml(const std::string& xmlStr);
    void parseSpinResultXml(const std::string& xmlStr);

    // Getters
    const std::vector<RouletteSlotConfig>& getSlots() const { return m_slots; }
    int getJackpot() const { return m_jackpot; }
    int getScore() const { return m_score; }
    int getFreeSpins() const { return m_freeSpins; }
    int getTimeRemaining() const { return m_timeRemaining; }
    int getCostOnce() const { return m_costOnce; }
    int getCostTen() const { return m_costTen; }
    const std::vector<RouletteSpinItem>& getLastSpinResults() const { return m_lastSpinResults; }
};

#endif // __C_ROULETTE_MGR_H__
