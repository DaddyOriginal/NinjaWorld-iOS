#include "CPlayerNinja.h"
#include "CNinjaTableMgr.h"
#include "CRLRequest.h"
#include <sstream>
#include <cstdlib>

static std::string extractAttr(const std::string& xml, const std::string& attr) {
    std::string pattern = attr + "=\"";
    size_t start = xml.find(pattern);
    if (start == std::string::npos) {
        pattern = attr + "='";
        start = xml.find(pattern);
        if (start == std::string::npos) return "";
    }
    start += pattern.length();
    char quoteChar = pattern[pattern.length() - 1];
    size_t end = xml.find(quoteChar, start);
    if (end == std::string::npos) return "";
    return xml.substr(start, end - start);
}

CPlayerNinja::CPlayerNinja()
    : m_seq(0)
    , m_ninjaId(0)
    , m_name("")
    , m_icon("")
    , m_desc("")
    , m_level(1)
    , m_currExp(0)
    , m_quality(1)
    , m_strengthLevel(0)
    , m_reincarnationLevel(0)
    , m_tupoLevel(0)
    , m_attackAdd(0)
    , m_defenseAdd(0)
    , m_chakraAdd(0)
    , m_attackMin(0)
    , m_attackMax(0)
    , m_defenseMin(0)
    , m_defenseMax(0)
    , m_chakraMin(0)
    , m_chakraMax(0)
    , m_hp(0)
{
}

CPlayerNinja::~CPlayerNinja() {
}

