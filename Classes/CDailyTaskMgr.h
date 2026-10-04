#ifndef __C_DAILY_TASK_MGR_H__
#define __C_DAILY_TASK_MGR_H__

#include "cocos2d.h"
#include "CRLRequest.h"
#include <string>
#include <vector>
#include <map>

USING_NS_CC;

#define kNotificationDailyTasksUpdated "kNotificationDailyTasksUpdated"
#define kNotificationDailyBoxClaimed "kNotificationDailyBoxClaimed"

/**
 * Trạng thái tiến trình 1 nhiệm vụ hàng ngày
 */
struct DailyTaskItem {
    int id;
    int curProcess;
    int needProcess;
    int points;
    std::string title;
    std::string desc;
    bool isDone;

    DailyTaskItem()
        : id(0), curProcess(0), needProcess(0), points(0), isDone(false) {}
};

/**
 * Cấu hình và trạng thái 1 Rương Mốc Năng Động (30, 80, 120, 160, 200, 250 điểm)
 */
struct DailyTaskAwardBox {
    int id;
    int status;      // 1: chưa đủ điểm nhận, 2: đã nhận, 4: đủ điểm sẵn sàng nhận
    int cost;        // Điểm cần đạt
    std::vector<int> dropList; // Danh sách ID drop theo dropinfo

    DailyTaskAwardBox()
        : id(0), status(1), cost(0) {}
};

/**
 * CDailyTaskMgr: Singleton quản lý tiến trình nhiệm vụ hàng ngày và nhận rương năng động
 * Giao tiếp với /rl_r_dailytask (Cmd=1: lấy danh sách, Cmd=2: nhận thưởng rương)
 */
class CDailyTaskMgr : public CCObject {
private:
    CDailyTaskMgr();
    virtual ~CDailyTaskMgr();

    static CDailyTaskMgr* s_instance;

    int m_curScore;
    int m_totalScore;
    std::vector<DailyTaskItem> m_taskList;
    std::vector<DailyTaskAwardBox> m_boxList;

    void onDailyTasksResp(CRLRequest* pRequest);
    void onClaimBoxResp(CRLRequest* pRequest);

public:
    static CDailyTaskMgr* sharedManager();
    static void purge();

    // Gửi yêu cầu lấy danh sách nhiệm vụ & rương
    void requestDailyTasks();

    // Gửi yêu cầu nhận rương mốc năng động
    void requestClaimBox(int awardId);

    // Phân tích phản hồi XML
    void parseDailyTasksXml(const std::string& xmlStr);
    void parseClaimBoxXml(const std::string& xmlStr, int awardId);

    // Getters
    int getCurScore() const { return m_curScore; }
    int getTotalScore() const { return m_totalScore; }
    const std::vector<DailyTaskItem>& getTaskList() const { return m_taskList; }
    const std::vector<DailyTaskAwardBox>& getBoxList() const { return m_boxList; }
    const DailyTaskAwardBox* getBoxById(int awardId) const;
};

#endif // __C_DAILY_TASK_MGR_H__
