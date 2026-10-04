#include "CGameCardEquipment.h"
#include "CItemTableMgr.h"
#include <sstream>
#include <cstdlib>

static std::string extractEquipTag(const std::string& xml, const std::string& tag) {
    std::string openTag = "<" + tag + ">";
    std::string closeTag = "</" + tag + ">";
    size_t start = xml.find(openTag);
    if (start == std::string::npos) return "";
    start += openTag.length();
    size_t end = xml.find(closeTag, start);
    if (end == std::string::npos) return "";
    return xml.substr(start, end - start);
}

CGameCardEquipment::CGameCardEquipment()
    : m_seq(0)
    , m_equipId(0)
    , m_level(1)
    , m_star(1)
    , m_slotType(1)
    , m_equippedNinjaSeq(0)
    , m_attack(0)
    , m_defense(0)
    , m_chakra(0)
    , m_name("")
    , m_icon("")
    , m_desc("")
{
}

CGameCardEquipment::~CGameCardEquipment() {
}

CGameCardEquipment* CGameCardEquipment::create() {
    CGameCardEquipment* pRet = new CGameCardEquipment();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CGameCardEquipment* CGameCardEquipment::createWithXmlSnippet(const std::string& xmlSnippet) {
    CGameCardEquipment* pRet = new CGameCardEquipment();
    if (pRet && pRet->initFromXmlSnippet(xmlSnippet)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CGameCardEquipment::initFromXmlSnippet(const std::string& xml) {
    if (xml.empty()) return false;

    std::string typeStr = extractEquipTag(xml, "type");
    int cardType = !typeStr.empty() ? atoi(typeStr.c_str()) : 2;
    if (cardType < 2 || cardType > 4) {
        return false; // Chỉ nhận Type 2 (Vũ khí), 3 (Giáp), 4 (Trang sức)
    }

    // Gán slotType: 1 = Vũ khí, 2 = Giáp, 3 = Trang sức
    m_slotType = cardType - 1;

    std::string seqStr = extractEquipTag(xml, "seq");
    if (!seqStr.empty()) m_seq = atoi(seqStr.c_str());

    std::string idStr = extractEquipTag(xml, "id");
    if (!idStr.empty()) m_equipId = atoi(idStr.c_str());

    std::string lvlStr = extractEquipTag(xml, "level");
    if (!lvlStr.empty()) m_level = atoi(lvlStr.c_str());

    std::string starStr = extractEquipTag(xml, "starlevel");
    if (!starStr.empty()) m_star = atoi(starStr.c_str());

    // Nạp thông tin mẫu từ CItemTableMgr
    const EquipTableEntry* pEntry = CItemTableMgr::sharedManager()->getEquipEntry(m_equipId);
    if (pEntry) {
        m_name = pEntry->name;
        m_icon = pEntry->icon;
        if (m_star <= 1) m_star = pEntry->star;
        m_slotType = pEntry->getSlotType();
        m_desc = pEntry->desc;
    } else {
        std::stringstream ss;
        ss << "Trang Bị #" << m_equipId;
        m_name = ss.str();
        m_icon = "weapon_001";
        m_desc = "";
    }

    calculateStats();
    return true;
}

void CGameCardEquipment::calculateStats() {
    const EquipTableEntry* pEntry = CItemTableMgr::sharedManager()->getEquipEntry(m_equipId);
    if (!pEntry) return;

    int lvlOffset = (m_level > 1) ? (m_level - 1) : 0;
    m_attack = pEntry->atkLow + lvlOffset * pEntry->atkGrowth;
    m_defense = pEntry->defLow + lvlOffset * pEntry->defGrowth;
    m_chakra = pEntry->chaLow + lvlOffset * pEntry->chaGrowth;
}

std::string CGameCardEquipment::getIconPath() const {
    return "ui/icon/" + m_icon + ".png";
}