CPlayerNinja* CPlayerNinja::create() {
    CPlayerNinja* pRet = new CPlayerNinja();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CPlayerNinja* CPlayerNinja::create(int seq, int ninjaId, int level, int quality) {
    CPlayerNinja* pRet = new CPlayerNinja();
    if (pRet) {
        pRet->m_seq = seq;
        pRet->m_ninjaId = ninjaId;
        pRet->m_level = level;
        pRet->m_quality = quality;
        pRet->calculateStats();
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CPlayerNinja* CPlayerNinja::createWithXmlSnippet(const std::string& xmlSnippet) {
    CPlayerNinja* pRet = new CPlayerNinja();
    if (pRet && pRet->initFromXmlSnippet(xmlSnippet)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CPlayerNinja::initFromXmlSnippet(const std::string& xml) {
    if (xml.empty()) return false;

    // Hỗ trợ cả 2 định dạng XML từ máy chủ:
    // Định dạng 1: Dạng thẻ con (<type>1</type><id>5</id><seq>1</seq>...)
    // Định dạng 2: Dạng thuộc tính (<card id="1" card_id="5" level="1" star="5" pos="1" exp="0" />)

    std::string typeStr = extractTag(xml, "type");
    if (typeStr.empty()) typeStr = extractAttr(xml, "type");
    if (!typeStr.empty() && atoi(typeStr.c_str()) != 1) {
        // Chỉ nhận thẻ bài loại 1 (Nhẫn Giả)
        return false;
    }

    // 1. Mã phiên bản sở hữu của người chơi (Runtime Sequence)
    std::string seqStr = extractTag(xml, "seq");
    if (seqStr.empty()) seqStr = extractAttr(xml, "id");
    if (!seqStr.empty()) m_seq = atoi(seqStr.c_str());

    // 2. Mã cấu hình Nhẫn Giả gốc (Template ID)
    std::string idStr = extractTag(xml, "id");
    if (idStr.empty() || (!seqStr.empty() && idStr == seqStr)) {
        std::string cardIdAttr = extractAttr(xml, "card_id");
        if (!cardIdAttr.empty()) idStr = cardIdAttr;
    }
    if (!idStr.empty()) m_ninjaId = atoi(idStr.c_str());

    // 3. Cấp độ
    std::string lvlStr = extractTag(xml, "level");
    if (lvlStr.empty()) lvlStr = extractAttr(xml, "level");
    if (!lvlStr.empty()) m_level = atoi(lvlStr.c_str());

    // 4. Kinh nghiệm
    std::string expStr = extractTag(xml, "exp");
    if (expStr.empty()) expStr = extractAttr(xml, "exp");
    if (!expStr.empty()) m_currExp = atoi(expStr.c_str());

    // 5. Cường hóa sao (StarLevel / Strength)
    std::string starStr = extractTag(xml, "starlevel");
    if (starStr.empty()) starStr = extractAttr(xml, "star");
    if (!starStr.empty()) m_strengthLevel = atoi(starStr.c_str());

    // 6. Trùng sinh (NewLife)
    std::string newlifeStr = extractTag(xml, "newlife");
    if (newlifeStr.empty()) newlifeStr = extractAttr(xml, "newlife");
    if (!newlifeStr.empty()) m_reincarnationLevel = atoi(newlifeStr.c_str());

    // 7. Đột phá (Tupo)
    std::string tupoStr = extractTag(xml, "tupo_level");
    if (tupoStr.empty()) tupoStr = extractAttr(xml, "tupo_level");
    if (!tupoStr.empty()) m_tupoLevel = atoi(tupoStr.c_str());

    // 8. Điểm tiềm năng tu luyện
    std::string atkAddStr = extractTag(xml, "attackadd");
    if (atkAddStr.empty()) atkAddStr = extractTag(xml, "attack_add");
    if (!atkAddStr.empty()) m_attackAdd = atoi(atkAddStr.c_str());

    std::string defAddStr = extractTag(xml, "defenseadd");
    if (defAddStr.empty()) defAddStr = extractTag(xml, "defence_add");
    if (!defAddStr.empty()) m_defenseAdd = atoi(defAddStr.c_str());

    std::string chaAddStr = extractTag(xml, "cha_add");
    if (!chaAddStr.empty()) m_chakraAdd = atoi(chaAddStr.c_str());

    // 9. Nạp thông tin mẫu từ Bảng Dữ Liệu Gốc (CNinjaTableMgr - 2,598 Nhẫn Giả)
    const NinjaTableEntry* pEntry = CNinjaTableMgr::sharedManager()->getNinjaEntry(m_ninjaId);
    if (pEntry) {
        m_name = pEntry->name;
        m_icon = pEntry->icon;
        m_quality = pEntry->star;
        m_desc = pEntry->desc;
    } else {
        std::stringstream ss;
        ss << "Nhẫn Giả #" << m_ninjaId;
        m_name = ss.str();
        m_icon = "npc30_6";
        m_quality = 4;
        m_desc = "";
    }

    // 10. Nếu XML có sẵn chỉ số chiến đấu đã tính từ máy chủ, lấy trực tiếp
    std::string atLowStr = extractTag(xml, "attacklow");
    std::string atHighStr = extractTag(xml, "attackhigh");
    std::string dfLowStr = extractTag(xml, "defenselow");
    std::string dfHighStr = extractTag(xml, "defensehigh");
    std::string chLowStr = extractTag(xml, "chakralow");
    std::string chHighStr = extractTag(xml, "chakrahigh");

    if (!atLowStr.empty() && !atHighStr.empty()) {
        m_attackMin = atoi(atLowStr.c_str());
        m_attackMax = atoi(atHighStr.c_str());
        m_defenseMin = atoi(dfLowStr.c_str());
        m_defenseMax = atoi(dfHighStr.c_str());
        m_chakraMin = atoi(chLowStr.c_str());
        m_chakraMax = atoi(chHighStr.c_str());
        m_hp = m_attackMax * 2 + m_defenseMax * 3;
    } else {
        calculateStats();
    }

    return true;
}

void CPlayerNinja::calculateStats() {
    const NinjaTableEntry* pEntry = CNinjaTableMgr::sharedManager()->getNinjaEntry(m_ninjaId);
    if (!pEntry) return;

    int lvlOffset = (m_level > 1) ? (m_level - 1) : 0;
    float strengthRatio = 1.0f + (m_strengthLevel * 0.05f) + (m_reincarnationLevel * 0.15f);

    m_attackMin = (int)((pEntry->atkMin + lvlOffset * pEntry->atkUpgMin + m_attackAdd) * strengthRatio);
    m_attackMax = (int)((pEntry->atkMax + lvlOffset * pEntry->atkUpgMax + m_attackAdd) * strengthRatio);

    m_defenseMin = (int)((pEntry->defMin + lvlOffset * pEntry->defUpgMin + m_defenseAdd) * strengthRatio);
    m_defenseMax = (int)((pEntry->defMax + lvlOffset * pEntry->defUpgMax + m_defenseAdd) * strengthRatio);

    m_chakraMin = (int)((pEntry->chaMin + lvlOffset * pEntry->chaUpgMin + m_chakraAdd) * strengthRatio);
    m_chakraMax = (int)((pEntry->chaMax + lvlOffset * pEntry->chaUpgMax + m_chakraAdd) * strengthRatio);

    m_hp = m_attackMax * 2 + m_defenseMax * 3;
}

void CPlayerNinja::setTupoAttrAdd(int atkAdd, int defAdd, int chaAdd) {
    m_attackAdd = atkAdd;
    m_defenseAdd = defAdd;
    m_chakraAdd = chaAdd;
    calculateStats();
}

int CPlayerNinja::getWarPower() const {
    return (int)(m_hp * 0.5f + ((m_attackMin + m_attackMax) / 2.0f) * 1.5f + ((m_defenseMin + m_defenseMax) / 2.0f) * 1.2f);
}

std::string CPlayerNinja::getPortraitPath() const {
    if (m_icon.empty()) return "0V.png";
    return m_icon + ".png";
}

std::string CPlayerNinja::getIconPath() const {
    if (m_icon.empty()) return "0V.png";
    return "icon_" + m_icon + ".png";
}

CCSprite* CPlayerNinja::createPortraitSprite() const {
    if (m_icon.empty()) return CCSprite::create("0V.png");

    CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* pFrame = pCache->spriteFrameByName(m_icon.c_str());
    if (!pFrame) {
        pFrame = pCache->spriteFrameByName((m_icon + ".png").c_str());
    }

    if (!pFrame) {
        std::string plistPath = "npc/" + m_icon + ".plist";
        pCache->addSpriteFramesWithFile(plistPath.c_str());
        pFrame = pCache->spriteFrameByName(m_icon.c_str());
        if (!pFrame) {
            pFrame = pCache->spriteFrameByName((m_icon + ".png").c_str());
        }
    }

    if (pFrame) {
        return CCSprite::createWithSpriteFrame(pFrame);
    }

    CCSprite* pDirect = CCSprite::create((m_icon + ".png").c_str());
    if (pDirect) return pDirect;

    return CCSprite::create("0V.png");
}

CCSprite* CPlayerNinja::createIconSprite() const {
    if (m_icon.empty()) return CCSprite::create("0V.png");

    std::string iconKey = "icon_" + m_icon;
    CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* pFrame = pCache->spriteFrameByName(iconKey.c_str());
    if (!pFrame) {
        pFrame = pCache->spriteFrameByName((iconKey + ".png").c_str());
    }

    if (!pFrame) {
        std::string plistPath = "icon/" + iconKey + ".plist";
        pCache->addSpriteFramesWithFile(plistPath.c_str());
        pFrame = pCache->spriteFrameByName(iconKey.c_str());
        if (!pFrame) {
            pFrame = pCache->spriteFrameByName((iconKey + ".png").c_str());
        }
    }

    if (pFrame) {
        return CCSprite::createWithSpriteFrame(pFrame);
    }

    CCSprite* pPortrait = createPortraitSprite();
    if (pPortrait) return pPortrait;

    return CCSprite::create("0V.png");
}

