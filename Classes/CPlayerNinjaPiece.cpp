#include "CPlayerNinjaPiece.h"
#include "CItemTableMgr.h"
#include <sstream>

CPlayerNinjaPiece::CPlayerNinjaPiece()
    : m_pieceId(0)
    , m_count(0)
    , m_reqCount(30)
    , m_type(1)
    , m_star(4)
    , m_targetId(0)
    , m_name("")
{
}

CPlayerNinjaPiece::~CPlayerNinjaPiece() {
}

CPlayerNinjaPiece* CPlayerNinjaPiece::create() {
    CPlayerNinjaPiece* pRet = new CPlayerNinjaPiece();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CPlayerNinjaPiece* CPlayerNinjaPiece::createWithIdAndCount(int pieceId, int count) {
    CPlayerNinjaPiece* pRet = new CPlayerNinjaPiece();
    if (pRet && pRet->initWithIdAndCount(pieceId, count)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CPlayerNinjaPiece::initWithIdAndCount(int pieceId, int count) {
    m_pieceId = pieceId;
    m_count = count;

    // Tra cứu thông tin từ CItemTableMgr (piece_info.bin)
    const PieceTableEntry* pEntry = CItemTableMgr::sharedManager()->getPieceEntry(pieceId);
    if (pEntry) {
        m_name = pEntry->name;
        m_type = pEntry->type;
        m_star = pEntry->star;
        m_targetId = pEntry->targetId;
        m_reqCount = pEntry->requireCount > 0 ? pEntry->requireCount : 30;
    } else {
        std::stringstream ss;
        ss << "Mảnh Ghép #" << pieceId;
        m_name = ss.str();
        m_type = 1;
        m_star = 4;
        m_targetId = pieceId;
        m_reqCount = 30;
    }

    return true;
}

std::string CPlayerNinjaPiece::getIconPath() const {
    std::stringstream ss;
    ss << "ui/icon/piece_" << m_pieceId << ".png";
    return ss.str();
}
