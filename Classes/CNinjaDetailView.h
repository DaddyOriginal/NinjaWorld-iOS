#ifndef _CNINJA_DETAIL_VIEW_H_
#define _CNINJA_DETAIL_VIEW_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CPlayerNinja.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CNinjaDetailView;

/**
 * Delegate nhận phản hồi sự kiện từ màn hình Chi Tiết Nhẫn Giả
 */
class NinjaDetailDelegate {
public:
    virtual ~NinjaDetailDelegate() {}
    virtual void onNinjaDetailClose(CNinjaDetailView* pView) {}
    virtual void onNinjaDetailChange(CNinjaDetailView* pView) {}
    virtual void onNinjaDetailUpgrade(CNinjaDetailView* pView) {}
};

/**
 * CNinjaDetailView: Popup Xem chi tiết & Thao tác Nhẫn Giả
 * Gắn kết với dlg_ui/NinjaDetailView.ccbi
 * Cho phép Đổi Tướng (BtnChange), Cường Hóa (BtnUpgrade), Đột Phá (BtnUpgrade1),
 * Trùng Sinh (BtnUpgradeRe), Tu Luyện Tiềm Năng (BtnUpgradePo).
 */
class CNinjaDetailView
    : public CCLayerColor
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
{
private:
    CPlayerNinja* m_pNinja;
    NinjaDetailDelegate* m_pDelegate;

    // Node & Controls từ NinjaDetailView.ccbi
    CCLabelTTF* m_pLabelName;
    CCNode* m_pLayerScrollView;
    CCControlButton* m_pBtnClose;
    CCControlButton* m_pBtnChange;
    CCControlButton* m_pBtnUpgrade;
    CCControlButton* m_pBtnUpgrade1;
    CCControlButton* m_pBtnUpgradeRe;
    CCControlButton* m_pBtnUpgradePo;

    // Dynamic UI labels trong scroll view
    CCLabelTTF* m_pLabelLevel;
    CCLabelTTF* m_pLabelQuality;
    CCLabelTTF* m_pLabelAttack;
    CCLabelTTF* m_pLabelDefense;
    CCLabelTTF* m_pLabelChakra;
    CCLabelTTF* m_pLabelWarPower;
    CCLabelTTF* m_pLabelDesc;
    CCSprite* m_pSpritePortrait;

public:
    CNinjaDetailView();
    virtual ~CNinjaDetailView();

    static CNinjaDetailView* create();
    static CNinjaDetailView* createWithNinja(CPlayerNinja* pNinja, NinjaDetailDelegate* pDelegate = NULL);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Gán dữ liệu và Delegate
    void SetNinja(CPlayerNinja* pNinja);
    CPlayerNinja* GetNinja() const { return m_pNinja; }
    void SetDelegate(NinjaDetailDelegate* pDelegate) { m_pDelegate = pDelegate; }

    // Cập nhật hiển thị giao diện
    void InitUI();

    // Hiển thị popup lên một parent node
    void Show(CCNode* pParent, int zOrder = 100);

    // Xử lý nút bấm CCB
    void onBtnClose(CCObject* pSender, CCControlEvent pEvent);
    void onBtnChange(CCObject* pSender, CCControlEvent pEvent);
    void onBtnUpgrade(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBreakthrough(CCObject* pSender, CCControlEvent pEvent);
    void onBtnReincarnation(CCObject* pSender, CCControlEvent pEvent);
    void onBtnPotential(CCObject* pSender, CCControlEvent pEvent);
};

#endif // _CNINJA_DETAIL_VIEW_H_
