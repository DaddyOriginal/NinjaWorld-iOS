#include "CDailyRewardView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CDailyRewardView::CDailyRewardView()
    : m_pLabelTitle(NULL)
    , m_pSprGot(NULL)
    , m_pBtnClose(NULL)
    , m_pBtnGetAward(NULL)
{
    for (int i = 0; i < 4; ++i) {
        m_pSprGiftIcon[i] = NULL;
        m_pSprPiece[i] = NULL;
        m_pLabelItem[i] = NULL;
        m_pBtnAward[i] = NULL;
    }
}

CDailyRewardView::~CDailyRewardView() {
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);
    CC_SAFE_RELEASE_NULL(m_pSprGot);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnGetAward);

    for (int i = 0; i < 4; ++i) {
        CC_SAFE_RELEASE_NULL(m_pSprGiftIcon[i]);
        CC_SAFE_RELEASE_NULL(m_pSprPiece[i]);
        CC_SAFE_RELEASE_NULL(m_pLabelItem[i]);
        CC_SAFE_RELEASE_NULL(m_pBtnAward[i]);
    }
}

CDailyRewardView* CDailyRewardView::createWithBox(const DailyTaskAwardBox& box) {
    CDailyRewardView* pView = new CDailyRewardView();
    if (pView && pView->initWithBox(box)) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CDailyRewardView::initWithBox(const DailyTaskAwardBox& box) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    m_boxData = box;

    // Nạp giao diện DailyRewardView.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("DailyRewardView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/DailyRewardView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    updateUI();
    return true;
}

void CDailyRewardView::onEnter() {
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

void CDailyRewardView::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool CDailyRewardView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    // Chặn touch xuyên xuống các layer bên dưới
    return true;
}

void CDailyRewardView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CDailyRewardView::updateUI() {
    // 1. Tiêu đề mốc năng động
    if (m_pLabelTitle) {
        std::stringstream ss;
        ss << m_boxData.cost << " Điểm Năng Động Nhận Thưởng";
        m_pLabelTitle->setString(ss.str().c_str());
    }

    // 2. Trạng thái nút Nhận & con dấu Đã Nhận
    if (m_boxData.status == 2) { // Đã nhận
        if (m_pSprGot) m_pSprGot->setVisible(true);
        if (m_pBtnGetAward) {
            m_pBtnGetAward->setVisible(false);
            m_pBtnGetAward->setEnabled(false);
        }
    } else { // Chưa nhận
        if (m_pSprGot) m_pSprGot->setVisible(false);
        if (m_pBtnGetAward) {
            m_pBtnGetAward->setVisible(true);
            m_pBtnGetAward->setEnabled(m_boxData.status == 4); // 4 = đủ điểm nhận
        }
    }

    // 3. Hiển thị 4 phần thưởng chuẩn theo mốc năng động
    // Mốc: Bạc, Vàng, Ramen, Hoán Cốt Đan
    int silverVal = 5000;
    int goldVal = 20;
    int ramenVal = 1;
    int hcdVal = 5;

    switch (m_boxData.cost) {
        case 30:  silverVal = 5000;   goldVal = 20;  ramenVal = 1; hcdVal = 5;  break;
        case 80:  silverVal = 15000;  goldVal = 50;  ramenVal = 2; hcdVal = 10; break;
        case 120: silverVal = 30000;  goldVal = 100; ramenVal = 3; hcdVal = 15; break;
        case 160: silverVal = 50000;  goldVal = 150; ramenVal = 4; hcdVal = 20; break;
        case 200: silverVal = 80000;  goldVal = 200; ramenVal = 5; hcdVal = 30; break;
        case 250: silverVal = 150000; goldVal = 300; ramenVal = 8; hcdVal = 50; break;
        default: break;
    }

    std::string itemNames[4];
    std::stringstream s1, s2, s3, s4;
    s1 << "Bạc *" << silverVal; itemNames[0] = s1.str();
    s2 << "Vàng *" << goldVal;  itemNames[1] = s2.str();
    s3 << "Mì Ramen *" << ramenVal; itemNames[2] = s3.str();
    s4 << "Hoán Cốt Đan *" << hcdVal; itemNames[3] = s4.str();

    for (int i = 0; i < 4; ++i) {
        if (m_pLabelItem[i]) {
            m_pLabelItem[i]->setVisible(true);
            m_pLabelItem[i]->setString(itemNames[i].c_str());
        }
        if (m_pSprPiece[i]) {
            m_pSprPiece[i]->setVisible(false);
        }
    }
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CDailyRewardView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CDailyRewardView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnClose") == 0 || strcmp(pSelectorName, "dialogClose") == 0) {
        return cccontrol_selector(CDailyRewardView::onBtnClose);
    }
    if (strcmp(pSelectorName, "onBtnGetAward") == 0) {
        return cccontrol_selector(CDailyRewardView::onBtnGetAward);
    }
    if (strncmp(pSelectorName, "click_btn_award_", 16) == 0) {
        return cccontrol_selector(CDailyRewardView::onBtnAwardClicked);
    }
    return NULL;
}

bool CDailyRewardView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_title", CCLabelTTF*, this->m_pLabelTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_got", CCSprite*, this->m_pSprGot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "closeButton", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_getAward", CCControlButton*, this->m_pBtnGetAward);

    for (int i = 0; i < 4; ++i) {
        char buf[64];
        snprintf(buf, sizeof(buf), "sprite_gift_icon_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCSprite*, this->m_pSprGiftIcon[i]);

        snprintf(buf, sizeof(buf), "spr_piece_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCSprite*, this->m_pSprPiece[i]);

        snprintf(buf, sizeof(buf), "label_item_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCLabelTTF*, this->m_pLabelItem[i]);

        snprintf(buf, sizeof(buf), "btn_award_%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCControlButton*, this->m_pBtnAward[i]);
    }

    return false;
}

void CDailyRewardView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CDailyRewardView::onBtnGetAward(CCObject* pSender, CCControlEvent pEvent) {
    if (m_boxData.status != 4) return;

    CCLog("[CDailyRewardView] Nguoi choi bam Nhan Thuong Ruong Moc %d Diem", m_boxData.cost);
    CDailyTaskMgr::sharedManager()->requestClaimBox(m_boxData.id);
    this->removeFromParentAndCleanup(true);
}

void CDailyRewardView::onBtnAwardClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CDailyRewardView] Bam xem chi tiet vat pham thuong");
}
