#include "CActiveTeamMgr.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CActiveTeamMgr::CActiveTeamMgr()
    : m_activeTeamIndex(0)
    , m_totalAttack(0)
    , m_totalDefense(0)
    , m_totalWarPower(0)
    , m_needReload(false)
{
    // Mặc định khởi tạo sẵn 6 ô vị trí cho đội hình
    for (int i = 1; i <= 6; ++i) {
        CTeamCard* pCard = CTeamCard::create();
        pCard->setSeq(i);
        pCard->retain();
        m_teamCards.push_back(pCard);
    }
}

CActiveTeamMgr::~CActiveTeamMgr() {
    clear();
}

CActiveTeamMgr* CActiveTeamMgr::create() {
    CActiveTeamMgr* pRet = new CActiveTeamMgr();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void CActiveTeamMgr::clear() {
    for (size_t i = 0; i < m_teamCards.size(); ++i) {
        CC_SAFE_RELEASE(m_teamCards[i]);
    }
    m_teamCards.clear();
}

bool CActiveTeamMgr::initFromXml(const std::string& xmlData) {
    if (xmlData.empty()) return false;

    // Xác định tag danh sách tướng theo Team đang kích hoạt:
    // Team 1: <ninjalist>, Team 2: <defenseninjalist>
    std::string listTag = (m_activeTeamIndex == 1) ? "defenseninjalist" : "ninjalist";
    size_t listStart = xmlData.find("<" + listTag + ">");
    if (listStart == std::string::npos) {
        // Fallback về <ninjalist>
        listStart = xmlData.find("<ninjalist>");
        listTag = "ninjalist";
    }

    if (listStart == std::string::npos) {
        CCLog("[CActiveTeamMgr] Khong tim thay the <%s> trong XML", listTag.c_str());
        return false;
    }

    size_t listEnd = xmlData.find("</" + listTag + ">", listStart);
    if (listEnd == std::string::npos) return false;

    std::string listXml = xmlData.substr(listStart, listEnd - listStart);

    // Reset lại 6 vị trí hiện tại
    for (size_t i = 0; i < m_teamCards.size(); ++i) {
        m_teamCards[i]->bindNinja(NULL);
    }

    // Quét từng thẻ <ninja>...</ninja>
    size_t pos = 0;
    while (true) {
        size_t nStart = listXml.find("<ninja>", pos);
        if (nStart == std::string::npos) break;
        size_t nEnd = listXml.find("</ninja>", nStart);
        if (nEnd == std::string::npos) break;
        nEnd += 8;

        std::string ninjaChunk = listXml.substr(nStart, nEnd - nStart);
        CTeamCard* pParsed = CTeamCard::createWithXmlSnippet(ninjaChunk);
        if (pParsed) {
            int seq = pParsed->getSeq();
            if (seq >= 1 && seq <= 6) {
                int slotIdx = seq - 1;
                // Gán thuộc tính vào ô tương ứng
                CTeamCard* pSlot = m_teamCards[slotIdx];
                pSlot->initFromXmlSnippet(ninjaChunk);

                // Liên kết với thẻ CPlayerNinja từ CPlayerDataMgr
                int nInstanceId = pParsed->getNinjaInstanceSeq();
                CPlayerNinja* pNinja = CPlayerDataMgr::sharedManager()->getNinjaBySeq(nInstanceId);
                if (pNinja) {
                    pSlot->bindNinja(pNinja);
                }
            }
        }

        pos = nEnd;
    }

    recalculateTeamStats();
    CCLog("[CActiveTeamMgr] Nap xong doi hinh: So thanh vien=%d, Tong Cong=%d, Tong Thu=%d, Luc chien=%d",
          firefly_GetMemberCount(), m_totalAttack, m_totalDefense, m_totalWarPower);
    return true;
}

CTeamCard* CActiveTeamMgr::firefly_GetTeamCardByIndex(int index) {
    if (index >= 0 && index < (int)m_teamCards.size()) {
        return m_teamCards[index];
    }
    return NULL;
}

int CActiveTeamMgr::firefly_GetMemberCount() const {
    int count = 0;
    for (size_t i = 0; i < m_teamCards.size(); ++i) {
        if (m_teamCards[i]->firefly_HasNinja()) {
            count++;
        }
    }
    return count;
}

void CActiveTeamMgr::recalculateTeamStats() {
    m_totalAttack = 0;
    m_totalDefense = 0;
    m_totalWarPower = 0;

    for (size_t i = 0; i < m_teamCards.size(); ++i) {
        CTeamCard* pCard = m_teamCards[i];
        if (pCard->firefly_HasNinja()) {
            m_totalAttack += pCard->firefly_GetRoundAttack();
            m_totalDefense += pCard->firefly_GetRoundDefense();
            CPlayerNinja* pNinja = pCard->firefly_GetNinja();
            if (pNinja) {
                m_totalWarPower += pNinja->getWarPower();
            }
        }
    }
}
