#ifndef _CNINJA_TABLE_MGR_H_
#define _CNINJA_TABLE_MGR_H_

#include "cocos2d.h"
#include <string>
#include <map>

USING_NS_CC;

/**
 * Cấu trúc thông số mẫu của một Nhẫn Giả trong bảng dữ liệu trò chơi (2,598 tướng)
 * Ánh xạ chuẩn từ ninjainfo.dbf / ninjainfo.json / ninjainfo.bin
 */
struct NinjaTableEntry {
    int id;
    std::string name;
    std::string icon;
    int star;
    int atkMin;
    int atkMax;
    int defMin;
    int defMax;
    int chaMin;
    int chaMax;
    int atkUpgMin;
    int atkUpgMax;
    int defUpgMin;
    int defUpgMax;
    int chaUpgMin;
    int chaUpgMax;
    std::string desc;
};

/**
 * CNinjaTableMgr: Quản lý nạp và tra cứu bảng cấu hình Nhẫn Giả (2,598 tướng)
 * Tải trực tiếp từ file nhị phân tốc độ cao data/ninjainfo.bin hoặc data/ninjainfo.json
 */
class CNinjaTableMgr : public CCObject {
private:
    static CNinjaTableMgr* s_instance;
    std::map<int, NinjaTableEntry> m_entries;
    bool m_isLoaded;

    CNinjaTableMgr();
    virtual ~CNinjaTableMgr();

public:
    static CNinjaTableMgr* sharedManager();
    static void purge();

    // Nạp toàn bộ bảng dữ liệu vào bộ nhớ
    bool loadTable();

    // Tra cứu thông số Nhẫn Giả theo ID mẫu
    const NinjaTableEntry* getNinjaEntry(int ninjaId);

    // Kiểm tra xem bảng đã được nạp chưa
    bool isLoaded() const { return m_isLoaded; }

    // Tổng số lượng Nhẫn Giả trong hệ thống
    size_t getCount() const { return m_entries.size(); }
};

#endif // _CNINJA_TABLE_MGR_H_
