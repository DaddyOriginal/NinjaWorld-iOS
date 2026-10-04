#ifndef __C_EIGHT_GATE_MGR_H__
#define __C_EIGHT_GATE_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>
#include <vector>

USING_NS_CC;

#define kNotificationEightGateUpdated "kNotificationEightGateUpdated"
#define kNotificationEightGateOpened "kNotificationEightGateOpened"
#define kNotificationTrainSoulUpdated "kNotificationTrainSoulUpdated"
#define kNotificationTrainSoulCollected "kNotificationTrainSoulCollected"

/**
 * Thuộc tính thưởng của 1 huyệt Bát Môn
 */
struct EightGateBonusItem {
    int type; // 1: Công, 2: Thủ, 3: Chakra, 4: Nhẫn thuật %, 5: Công %, 6: Thủ %, 7: Chakra %
    int value;

    EightGateBonusItem() : type(0), value(0) {}
    EightGateBonusItem(int t, int v) : type(t), value(v) {}
};

/**
 * CEightGateMgr: Quản lý Bát Môn Độn Giáp & Luyện Hồn (Eight Inner Gates)
 * Giao tiếp với /rl_w_eight_gate & /rl_r_eight_gate
 */
class CEightGateMgr : public CCObject {
private:
    CEightGateMgr();
    virtual ~CEightGateMgr();

    static CEightGateMgr* s_instance;

    // Bát Môn
    int m_soul;
    int m_gateLevel;
    int m_gateId;
    int m_costSoul;
    int m_costSilver;
    std::vector<EightGateBonusItem> m_currentGateBonuses;

    // Thuộc tính tổng cộng dồn
    int m_addAttack;
    int m_addDefense;
    int m_addChakra;
    int m_addAttackPer;
    int m_addDefensePer;
    int m_addChakraPer;

    // Luyện Hồn (Train Soul)
    int m_normalCostSilver;
    int m_specialCostGold;
    int m_normalTimesLeft;
    int m_specialTimesLeft;
    int m_freeSpecialMultiTimes;
    int m_baseSoulGainNormal;
    int m_baseSoulGainSpecial;
    int m_pendingSoul;
    int m_currentMultiplier;
    bool m_isTraining;

    void onEightGateResp(CRLRequest* pRequest);
    void onOpenGateResp(CRLRequest* pRequest);
    void onTrainSoulResp(CRLRequest* pRequest);

public:
    static CEightGateMgr* sharedManager();
    static void purge();

    // Requests Bát Môn
    void requestGateInfo();
    void requestOpenGate();

    // Requests Luyện Hồn
    void requestTrainSoulInfo();
    void requestDoTrain(bool isSpecial);
    void requestMultiplySoul(bool isSpecial);
    void requestCollectSoul();

    // XML Parsers
    void parseGateInfoXml(const std::string& xmlStr);
    void parseOpenGateXml(const std::string& xmlStr);
    void parseTrainSoulXml(const std::string& xmlStr);

    // Getters Bát Môn
    int getSoul() const { return m_soul; }
    int getGateLevel() const { return m_gateLevel; }
    int getGateId() const { return m_gateId; }
    int getCostSoul() const { return m_costSoul; }
    int getCostSilver() const { return m_costSilver; }
    const std::vector<EightGateBonusItem>& getCurrentGateBonuses() const { return m_currentGateBonuses; }

    int getAddAttack() const { return m_addAttack; }
    int getAddDefense() const { return m_addDefense; }
    int getAddChakra() const { return m_addChakra; }
    int getAddAttackPer() const { return m_addAttackPer; }
    int getAddDefensePer() const { return m_addDefensePer; }
    int getAddChakraPer() const { return m_addChakraPer; }

    // Getters Luyện Hồn
    int getNormalCostSilver() const { return m_normalCostSilver; }
    int getSpecialCostGold() const { return m_specialCostGold; }
    int getNormalTimesLeft() const { return m_normalTimesLeft; }
    int getSpecialTimesLeft() const { return m_specialTimesLeft; }
    int getFreeSpecialMultiTimes() const { return m_freeSpecialMultiTimes; }
    int getBaseSoulGainNormal() const { return m_baseSoulGainNormal; }
    int getBaseSoulGainSpecial() const { return m_baseSoulGainSpecial; }
    int getPendingSoul() const { return m_pendingSoul; }
    int getCurrentMultiplier() const { return m_currentMultiplier; }
    bool isTraining() const { return m_isTraining; }
};

#endif // __C_EIGHT_GATE_MGR_H__
