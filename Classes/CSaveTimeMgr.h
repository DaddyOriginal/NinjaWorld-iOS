#ifndef __C_SAVE_TIME_MGR_H__
#define __C_SAVE_TIME_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>

USING_NS_CC;

#define kNotificationSaveTimeUpdated "kNotificationSaveTimeUpdated"
#define kNotificationSaveTimeExchanged "kNotificationSaveTimeExchanged"

/**
 * CSaveTimeMgr: Quản lý tính năng Tiết Kiệm Thời Gian / Đổi Mì Ramen lấy EXP & Bạc (/rl_r_savetime)
 */
class CSaveTimeMgr : public CCObject {
private:
    CSaveTimeMgr();
    virtual ~CSaveTimeMgr();

    static CSaveTimeMgr* s_instance;

    int m_chapter;
    int m_round;
    int m_ramenCount;
    int m_totalTrans;
    int m_usedTrans;
    int m_expPerRamen;
    int m_silverPerRamen;

    void onSaveTimeInfoResp(CRLRequest* pRequest);
    void onExchangeResp(CRLRequest* pRequest);

public:
    static CSaveTimeMgr* sharedManager();
    static void purge();

    // Requests
    void requestInfo();
    void requestExchange();

    // Parsers
    void parseInfoXml(const std::string& xmlStr);
    void parseExchangeXml(const std::string& xmlStr);

    // Getters
    int getChapter() const { return m_chapter; }
    int getRound() const { return m_round; }
    int getRamenCount() const { return m_ramenCount; }
    int getTotalTrans() const { return m_totalTrans; }
    int getUsedTrans() const { return m_usedTrans; }
    int getExpPerRamen() const { return m_expPerRamen; }
    int getSilverPerRamen() const { return m_silverPerRamen; }
};

#endif // __C_SAVE_TIME_MGR_H__
