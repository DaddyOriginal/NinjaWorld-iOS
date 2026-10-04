#include "CDailyTaskMgr.h"
#include "CDailyTaskTableMgr.h"
#include "CPlayerDataMgr.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CDailyTaskMgr* CDailyTaskMgr::s_instance = NULL;

CDailyTaskMgr::CDailyTaskMgr()
    : m_curScore(0)
    , m_totalScore(250)
{
}

CDailyTaskMgr::~CDailyTaskMgr() {
    m_taskList.clear();
    m_boxList.clear();
}

CDailyTaskMgr* CDailyTaskMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CDailyTaskMgr();
    }
    return s_instance;
}

void CDailyTaskMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

void CDailyTaskMgr::requestDailyTasks() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_dailytask");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "1");
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CDailyTaskMgr::onDailyTasksResp));
    req->send();
    CCLog("[CDailyTaskMgr] Gui yeu cau lay danh sach nhiem vu hang ngay (/rl_r_dailytask - Cmd 1)");
}

void CDailyTaskMgr::onDailyTasksResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CDailyTaskMgr] Loi goi /rl_r_dailytask: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    parseDailyTasksXml(pRequest->getResponseString());
    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationDailyTasksUpdated, NULL);
}

void CDailyTaskMgr::parseDailyTasksXml(const std::string& xmlStr) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CDailyTaskMgr] Loi parse XML nhiem vu hang ngay!");
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    const char* code = root->Attribute("code");
    if (code && strcmp(code, "0") != 0) {
        CCLog("[CDailyTaskMgr] Server tra ve ma loi code=%s", code);
        return;
    }

    // 1. Phân tích điểm năng động người chơi <player>
    XMLElement* playerElem = root->FirstChildElement("player");
    if (playerElem) {
        XMLElement* totalElem = playerElem->FirstChildElement("total");
        if (totalElem && totalElem->GetText()) {
            m_totalScore = atoi(totalElem->GetText());
        }
        XMLElement* curElem = playerElem->FirstChildElement("current");
        if (curElem && curElem->GetText()) {
            m_curScore = atoi(curElem->GetText());
        }
    }

    // 2. Phân tích danh sách tiến trình nhiệm vụ <task_list>
    m_taskList.clear();
    XMLElement* taskListElem = root->FirstChildElement("task_list");
    if (taskListElem) {
        XMLElement* itemElem = taskListElem->FirstChildElement("item");
        while (itemElem) {
            DailyTaskItem taskItem;
            taskItem.id = itemElem->IntAttribute("id");
            taskItem.curProcess = itemElem->IntAttribute("process");

            // Tra cứu cấu hình từ bảng CDailyTaskTableMgr
            const DailyTaskConfigEntry* cfg = CDailyTaskTableMgr::sharedManager()->getTask(taskItem.id);
            if (cfg) {
                taskItem.title = cfg->title;
                taskItem.desc = cfg->name;
                taskItem.needProcess = cfg->targetCount;
                taskItem.points = cfg->scorePoints;
            } else {
                taskItem.title = "Nhiệm vụ";
                taskItem.desc = "";
                taskItem.needProcess = 1;
                taskItem.points = 10;
            }

            taskItem.isDone = (taskItem.curProcess >= taskItem.needProcess);
            m_taskList.push_back(taskItem);

            itemElem = itemElem->NextSiblingElement("item");
        }
    }

    // 3. Phân tích danh sách rương mốc năng động <award_list>
    m_boxList.clear();
    XMLElement* awardListElem = root->FirstChildElement("award_list");
    if (awardListElem) {
        XMLElement* awardElem = awardListElem->FirstChildElement("item");
        while (awardElem) {
            DailyTaskAwardBox box;
            box.id = awardElem->IntAttribute("id");
            box.status = awardElem->IntAttribute("status");
            box.cost = awardElem->IntAttribute("cost");

            XMLElement* dropListElem = awardElem->FirstChildElement("drop_list");
            if (dropListElem) {
                XMLElement* dropItem = dropListElem->FirstChildElement("item");
                while (dropItem) {
                    if (dropItem->GetText()) {
                        box.dropList.push_back(atoi(dropItem->GetText()));
                    }
                    dropItem = dropItem->NextSiblingElement("item");
                }
            }

            m_boxList.push_back(box);
            awardElem = awardElem->NextSiblingElement("item");
        }
    }

    CCLog("[CDailyTaskMgr] Dong bo thanh cong: Diem=%d/%d, Tasks=%lu, Boxes=%lu",
          m_curScore, m_totalScore, (unsigned long)m_taskList.size(), (unsigned long)m_boxList.size());
}

void CDailyTaskMgr::requestClaimBox(int awardId) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_dailytask");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "2");
    req->setParam("AwardID", CCString::createWithFormat("%d", awardId)->getCString());
    if (CPlayerDataMgr::sharedManager()) {
        req->setParam("Uid", CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getUserId())->getCString());
    }

    req->setCallback(this, callfuncND_selector(CDailyTaskMgr::onClaimBoxResp));
    req->send();
    CCLog("[CDailyTaskMgr] Gui yeu cau nhan thuong ruong nang dong (AwardID=%d)", awardId);
}

void CDailyTaskMgr::onClaimBoxResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CDailyTaskMgr] Loi nhan thuong ruong: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    std::string resp = pRequest->getResponseString();
    parseClaimBoxXml(resp, 0);
}

void CDailyTaskMgr::parseClaimBoxXml(const std::string& xmlStr, int awardId) {
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) {
        CCLog("[CDailyTaskMgr] Loi parse XML nhan thuong ruong!");
        return;
    }

    XMLElement* root = doc.RootElement();
    if (!root) return;

    const char* code = root->Attribute("code");
    if (code && strcmp(code, "0") != 0) {
        CCLog("[CDailyTaskMgr] Loi server tra ve khi nhan ruong: code=%s", code);
        return;
    }

    // Cập nhật phần thưởng vào CPlayerDataMgr
    XMLElement* awardElem = root->FirstChildElement("award");
    if (awardElem) {
        int coin = awardElem->IntAttribute("coin");
        int cash = awardElem->IntAttribute("cash");

        if (CPlayerDataMgr::sharedManager()) {
            if (coin > 0) CPlayerDataMgr::sharedManager()->firefly_AddSilver(coin);
            if (cash > 0) CPlayerDataMgr::sharedManager()->firefly_AddGold(cash);
        }

        CCLog("[CDailyTaskMgr] Nhan thuong thanh cong: +%d Bac, +%d Vang!", coin, cash);
    }

    CCNotificationCenter::sharedNotificationCenter()->postNotification(kNotificationDailyBoxClaimed, NULL);

    // Tự động làm mới lại toàn bộ danh sách để cập nhật trạng thái rương
    requestDailyTasks();
}

const DailyTaskAwardBox* CDailyTaskMgr::getBoxById(int awardId) const {
    for (size_t i = 0; i < m_boxList.size(); ++i) {
        if (m_boxList[i].id == awardId) {
            return &(m_boxList[i]);
        }
    }
    return NULL;
}
