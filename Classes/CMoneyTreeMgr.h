#ifndef _CMONEY_TREE_MGR_H_
#define _CMONEY_TREE_MGR_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

struct SMoneyTreeConfig {
    int timesPerDay;
    int lowMultiple;
    int topMultiple;
    int shakeCost;
    int freeShake;
    int startSilver;

    SMoneyTreeConfig()
        : timesPerDay(15)
        , lowMultiple(2)
        , topMultiple(10)
        , shakeCost(2)
        , freeShake(1)
        , startSilver(2000)
    {
    }
};

struct SMoneyTreeUser {
    int usedTimes;
    int freeTimes;
    int latestSilver;
    long long latestTime;
    int coin;
    int cash;

    SMoneyTreeUser()
        : usedTimes(0)
        , freeTimes(0)
        , latestSilver(0)
        , latestTime(0)
        , coin(0)
        , cash(0)
    {
    }
};

class CMoneyTreeMgr : public CCObject {
private:
    static CMoneyTreeMgr* m_pSharedMgr;

    SMoneyTreeConfig m_config;
    SMoneyTreeUser m_user;

    CCObject* m_pTarget;
    SEL_CallFuncO m_pSelector;

    CMoneyTreeMgr();
    virtual ~CMoneyTreeMgr();

public:
    static CMoneyTreeMgr* sharedManager();
    static void purgeManager();

    const SMoneyTreeConfig& getConfig() const { return m_config; }
    const SMoneyTreeUser& getUser() const { return m_user; }

    int getRemainTimes() const {
        int rem = m_config.timesPerDay - m_user.usedTimes;
        return rem > 0 ? rem : 0;
    }

    bool hasFreeTurn() const {
        return m_user.freeTimes < m_config.freeShake;
    }

    void requestTreeInfo(CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);
    void requestSwingTree(CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);

    void onHttpRequestCompleted(CCObject* pSender);

private:
    void parseTreeInfoXml(const std::string& xmlStr);
    void parseSwingXml(const std::string& xmlStr);
};

#endif // _CMONEY_TREE_MGR_H_
