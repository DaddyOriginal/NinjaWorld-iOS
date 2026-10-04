#include "CDailyTaskTableMgr.h"
#include <cstdio>
#include <cstring>

#pragma pack(push, 1)
struct RawDailyTaskRecord {
    int id;
    char name[128];
    char title[64];
    int actionType;
    int targetCount;
    int scorePoints;
    int isOpen;
};
#pragma pack(pop)

CDailyTaskTableMgr* CDailyTaskTableMgr::s_instance = NULL;

CDailyTaskTableMgr::CDailyTaskTableMgr()
    : m_isLoaded(false)
{
}

CDailyTaskTableMgr::~CDailyTaskTableMgr() {
    m_tasks.clear();
    m_taskList.clear();
}

CDailyTaskTableMgr* CDailyTaskTableMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CDailyTaskTableMgr();
        s_instance->loadTables();
    }
    return s_instance;
}

void CDailyTaskTableMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

bool CDailyTaskTableMgr::loadTables() {
    if (m_isLoaded) return true;

    std::string taskPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("data/dailytask_info.bin");
    if (taskPath.empty()) {
        taskPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("dailytask_info.bin");
    }

    FILE* fp = NULL;
    if (!taskPath.empty()) {
        fp = fopen(taskPath.c_str(), "rb");
    }

    if (fp) {
        char magic[5] = {0};
        if (fread(magic, 1, 4, fp) == 4 && memcmp(magic, "DTSK", 4) == 0) {
            unsigned int count = 0;
            if (fread(&count, sizeof(unsigned int), 1, fp) == 1) {
                for (unsigned int i = 0; i < count; ++i) {
                    RawDailyTaskRecord raw;
                    if (fread(&raw, sizeof(RawDailyTaskRecord), 1, fp) == 1) {
                        DailyTaskConfigEntry entry;
                        entry.id = raw.id;
                        entry.name = raw.name;
                        entry.title = raw.title;
                        entry.actionType = raw.actionType;
                        entry.targetCount = raw.targetCount;
                        entry.scorePoints = raw.scorePoints;
                        entry.isOpen = raw.isOpen;

                        m_tasks[entry.id] = entry;
                        m_taskList.push_back(entry);
                    }
                }
                CCLog("[CDailyTaskTableMgr] Load thanh cong %u nhiem vu hang ngay tu dailytask_info.bin!", count);
                m_isLoaded = true;
            }
        }
        fclose(fp);
    } else {
        CCLog("[CDailyTaskTableMgr] CANH BAO: Khong tim thay data/dailytask_info.bin!");
    }

    return m_isLoaded;
}

const DailyTaskConfigEntry* CDailyTaskTableMgr::getTask(int taskId) {
    std::map<int, DailyTaskConfigEntry>::const_iterator it = m_tasks.find(taskId);
    if (it != m_tasks.end()) {
        return &(it->second);
    }
    return NULL;
}
