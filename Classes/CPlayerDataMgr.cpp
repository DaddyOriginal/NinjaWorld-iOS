#include "CPlayerDataMgr.h"
#include <sstream>
#include <cstdlib>

CPlayerDataMgr* CPlayerDataMgr::s_instance = NULL;

CPlayerDataMgr::CPlayerDataMgr()
    : m_userId(0)
    , m_username("")
    , m_nickname("")
    , m_sessionToken("")
    , m_serverId(0)
    , m_serverName("")
    , m_level(1)
    , m_exp(0)
    , m_maxExp(100)
    , m_gold(0)
    , m_silver(0)
    , m_bodyValue(120)
    , m_maxBodyValue(120)
    , m_vipLevel(0)
    , m_combatPower(0)
    , m_countryType(1)
    , m_avatarId(5)
    , m_pActiveTeam(NULL)
{
    m_pActiveTeam = CActiveTeamMgr::create();
    m_pActiveTeam->retain();
}

CPlayerDataMgr::~CPlayerDataMgr() {
    for (size_t i = 0; i < m_ninjas.size(); ++i) CC_SAFE_RELEASE(m_ninjas[i]);
    m_ninjas.clear();
    for (size_t i = 0; i < m_equipments.size(); ++i) CC_SAFE_RELEASE(m_equipments[i]);
    m_equipments.clear();
    for (size_t i = 0; i < m_pieces.size(); ++i) CC_SAFE_RELEASE(m_pieces[i]);
    m_pieces.clear();
    for (size_t i = 0; i < m_marks.size(); ++i) CC_SAFE_RELEASE(m_marks[i]);
    m_marks.clear();
    CC_SAFE_RELEASE_NULL(m_pActiveTeam);
}

CPlayerDataMgr* CPlayerDataMgr::sharedManager() {
    if (!s_instance) {
        s_instance = new CPlayerDataMgr();
    }
    return s_instance;
}

void CPlayerDataMgr::purge() {
    CC_SAFE_DELETE(s_instance);
}

static std::string extractTagValue(const std::string& xml, const std::string& tag) {
    std::string openTag = "<" + tag + ">";
    std::string closeTag = "</" + tag + ">";
    size_t start = xml.find(openTag);
    if (start == std::string::npos) return "";
    start += openTag.length();
    size_t end = xml.find(closeTag, start);
    if (end == std::string::npos) return "";
    return xml.substr(start, end - start);
}

