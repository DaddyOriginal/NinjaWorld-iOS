#ifndef _CAWARD_CENTER_MGR_H_
#define _CAWARD_CENTER_MGR_H_

#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;

struct SAwardItem {
    int type;
    int dropId;
    int count;

    SAwardItem() : type(0), dropId(0), count(1) {}
    SAwardItem(int t, int d, int c = 1) : type(t), dropId(d), count(c) {}
};

struct SAwardMsg {
    int id;
    long long timestamp;
    std::string title;
    std::string content;
    std::vector<SAwardItem> awards;

    SAwardMsg() : id(0), timestamp(0) {}
};

class CAwardCenterMgr : public CCObject {
private:
    static CAwardCenterMgr* m_pSharedMgr;

    std::vector<SAwardMsg> m_awardList;
    std::string m_expiry;

    CCObject* m_pTarget;
    SEL_CallFuncO m_pSelector;

    CAwardCenterMgr();
    virtual ~CAwardCenterMgr();

public:
    static CAwardCenterMgr* sharedManager();
    static void purgeManager();

    const std::vector<SAwardMsg>& getAwardList() const { return m_awardList; }
    const std::string& getExpiry() const { return m_expiry; }
    int getAwardCount() const { return (int)m_awardList.size(); }

    void requestAwardList(CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);
    void requestClaimAward(int msgId, CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);
    void requestClaimAll(CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);

    void onHttpRequestCompleted(CCObject* pSender);

private:
    void parseAwardListXml(const std::string& xmlStr);
    void parseClaimAwardXml(const std::string& xmlStr, int claimedMsgId);
    void parseClaimAllXml(const std::string& xmlStr);
};

#endif // _CAWARD_CENTER_MGR_H_
