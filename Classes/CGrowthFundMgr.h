#ifndef _CGROWTH_FUND_MGR_H_
#define _CGROWTH_FUND_MGR_H_

#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;

struct SGrowthFundItem {
    int id;
    int level;
    int cash;
    int state; // 0 = chưa đạt điều kiện, 1 = có thể nhận, 2 = đã nhận
    std::string title;
    std::string desc;

    SGrowthFundItem() : id(0), level(0), cash(0), state(0) {}
};

struct SGrowthFundPreview {
    int buyCash;
    int multiple;
    int totalCash;
    int buyState; // 0 = chưa mua, 1 = đã mua
    int needVip;

    SGrowthFundPreview() : buyCash(500), multiple(5), totalCash(2500), buyState(0), needVip(2) {}
};

class CGrowthFundMgr : public CCObject {
private:
    static CGrowthFundMgr* m_pSharedMgr;

    SGrowthFundPreview m_preview;
    std::vector<SGrowthFundItem> m_milestones;

    CCObject* m_pTarget;
    SEL_CallFuncO m_pSelector;

    CGrowthFundMgr();
    virtual ~CGrowthFundMgr();

public:
    static CGrowthFundMgr* sharedManager();
    static void purgeManager();

    const SGrowthFundPreview& getPreview() const { return m_preview; }
    const std::vector<SGrowthFundItem>& getMilestones() const { return m_milestones; }
    int getMilestoneCount() const { return (int)m_milestones.size(); }
    bool isBought() const { return m_preview.buyState == 1; }

    void requestFundInfo(CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);
    void requestBuyFund(CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);
    void requestClaimMilestone(int id, CCObject* pTarget = NULL, SEL_CallFuncO pSelector = NULL);

    void onHttpRequestCompleted(CCObject* pSender);

private:
    void parseFundXml(const std::string& xmlStr);
};

#endif // _CGROWTH_FUND_MGR_H_