bool CPlayerDataMgr::parseLoginXml(const std::string& xmlData) {
    if (xmlData.empty()) return false;

    std::string retStr = extractTagValue(xmlData, "ret");
    if (!retStr.empty() && retStr != "0") {
        CCLog("[CPlayerDataMgr] Server bao loi: ret=%s", retStr.c_str());
        return false;
    }

    std::string uidStr = extractTagValue(xmlData, "uid");
    if (uidStr.empty()) uidStr = extractTagValue(xmlData, "userid");
    if (!uidStr.empty()) m_userId = atoi(uidStr.c_str());

    std::string session = extractTagValue(xmlData, "session");
    if (!session.empty()) m_sessionToken = session;

    std::string nick = extractTagValue(xmlData, "nick");
    if (nick.empty()) nick = extractTagValue(xmlData, "username");
    if (!nick.empty()) {
        m_nickname = nick;
        if (m_username.empty()) m_username = nick;
    }

    std::string lvlStr = extractTagValue(xmlData, "level");
    if (!lvlStr.empty()) m_level = atoi(lvlStr.c_str());

    std::string expStr = extractTagValue(xmlData, "curexp");
    if (expStr.empty()) expStr = extractTagValue(xmlData, "exp");
    if (!expStr.empty()) m_exp = atoi(expStr.c_str());

    std::string goldStr = extractTagValue(xmlData, "cash");
    if (goldStr.empty()) goldStr = extractTagValue(xmlData, "gold");
    if (!goldStr.empty()) m_gold = atoi(goldStr.c_str());

    std::string silverStr = extractTagValue(xmlData, "coin");
    if (silverStr.empty()) silverStr = extractTagValue(xmlData, "silver");
    if (!silverStr.empty()) m_silver = atoi(silverStr.c_str());

    std::string bodyStr = extractTagValue(xmlData, "curaction");
    if (bodyStr.empty()) bodyStr = extractTagValue(xmlData, "body");
    if (!bodyStr.empty()) m_bodyValue = atoi(bodyStr.c_str());

    std::string vipStr = extractTagValue(xmlData, "viplevel");
    if (!vipStr.empty()) m_vipLevel = atoi(vipStr.c_str());

    std::string countryStr = extractTagValue(xmlData, "country");
    if (!countryStr.empty()) m_countryType = atoi(countryStr.c_str());

    std::string warStr = extractTagValue(xmlData, "warpower");
    if (!warStr.empty()) m_combatPower = atoi(warStr.c_str());

    // 1. Quét và phân tích danh sách thẻ bài <card> (Nhẫn Giả, Trang Bị, Ấn Ký)
    size_t pos = 0;
    while (true) {
        size_t cStart = xmlData.find("<card", pos);
        if (cStart == std::string::npos) break;
        
        size_t cCloseTag = xmlData.find("</card>", cStart);
        size_t cSelfClose = xmlData.find("/>", cStart);
        size_t cEnd = std::string::npos;

        if (cCloseTag != std::string::npos && (cSelfClose == std::string::npos || cCloseTag < cSelfClose)) {
            cEnd = cCloseTag + 7;
        } else if (cSelfClose != std::string::npos) {
            cEnd = cSelfClose + 2;
        } else {
            break;
        }

        std::string cardChunk = xmlData.substr(cStart, cEnd - cStart);
        std::string typeStr = extractTagValue(cardChunk, "type");
        int cardType = !typeStr.empty() ? atoi(typeStr.c_str()) : 1;

        if (cardType == 1) {
            // Thẻ Nhẫn Giả
            CPlayerNinja* pNinja = CPlayerNinja::createWithXmlSnippet(cardChunk);
            if (pNinja && pNinja->firefly_GetDataID() > 0) {
                addOrUpdateNinja(pNinja);
            }
        } else if (cardType >= 2 && cardType <= 4) {
            // Thẻ Trang Bị (Vũ khí, Giáp, Trang sức)
            CGameCardEquipment* pEquip = CGameCardEquipment::createWithXmlSnippet(cardChunk);
            if (pEquip && pEquip->getEquipId() > 0) {
                pEquip->retain();
                m_equipments.push_back(pEquip);
            }
        } else if (cardType == 5) {
            // Thẻ Ấn Ký Vĩ Thú
            CGameCardMark* pMark = CGameCardMark::createWithXmlSnippet(cardChunk);
            if (pMark && pMark->getMarkId() > 0) {
                pMark->retain();
                m_marks.push_back(pMark);
            }
        }
        pos = cEnd;
    }

    // 2. Quét danh sách Mảnh Ghép <chip>...</chip>
    pos = 0;
    while (true) {
        size_t chipStart = xmlData.find("<chip>", pos);
        if (chipStart == std::string::npos) break;
        size_t chipEnd = xmlData.find("</chip>", chipStart);
        if (chipEnd == std::string::npos) break;
        chipEnd += 7;

        std::string chipChunk = xmlData.substr(chipStart, chipEnd - chipStart);
        std::string chipIdStr = extractTagValue(chipChunk, "chip_id");
        std::string chipNumStr = extractTagValue(chipChunk, "chip_num");

        if (!chipIdStr.empty() && !chipNumStr.empty()) {
            int cid = atoi(chipIdStr.c_str());
            int num = atoi(chipNumStr.c_str());
            if (cid > 0 && num > 0) {
                CPlayerNinjaPiece* pPiece = CPlayerNinjaPiece::createWithIdAndCount(cid, num);
                if (pPiece) {
                    pPiece->retain();
                    m_pieces.push_back(pPiece);
                }
            }
        }
        pos = chipEnd;
    }

    // 3. Nạp dữ liệu đội hình chiến đấu vào m_pActiveTeam
    if (m_pActiveTeam) {
        std::string activeIdStr = extractTagValue(xmlData, "activeid");
        if (!activeIdStr.empty()) {
            m_pActiveTeam->setActiveTeamIndex(atoi(activeIdStr.c_str()));
        }
        m_pActiveTeam->initFromXml(xmlData);

        if (m_combatPower == 0) {
            m_combatPower = m_pActiveTeam->firefly_GetWarPower();
        }
    }

    // 4. Đảm bảo luôn có ít nhất một Ninja khởi đầu hợp lệ
    if (m_ninjas.empty()) {
        int starterId = (m_avatarId > 0) ? m_avatarId : 56;
        CPlayerNinja* pStarter = CPlayerNinja::create(1, starterId, 1, 5);
        if (pStarter) {
            addOrUpdateNinja(pStarter);
        }
    }

    if (m_pActiveTeam && !m_ninjas.empty()) {
        CTeamCard* pSlot1 = m_pActiveTeam->firefly_GetTeamCardByIndex(0);
        if (pSlot1 && !pSlot1->firefly_HasNinja()) {
            pSlot1->bindNinja(m_ninjas[0]);
        }
        if (m_combatPower == 0) {
            m_pActiveTeam->recalculateTeamStats();
            m_combatPower = m_pActiveTeam->firefly_GetWarPower();
        }
    }

    if (m_nickname.empty()) {
        m_nickname = m_username.empty() ? "Ninja" : m_username;
    }
    if (m_gold == 0) m_gold = 1000;
    if (m_silver == 0) m_silver = 100000;
    if (m_combatPower == 0 && !m_ninjas.empty()) {
        m_combatPower = m_ninjas[0]->getWarPower();
    }

    CCLog("[CPlayerDataMgr] Nap hoan tat: UID=%d, Nick=%s, Level=%d, Vang=%d, Bac=%d, LucChien=%d, Ninja=%d, TrangBi=%d, Manh=%d, AnKy=%d",
          m_userId, m_nickname.c_str(), m_level, m_gold, m_silver, m_combatPower,
          (int)m_ninjas.size(), (int)m_equipments.size(), (int)m_pieces.size(), (int)m_marks.size());
    return true;
}

