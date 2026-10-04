#ifndef _CSELECT_ROLE_AVATOR_SCENE_H_
#define _CSELECT_ROLE_AVATOR_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CRLRequest.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * CSelectRoleAvatorScene: Màn hình chọn Tướng khởi đầu & Đặt tên nhân vật (SelectorRoleAvatar.ccbi)
 * Cung cấp 3 Ninja khởi đầu chuẩn nguyên bản:
 * - 003: Neji Hyuga (Mã Thẻ 55)
 * - 004: Sakura Haruno (Mã Thẻ 56)
 * - 005: Shikamaru Nara (Mã Thẻ 57)
 * Tích hợp CCEditBox nhập tên, nút Xúc xắc sinh tên ngẫu nhiên (BtnRollName),
 * và gửi gói tin CMD 2100 (/rl_w_reg2) để khởi tạo nhân vật chính thức trên Máy chủ.
 */
class CSelectRoleAvatorScene : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CCEditBoxDelegate
    , public CRLRequestDelegate
{
private:
    int m_country;              // Quốc gia đã chọn (1 - 5)
    int m_selectedNinjaIndex;   // 3: Neji, 4: Sakura, 5: Shikamaru
    int m_selectedCardId;       // 55, 56, 57

    // UI Nodes từ SelectorRoleAvatar.ccbi
    CCControlButton* m_pBtnRole003;
    CCControlButton* m_pBtnRole004;
    CCControlButton* m_pBtnRole005;
    CCControlButton* m_pBtnRollName;
    CCControlButton* m_pBtnOk;

    CCNode* m_pDesc003;
    CCNode* m_pDesc004;
    CCNode* m_pDesc005;

    CCNode* m_pCardFixNode;
    CCNode* m_pNameFixNode;
    CCNode* m_pInputNameNode;

    CCSprite* m_pCurrentPortrait;
    CCEditBox* m_pEditName;
    CCLabelTTF* m_pLabelStatus;

public:
    CSelectRoleAvatorScene();
    virtual ~CSelectRoleAvatorScene();

    static CCScene* scene(int country = 1);
    static CSelectRoleAvatorScene* createWithCountry(int country);

    virtual bool initWithCountry(int country);
    virtual void onEnter();
    virtual void onExit();

    void selectNinja(int roleIndex);
    void rollRandomName();
    void updateUI();

    // CCB Resolvers
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnSelectNinja003(CCObject* pSender, CCControlEvent pEvent);
    void onBtnSelectNinja004(CCObject* pSender, CCControlEvent pEvent);
    void onBtnSelectNinja005(CCObject* pSender, CCControlEvent pEvent);
    void onBtnRandName(CCObject* pSender, CCControlEvent pEvent);
    void onBtnEnterGame(CCObject* pSender, CCControlEvent pEvent);
    void onBtnBack(CCObject* pSender, CCControlEvent pEvent);

    // CCEditBox Delegate
    virtual void editBoxEditingDidBegin(CCEditBox* editBox) {}
    virtual void editBoxEditingDidEnd(CCEditBox* editBox) {}
    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text) {}
    virtual void editBoxReturn(CCEditBox* editBox) {}

    // Network Delegate
    virtual void onHttpSuccess(CRLRequest* pRequest, const std::string& responseData);
    virtual void onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg);

private:
    void sendCreateRoleRequest(const std::string& nickName);
    void sendFetchMainpageRequest();
    void enterMainGame();
};

#endif // _CSELECT_ROLE_AVATOR_SCENE_H_
