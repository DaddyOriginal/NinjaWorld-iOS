#ifndef _CGAME_CARD_MARK_H_
#define _CGAME_CARD_MARK_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

/**
 * CGameCardMark: Thẻ Ấn Ký Vĩ Thú của người chơi
 * Ánh xạ trực tiếp từ thẻ <card type="5"> của máy chủ gửi về.
 */
class CGameCardMark : public CCObject {
private:
    int m_seq;                  // Runtime Sequence ID
    int m_markId;               // Template ID (1 -> 91 trong markinfo.bin)
    int m_level;                // Cấp độ
    int m_quality;              // Phẩm chất
    int m_equippedNinjaSeq;     // 0 nếu trong túi, > 0 nếu đang được trang bị

    float m_atkRate;
    float m_defRate;
    float m_chaRate;

    std::string m_name;
    std::string m_icon;
    std::string m_desc;

public:
    CGameCardMark();
    virtual ~CGameCardMark();

    static CGameCardMark* create();
    static CGameCardMark* createWithXmlSnippet(const std::string& xmlSnippet);

    bool initFromXmlSnippet(const std::string& xmlSnippet);

    int getSeq() const { return m_seq; }
    void setSeq(int seq) { m_seq = seq; }

    int getMarkId() const { return m_markId; }
    int getLevel() const { return m_level; }
    void setLevel(int lvl) { m_level = lvl; }

    int getQuality() const { return m_quality; }

    int getEquippedNinjaSeq() const { return m_equippedNinjaSeq; }
    void setEquippedNinjaSeq(int nSeq) { m_equippedNinjaSeq = nSeq; }
    bool isEquipped() const { return m_equippedNinjaSeq > 0; }

    float getAtkRate() const { return m_atkRate; }
    float getDefRate() const { return m_defRate; }
    float getChaRate() const { return m_chaRate; }

    const std::string& getName() const { return m_name; }
    const std::string& getIcon() const { return m_icon; }
    const std::string& getDesc() const { return m_desc; }

    std::string getIconPath() const;
};

#endif // _CGAME_CARD_MARK_H_
