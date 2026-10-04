#ifndef __C_DAILY_TASK_TABLE_MGR_H__
#define __C_DAILY_TASK_TABLE_MGR_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <map>

USING_NS_CC;

struct DailyTaskConfigEntry {
    int id;
    std::string name;       // Tiêu đề/mô tả chi tiết nhiệm vụ ("Complete training 20 times")
    std::string title;      // Tên ngắn ("Training")
    int actionType;
    int targetCount;        // Tiến trình cần đạt (20, 5, 2...)
    int scorePoints;        // Điểm năng động nhận được (10, 15, 30...)
    int isOpen;             // 1: Hoạt động, 0: Đóng
};

/**
 * CDailyTaskTableMgr: Quản lý nạp và tra cứu cấu hình bảng nhiệm vụ hàng ngày (dailytask_info.bin)
 * Chuẩn định dạng từ dailytask_info.json (Struct_Dailytask_Info)
 */
class CDailyTaskTableMgr : public CCObject {
private:
    CDailyTaskTableMgr();
    virtual ~CDailyTaskTableMgr();

    static CDailyTaskTableMgr* s_instance;
    bool m_isLoaded;

    std::map<int, DailyTaskConfigEntry> m_tasks;
    std::vector<DailyTaskConfigEntry> m_taskList;

public:
    static CDailyTaskTableMgr* sharedManager();
    static void purge();

    bool loadTables();

    const DailyTaskConfigEntry* getTask(int taskId);
    const std::map<int, DailyTaskConfigEntry>& getAllTasks() const { return m_tasks; }
    const std::vector<DailyTaskConfigEntry>& getTaskList() const { return m_taskList; }
    int getTaskCount() const { return (int)m_tasks.size(); }
};

#endif // __C_DAILY_TASK_TABLE_MGR_H__