CPlayerNinja* CPlayerDataMgr::getNinjaBySeq(int seq) {
    for (size_t i = 0; i < m_ninjas.size(); ++i) {
        if (m_ninjas[i]->getSeq() == seq) {
            return m_ninjas[i];
        }
    }
    // Fallback: nếu không tìm thấy theo seq, thử tìm theo ninjaId
    for (size_t i = 0; i < m_ninjas.size(); ++i) {
        if (m_ninjas[i]->firefly_GetDataID() == seq) {
            return m_ninjas[i];
        }
    }
    return NULL;
}

void CPlayerDataMgr::addOrUpdateNinja(CPlayerNinja* pNinja) {
    if (!pNinja) return;

    for (size_t i = 0; i < m_ninjas.size(); ++i) {
        if (m_ninjas[i]->getSeq() == pNinja->getSeq()) {
            // Đã tồn tại -> Cập nhật thông số
            m_ninjas[i]->firefly_SetLevel(pNinja->firefly_GetLevel());
            m_ninjas[i]->firefly_SetStrengthLevel(pNinja->firefly_GetStrengthLevel());
            m_ninjas[i]->firefly_SetReincarnationLevel(pNinja->firefly_GetReincarnationLevel());
            m_ninjas[i]->SetTupoLevel(pNinja->GetTupoLevel());
            return;
        }
    }

    pNinja->retain();
    m_ninjas.push_back(pNinja);
}

void CPlayerDataMgr::removeNinja(int seq) {
    for (std::vector<CPlayerNinja*>::iterator it = m_ninjas.begin(); it != m_ninjas.end(); ++it) {
        if ((*it)->getSeq() == seq) {
            (*it)->release();
            m_ninjas.erase(it);
            break;
        }
    }
}

std::vector<CGameCardEquipment*> CPlayerDataMgr::getEquipmentsBySlot(int slotType) const {
    std::vector<CGameCardEquipment*> result;
    for (size_t i = 0; i < m_equipments.size(); ++i) {
        if (slotType == 0 || m_equipments[i]->getSlotType() == slotType) {
            result.push_back(m_equipments[i]);
        }
    }
    return result;
}

CGameCardEquipment* CPlayerDataMgr::getEquipmentBySeq(int seq) {
    for (size_t i = 0; i < m_equipments.size(); ++i) {
        if (m_equipments[i]->getSeq() == seq) {
            return m_equipments[i];
        }
    }
    return NULL;
}

std::vector<CPlayerNinjaPiece*> CPlayerDataMgr::getPiecesByType(int pieceType) const {
    std::vector<CPlayerNinjaPiece*> result;
    for (size_t i = 0; i < m_pieces.size(); ++i) {
        if (pieceType == 0 || m_pieces[i]->getType() == pieceType) {
            result.push_back(m_pieces[i]);
        }
    }
    return result;
}

CPlayerNinjaPiece* CPlayerDataMgr::getPieceById(int pieceId) {
    for (size_t i = 0; i < m_pieces.size(); ++i) {
        if (m_pieces[i]->getPieceId() == pieceId) {
            return m_pieces[i];
        }
    }
    return NULL;
}

CGameCardMark* CPlayerDataMgr::getMarkBySeq(int seq) {
    for (size_t i = 0; i < m_marks.size(); ++i) {
        if (m_marks[i]->getSeq() == seq) {
            return m_marks[i];
        }
    }
    return NULL;
}

int CPlayerDataMgr::firefly_GetMaxTeamMembers() const {
    // Công thức mở khóa ô chuẩn:
    // Slot 1: Lv 1, Slot 2: Lv 2, Slot 3: Lv 6, Slot 4: Lv 15, Slot 5: Lv 30, Slot 6: Lv 50
    if (m_level >= 50) return 6;
    if (m_level >= 30) return 5;
    if (m_level >= 15) return 4;
    if (m_level >= 6)  return 3;
    if (m_level >= 2)  return 2;
    return 1;
}

int CPlayerDataMgr::firefly_GetPlayerAttack() {
    if (m_pActiveTeam) {
        return m_pActiveTeam->firefly_GetAttack();
    }
    return 0;
}
