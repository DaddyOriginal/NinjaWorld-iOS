#ifndef _CMY_GROUP_CARD_VIEW_H_
#define _CMY_GROUP_CARD_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CPlayerNinja.h"
#include "CTeamCard.h"
#include "CActiveTeamMgr.h"
#include "CNinjaDetailView.h"
#include "CRLRequest.h"
#include <vector>
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CMyGroupCardView: Màn hình Đội hình ra trận & Quản lý Nhẫn Giả (Firefly MMO)
 * Gắn kết trực tiếp với sub_ui/TeamNinjaView.ccbi
 * Quản lý 6 vị trí ra trận (node_icon1 -> node_icon6), 4 ô trang bị, 4 ô nhẫn thuật,
 * duyên phận tướng và mở modal Chi tiết Nhẫn Giả (CNinjaDetailView).
 */
class CMyGroupCardView
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public NinjaDetailDelegate
    , public CRLNetDelegate
{
private:
    int m_curSlotIndex;                 // Vị trí đang chọn xem (0 -> 5 tương ứng Slot 1 -> 6)
    CNinjaDetailView* m_pDetailDialog;  // Popup chi tiết nhẫn giả

    // CCB Variable Binders từ TeamNinjaView.ccbi
    CCNode* m_pNodeIcons[6];            // node_icon1 -> node_icon6
    CCNode* m_pLayerNinja;              // layer_ninja: Container thông tin tướng
    CCSprite* m_pSpriteNinjaIcon;       // sprite_ninjaicon: Ảnh lớn chân dung tướng
    CCSprite* m_pSpriteNoNinja;         // sprite_noninja: Hiển thị khi ô trống
    CCSprite* m_pSpriteNinjaCamp;       // sprite_ninjacamp: Huy hiệu làng
    CCLabelTTF* m_pLabelNinjaName;      // label_ninjaname: Tên tướng
    CCLabelTTF* m_pLabelLevel;          // label_level: Cấp độ
    CCLabelTTF* m_pLabelNinjaGroup;     // label_ninjagroup: Vị trí đội hình
    CCLabelTTF* m_pLabelAttack;         // label_ninjaattack: Công
    CCLabelTTF* m_pLabelDefense;        // label_ninjadefense: Thủ
    CCLabelTTF* m_pLabelChakra;         // label_ninjacharkra: Chakra

    // Sao & Cường hóa
    CCSprite* m_pSpriteStars[5];        // sprite_star01 -> sprite_star05
    CCSprite* m_pSpriteStrengthIcon;    // sprite_strength_level_icon
    CCSprite* m_pSpriteStrengthFrame;   // sprite_strength_level_frame

    // 4 Trang bị & 4 Nhẫn thuật
    CCSprite* m_pSpriteEquipIcons[4];   // Vũ khí, Giáp, Trang sức, Ấn ký
    CCLabelTTF* m_pLabelEquips[8];      // label_equip1 -> label_equip8
    CCSprite* m_pSpriteSkills[4];       // sprite_skill1 -> sprite_skill4

    // Duyên phận & Combo bộ
    CCNode* m_pLayerMoreCom;            // layer_morecom
    CCLabelTTF* m_pLabelMoreCom;        // label_morecom
    CCNode* m_pLayerSuit;               // layer_suit

public:
    CMyGroupCardView();
    virtual ~CMyGroupCardView();

    static CCScene* scene();
    static CMyGroupCardView* create();

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // Touch Handling để chọn các ô vị trí (Slot 1 -> 6) và xem chi tiết
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);

    // CCB Binders
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Cập nhật giao diện toàn diện
    void InitUI();
    void selectSlot(int slotIndex);
    int getSelectedSlotIndex() const { return m_curSlotIndex; }

    // Hiển thị Popup chi tiết
    void showNinjaDetail();

    // NinjaDetailDelegate
    virtual void onNinjaDetailClose(CNinjaDetailView* pView);
    virtual void onNinjaDetailChange(CNinjaDetailView* pView);
    virtual void onNinjaDetailUpgrade(CNinjaDetailView* pView);

    // Network callback
    virtual void onHttpRequestCompleted(CRLRequest* pRequest);

    // Gửi yêu cầu thay đổi tướng trong đội hình lên server
    void requestChangeFormation(int slotSeq, int newNinjaSeq, int oldNinjaSeq);
};

#endif // _CMY_GROUP_CARD_VIEW_H_
