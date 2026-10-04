#include "CTeamCard.h"
#include "CRLRequest.h"
#include <cstdlib>

CTeamCard::CTeamCard()
    : m_seq(1)
    , m_ninjaInstanceSeq(0)
    , m_pNinja(NULL)
    , m_weaponId(0)
    , m_armorId(0)
    , m_decoratorId(0)
    , m_markId(0)
    , m_ninjutsu1(0)
    , m_ninjutsu2(0)
    , m_ninjutsu3(0)
    , m_ninjutsu4(0)
    , m_attackLow(0)
    , m_attackHigh(0)
    , m_defenselow(0)
    , m_defenseHigh(0)
    , m_chakraLow(0)
    , m_chakraHigh(0)
{
}

CTeamCard::~CTeamCard() {
    CC_SAFE_RELEASE_NULL(m_pNinja);
}

CTeamCard* CTeamCard::create() {
    CTeamCard* pRet = new CTeamCard();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CTeamCard* CTeamCard::createWithXmlSnippet(const std::string& xmlSnippet) {
    CTeamCard* pRet = new CTeamCard();
    if (pRet && pRet->initFromXmlSnippet(xmlSnippet)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CTeamCard::initFromXmlSnippet(const std::string& xml) {
    if (xml.empty()) return false;

    std::string seqStr = extractTag(xml, "seq");
    if (!seqStr.empty()) m_seq = atoi(seqStr.c_str());

    std::string nIdStr = extractTag(xml, "ninjaid");
    if (!nIdStr.empty()) m_ninjaInstanceSeq = atoi(nIdStr.c_str());

    std::string wStr = extractTag(xml, "weaponid");
    if (!wStr.empty()) m_weaponId = atoi(wStr.c_str());

    std::string aStr = extractTag(xml, "armorid");
    if (!aStr.empty()) m_armorId = atoi(aStr.c_str());

    std::string dStr = extractTag(xml, "decoratorid");
    if (!dStr.empty()) m_decoratorId = atoi(dStr.c_str());

    std::string mStr = extractTag(xml, "markid");
    if (!mStr.empty()) m_markId = atoi(mStr.c_str());

    std::string n1 = extractTag(xml, "ninjintsu1seq");
    if (!n1.empty()) m_ninjutsu1 = atoi(n1.c_str());

    std::string n2 = extractTag(xml, "ninjintsu2seq");
    if (!n2.empty()) m_ninjutsu2 = atoi(n2.c_str());

    std::string n3 = extractTag(xml, "ninjintsu3seq");
    if (!n3.empty()) m_ninjutsu3 = atoi(n3.c_str());

    std::string n4 = extractTag(xml, "ninjintsu4seq");
    if (!n4.empty()) m_ninjutsu4 = atoi(n4.c_str());

    std::string atLow = extractTag(xml, "attacklow");
    if (!atLow.empty()) m_attackLow = atoi(atLow.c_str());

    std::string atHigh = extractTag(xml, "attackhigh");
    if (!atHigh.empty()) m_attackHigh = atoi(atHigh.c_str());

    std::string dfLow = extractTag(xml, "defenselow");
    if (!dfLow.empty()) m_defenselow = atoi(dfLow.c_str());

    std::string dfHigh = extractTag(xml, "defensehigh");
    if (!dfHigh.empty()) m_defenseHigh = atoi(dfHigh.c_str());

    std::string chLow = extractTag(xml, "chakralow");
    if (!chLow.empty()) m_chakraLow = atoi(chLow.c_str());

    std::string chHigh = extractTag(xml, "chakrahigh");
    if (!chHigh.empty()) m_chakraHigh = atoi(chHigh.c_str());

    // Trích xuất danh sách Duyên phận (combinlist)
    m_combinList.clear();
    size_t pos = 0;
    while (true) {
        size_t cStart = xml.find("<combinid>", pos);
        if (cStart == std::string::npos) break;
        cStart += 10;
        size_t cEnd = xml.find("</combinid>", cStart);
        if (cEnd == std::string::npos) break;
        int cid = atoi(xml.substr(cStart, cEnd - cStart).c_str());
        if (cid > 0) m_combinList.push_back(cid);
        pos = cEnd + 11;
    }

    return true;
}

void CTeamCard::bindNinja(CPlayerNinja* pNinja) {
    if (m_pNinja != pNinja) {
        CC_SAFE_RELEASE(m_pNinja);
        m_pNinja = pNinja;
        CC_SAFE_RETAIN(m_pNinja);

        if (m_pNinja) {
            m_ninjaInstanceSeq = m_pNinja->getSeq();
            // Nếu chỉ số server chưa gửi về, lấy trực tiếp từ thẻ Ninja
            if (m_attackLow == 0) m_attackLow = m_pNinja->firefly_GetAttackMin();
            if (m_attackHigh == 0) m_attackHigh = m_pNinja->firefly_GetAttackMax();
            if (m_defenselow == 0) m_defenselow = m_pNinja->firefly_GetDefenseMin();
            if (m_defenseHigh == 0) m_defenseHigh = m_pNinja->firefly_GetDefenseMax();
            if (m_chakraLow == 0) m_chakraLow = m_pNinja->firefly_GetChakraMin();
            if (m_chakraHigh == 0) m_chakraHigh = m_pNinja->firefly_GetChakraMax();
        }
    }
}

int CTeamCard::getEquipId(int slot) const {
    switch (slot) {
        case 0: return m_weaponId;
        case 1: return m_armorId;
        case 2: return m_decoratorId;
        case 3: return m_markId;
        default: return 0;
    }
}

void CTeamCard::setEquipId(int slot, int equipId) {
    switch (slot) {
        case 0: m_weaponId = equipId; break;
        case 1: m_armorId = equipId; break;
        case 2: m_decoratorId = equipId; break;
        case 3: m_markId = equipId; break;
        default: break;
    }
}

int CTeamCard::firefly_GetNinjutsu(int slot) const {
    switch (slot) {
        case 0: return m_ninjutsu1;
        case 1: return m_ninjutsu2;
        case 2: return m_ninjutsu3;
        case 3: return m_ninjutsu4;
        default: return 0;
    }
}
