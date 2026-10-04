#ifndef _CMY_BACKPACK_CARD_VIEW_H_
#define _CMY_BACKPACK_CARD_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CPlayerNinja.h"
#include "CGameCardEquipment.h"
#include "CPlayerNinjaPiece.h"
#include "CGameCardMark.h"
#include "CRLRequest.h"
#include <vector>
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

enum BACKPACK_MODE {
    MODE_ITEMS = 0,         // Túi đồ chính (node_backpack)
    MODE_PIECES = 1         // Túi mảnh ghép (node_piecebackpack)
};

enum BACKPACK_SUBTAB {
    // Mode Items
    TAB_NINJA = 0,          // Thẻ Tướng
    TAB_WEAPON = 1,         // Vũ khí
    TAB_ARMOR = 2,          // Áo giáp
    TAB_ACCESSORY = 3,      // Trang sức
    TAB_MARK = 4,           // Ấn ký
    TAB_NINJUTSU = 5,       // Nhẫn thuật

    // Mode Pieces
    TAB_PIECE_NINJA = 10,   // Mảnh Tướng
    TAB_PIECE_EQUIP = 11,   // Mảnh Trang bị
    TAB_PIECE_NINJUTSU = 12,// Mảnh Nhẫn thuật
    TAB_PIECE_PET = 13      // Mảnh Linh thú
};

/**
 * CMyBackpackCardView: Màn hình Túi Đồ & Quản lý Trang bị / Mảnh ghép (Firefly MMO)
 * Gắn kết với backpack/MyBackpackView.ccbi
 * Quản lý 2 chế độ (Túi Chính & Túi Mảnh), phân loại vũ khí, giáp, trang sức, ấn ký,
 * cho phép xem chỉ số, trang bị lên tướng và hợp thành mảnh ghép.
 */
class CMyBackpackCardView
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CRLNetDelegate
{
private:
    BACKPACK_MODE m_currentMode;
    BACKPACK_SUBTAB m_currentSubTab;
    int m_selectedItemIndex;

    // CCB Variable Binders từ MyBackpackView.ccbi
    CCNode* m_pNodeBackpack;        // node_backpack: Container túi chính
    CCNode* m_pNodePieceBackpack;   // node_piecebackpack: Container túi mảnh
    CCNode* m_pNodeTableContent;    // node_tablecontent: Vùng chứa danh sách vật phẩm
    CCNode* m_pNodeCardContent;     // node_cardcontent: Khung xem chi tiết vật phẩm chọn

    // Mode Switcher Buttons
    CCControlButton* m_pBtnBackPack;
    CCControlButton* m_pBtnPieceBackPack;

    // Sub Tab Buttons (Mode Items)
    CCControlButton* m_pBtnNinja;
    CCControlButton* m_pBtnWeapon;
    CCControlButton* m_pBtnArmors;
    CCControlButton* m_pBtnAccessori;
    CCControlButton* m_pBtnMarks;
    CCControlButton* m_pBtnNinjutsu;

    // Sub Tab Buttons (Mode Pieces)
    CCControlButton* m_pBtnNinjaPiece;
    CCControlButton* m_pBtnEquipPiece;
    CCControlButton* m_pBtnNinjutsuPiece;
    CCControlButton* m_pBtnPetPiece;

    // Scrollable Grid
    CCScrollView* m_pScrollView;
    CCNode* m_pGridContainer;

    // Preview Panel Labels & Sprites
    CCLabelTTF* m_pLabelPreviewName;
    CCLabelTTF* m_pLabelPreviewLevel;
    CCLabelTTF* m_pLabelPreviewStats;
    CCLabelTTF* m_pLabelPreviewDesc;
    CCSprite* m_pSpritePreviewIcon;
    CCControlButton* m_pBtnAction;  // Nút hành động (Trang bị / Hợp thành / Cường hóa)

public:
    CMyBackpackCardView();
    virtual ~CMyBackpackCardView();

    static CCScene* scene();
    static CMyBackpackCardView* create();

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Binders
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Chuyển đổi Mode & Tab
    void switchMode(BACKPACK_MODE mode);
    void switchSubTab(BACKPACK_SUBTAB tab);

    // Cập nhật hiển thị giao diện danh sách
    void refreshItemList();
    void updatePreviewPanel();

    // Nút bấm chuyển Tab trên CCB
    void onBtnModeBackpack(CCObject* pSender, CCControlEvent pEvent);
    void onBtnModePieceBackpack(CCObject* pSender, CCControlEvent pEvent);

    void onTabNinja(CCObject* pSender, CCControlEvent pEvent);
    void onTabWeapon(CCObject* pSender, CCControlEvent pEvent);
    void onTabArmors(CCObject* pSender, CCControlEvent pEvent);
    void onTabAccessori(CCObject* pSender, CCControlEvent pEvent);
    void onTabMarks(CCObject* pSender, CCControlEvent pEvent);
    void onTabNinjutsu(CCObject* pSender, CCControlEvent pEvent);

    void onTabNinjaPiece(CCObject* pSender, CCControlEvent pEvent);
    void onTabEquipPiece(CCObject* pSender, CCControlEvent pEvent);
    void onTabNinjutsuPiece(CCObject* pSender, CCControlEvent pEvent);
    void onTabPetPiece(CCObject* pSender, CCControlEvent pEvent);

    // Hành động (Trang bị / Ghép mảnh)
    void onBtnActionClick(CCObject* pSender, CCControlEvent pEvent);
    void onSelectItem(int index);

    // Network callback
    virtual void onHttpRequestCompleted(CRLRequest* pRequest);
};

#endif // _CMY_BACKPACK_CARD_VIEW_H_
