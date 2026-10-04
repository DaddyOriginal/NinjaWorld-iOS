#include "CGameCardMark.h"
#include "CItemTableMgr.h"
#include "CRLRequest.h"
#include <sstream>
#include <cstdlib>

CGameCardMark::CGameCardMark()
    : m_seq(0)
    , m_markId(0)
    , m_level(1)
    , m_quality(1)
    , m_equippedNinjaSeq(0)
    , m_atkRate(0.0f)
    , m_defRate(0.0f)
    , m_chaRate(0.0f)
    , m_name("")
    , m_icon("")
    , m_desc("")
{
}

CGameCardMark::~CGameCardMark() {
}

CGameCardMark* CGameCardMark::create() {
    CGameCardMark* pRet = new CGameCardMark();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CGameCardMark* CGameCardMark::createWithXmlSnippet(const std::string& xmlSnippet) {
    CGameCardMark* pRet = new CGameCardMark();
    if (pRet && pRet->initFromXmlSnippet(xmlSnippet)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CGameCardMark::initFromXmlSnippet(const std::string& xml) {
    if (xml.empty()) return false;

    std::string typeStr = extractTag(xml, "type");
    if (!typeStr.empty() && atoi(typeStr.c_str()) != 5) {
        return false; // Chỉ nhận Type 5 (Ấn ký)
    }

    std::string seqStr = extractTag(xml, "seq");
    if (!seqStr.empty()) m_seq = atoi(seqStr.c_str());

    std::string idStr = extractTag(xml, "id");
    if (!idStr.empty()) m_markId = atoi(idStr.c_str());

    std::string lvlStr = extractTag(xml, "level");
    if (!lvlStr.empty()) m_level = atoi(lvlStr.c_str());

    // Nạp thông tin mẫu từ CItemTableMgr
    const MarkTableEntry* pEntry = CItemTableMgr::sharedManager()->getMarkEntry(m_markId);
    if (pEntry) {
        m_name = pEntry->name;
        m_icon = pEntry->icon;
        m_quality = pEntry->quality;
        m_atkRate = pEntry->atkRate;
        m_defRate = pEntry->defRate;
        m_chaRate = pEntry->chaRate;
        m_desc = pEntry->desc;
    } else {
        std::stringstream ss;
        ss << "Ấn Ký #" << m_markId;
        m_name = ss.str();
        m_icon = "hallmark_1";
        m_desc = "";
    }

    return true;
}

std::string CGameCardMark::getIconPath() const {
    return "ui/icon/" + m_icon + ".png";
}
