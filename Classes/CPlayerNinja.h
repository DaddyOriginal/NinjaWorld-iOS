#ifndef _CPLAYER_NINJA_H_
#define _CPLAYER_NINJA_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

/**
 * CPlayerNinja: Thẻ bài Nhẫn Giả của người chơi (Model Data)
 * Đại diện cho một Ninja sở hữu, ánh xạ trực tiếp từ thẻ <card type="1"> của máy chủ.
 */
class CPlayerNinja : public CCObject {
private:
    int m_seq;                  // Mã định danh phiên bản thẻ (Runtime Sequence ID trong túi/đội hình)
    int m_ninjaId;              // Mã mẫu Ninja gốc (1: Lục Đạo, 2: Madara, 3: Hashirama, 4: Obito, 5: Naruto, ...)
    std::string m_name;         // Tên Nhẫn Giả (Naruto Uzumaki, Uchiha Sasuke, ...)
    std::string m_icon;         // Mã icon hình ảnh (npc30_6, npc17_4, ...)
    std::string m_desc;         // Tiểu sử nhân vật

    int m_level;                // Cấp độ hiện tại (1 - 100)
    int m_currExp;              // Điểm kinh nghiệm hiện tại
    int m_quality;              // Phẩm chất / Số sao gốc (1 - 6 sao)
    int m_strengthLevel;        // Cấp cường hóa sao (StarLevel / Strength)
    int m_reincarnationLevel;   // Cấp trùng sinh (NewLife 0 - 5)
    int m_tupoLevel;            // Cấp đột phá (Tupo 0 - 10)

    // Điểm tiềm năng tu luyện (Đan dược / Thuộc tính cộng thêm)
    int m_attackAdd;            // Tiềm năng Công cộng thêm
    int m_defenseAdd;           // Tiềm năng Thủ cộng thêm
    int m_chakraAdd;            // Tiềm năng Chakra cộng thêm

    // Chỉ số chiến đấu tính toán (Base & Current)
    int m_attackMin;
    int m_attackMax;
    int m_defenseMin;
    int m_defenseMax;
    int m_chakraMin;
    int m_chakraMax;
    int m_hp;

public:
    CPlayerNinja();
    virtual ~CPlayerNinja();

    static CPlayerNinja* create();
    static CPlayerNinja* create(int seq, int ninjaId, int level, int quality);
    static CPlayerNinja* createWithXmlSnippet(const std::string& xmlSnippet);

    // Khởi tạo và phân tích từ đoạn XML thẻ <card>
    bool initFromXmlSnippet(const std::string& xmlSnippet);

    // Tính toán lại toàn bộ chỉ số thực chiến theo cấp độ, sao và tiềm năng
    void calculateStats();

    // ========================================================
    // GETTERS & SETTERS (Tương thích chuẩn hàm native firefly_...)
    // ========================================================
    int firefly_GetDataID() const { return m_ninjaId; }
    void firefly_SetDataID(int id) { m_ninjaId = id; }
    int getNinjaId() const { return m_ninjaId; }

    int getSeq() const { return m_seq; }
    void setSeq(int seq) { m_seq = seq; }

    int firefly_GetLevel() const { return m_level; }
    void firefly_SetLevel(int lvl) { m_level = lvl; calculateStats(); }

    int firefly_GetCurrExp() const { return m_currExp; }
    void firefly_SetExp(int exp) { m_currExp = exp; }
    void firefly_AddExp(int exp) { m_currExp += exp; }

    int firefly_GetQuality() const { return m_quality; }
    void firefly_SetQuality(int q) { m_quality = q; }

    int firefly_GetStrengthLevel() const { return m_strengthLevel; }
    void firefly_SetStrengthLevel(int sl) { m_strengthLevel = sl; calculateStats(); }

    int firefly_GetReincarnationLevel() const { return m_reincarnationLevel; }
    void firefly_SetReincarnationLevel(int rl) { m_reincarnationLevel = rl; calculateStats(); }

    int GetTupoLevel() const { return m_tupoLevel; }
    void SetTupoLevel(int tl) { m_tupoLevel = tl; calculateStats(); }

    const std::string& firefly_GetName() const { return m_name; }
    void firefly_SetName(const std::string& name) { m_name = name; }

    const std::string& firefly_GetCardIcon() const { return m_icon; }
    void firefly_SetCardIcon(const std::string& icon) { m_icon = icon; }

    const std::string& firefly_GetDesc() const { return m_desc; }

    int firefly_GetAttackMin() const { return m_attackMin; }
    int firefly_GetAttackMax() const { return m_attackMax; }
    int firefly_GetDefenseMin() const { return m_defenseMin; }
    int firefly_GetDefenseMax() const { return m_defenseMax; }
    int firefly_GetChakraMin() const { return m_chakraMin; }
    int firefly_GetChakraMax() const { return m_chakraMax; }
    int firefly_GetHp() const { return m_hp; }

    int getAttackAdd() const { return m_attackAdd; }
    int getDefenseAdd() const { return m_defenseAdd; }
    int getChakraAdd() const { return m_chakraAdd; }
    void setTupoAttrAdd(int atkAdd, int defAdd, int chaAdd);

    // Tính tổng chiến lực cá nhân của Ninja (Combat Warpower)
    int getWarPower() const;

    // Lấy chuỗi đường dẫn hình ảnh sprite avatar thẻ bài
    std::string getPortraitPath() const;
    std::string getIconPath() const;
};

#endif // _CPLAYER_NINJA_H_
