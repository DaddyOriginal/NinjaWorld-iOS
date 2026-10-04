#ifndef _CPLAYER_NINJA_PIECE_H_
#define _CPLAYER_NINJA_PIECE_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

/**
 * CPlayerNinjaPiece: Mảnh ghép Nhẫn Giả / Trang bị / Bí kíp trong Túi Mảnh
 * Ánh xạ từ thẻ <chip><chip_id>X</chip_id><chip_num>Y</chip_num></chip>
 */
class CPlayerNinjaPiece : public CCObject {
private:
    int m_pieceId;          // Mã định danh mảnh (tra cứu trong piece_info.bin)
    int m_count;            // Số lượng mảnh đang sở hữu
    int m_reqCount;         // Số lượng mảnh yêu cầu để hợp thành
    int m_type;             // 1: Mảnh Ninja, 2: Mảnh Trang bị, 4: Mảnh Nhẫn thuật, 5: Mảnh Linh thú
    int m_star;             // Số sao
    int m_targetId;         // Mã Tướng/Trang bị tạo thành sau khi ghép
    std::string m_name;     // Tên mảnh

public:
    CPlayerNinjaPiece();
    virtual ~CPlayerNinjaPiece();

    static CPlayerNinjaPiece* create();
    static CPlayerNinjaPiece* createWithIdAndCount(int pieceId, int count);

    bool initWithIdAndCount(int pieceId, int count);

    int getPieceId() const { return m_pieceId; }
    int getCount() const { return m_count; }
    void setCount(int c) { m_count = c; }
    void addCount(int c) { m_count += c; }

    int getReqCount() const { return m_reqCount; }
    int getType() const { return m_type; }
    int getStar() const { return m_star; }
    int getTargetId() const { return m_targetId; }
    const std::string& getName() const { return m_name; }

    // Kiểm tra có đủ số lượng để Hợp Thành (Ghép) không
    bool canSynthesize() const { return (m_count >= m_reqCount && m_reqCount > 0); }

    std::string getIconPath() const;
};

#endif // _CPLAYER_NINJA_PIECE_H_
