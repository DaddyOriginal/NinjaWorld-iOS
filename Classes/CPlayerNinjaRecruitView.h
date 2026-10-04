#ifndef __C_PLAYER_NINJA_RECRUIT_VIEW_H__
#define __C_PLAYER_NINJA_RECRUIT_VIEW_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCBManager.h"
#include "CRLRequest.h"
#include "CNinjaTableMgr.h"
#include "CPlayerDataMgr.h"
#include <vector>
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

struct RecruitTierData {
    int recruitId;       // 1 = Hạ Đẳng, 2 = Trung Đẳng, 3 = Thần Khí Cao Cấp
    std::string name;
    int costSingle;      // Vàng (e.g. 300, 150, 280)
    int costTen;         // Vàng (e.g. 2700, 1350, 2500)
    int cooldownSeconds;
    int timeLeft;
    int protectComing;   // Số lần còn lại trước khi kích hoạt bảo hiểm
    bool isFree;
};

class CPlayerNinjaRecruitView : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner {
private:
    // StoreItemsView.ccbi members
    CCControlButton* m_pBtnPray;
    CCControlButton* m_pBtnConsum;
    CCControlButton* m_pBtnGiftPack;
    CCControlButton* m_pBtnVipInfo;
    CCNode* m_pNodeTableContent;
    CCNode* m_pNodeTouchContent;
    CCNode* m_pNodeCardContent;

    std::vector<RecruitTierData> m_tiers;
    int m_selectedTier;

    void requestRecruitInfo();
    void onRecruitInfoResp(CRLRequest* pRequest);
    void buildRecruitBanners();
    void setupBannerCell(CCNode* cellNode, const RecruitTierData& tier, float posX);

    void sendRecruitRequest(int recruitId, bool isTen, bool isFree);
    void onRecruitResultResp(CRLRequest* pRequest);

    void showSingleRecruitPopup(int ninjaId, int star);
    void showTenRecruitPopup(const std::vector<int>& ninjaIds);

public:
    CPlayerNinjaRecruitView();
    virtual ~CPlayerNinjaRecruitView();

    static CPlayerNinjaRecruitView* create();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCB Bindings
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Callbacks
    void onBtnPrayClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnConsumClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnGiftPackClicked(CCObject* pSender, CCControlEvent pEvent);
    void onBtnVipInfoClicked(CCObject* pSender, CCControlEvent pEvent);

    void onRecruitSingleClicked(CCObject* pSender);
    void onRecruitTenClicked(CCObject* pSender);

    void refreshView();
};

#endif // __C_PLAYER_NINJA_RECRUIT_VIEW_H__
