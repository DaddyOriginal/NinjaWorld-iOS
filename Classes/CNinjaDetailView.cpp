#include "CNinjaDetailView.h"
#include "CCBManager.h"
#include <sstream>

CNinjaDetailView::CNinjaDetailView()
    : m_pNinja(NULL)
    , m_pDelegate(NULL)
    , m_pLabelName(NULL)
    , m_pLayerScrollView(NULL)
    , m_pBtnClose(NULL)
    , m_pBtnChange(NULL)
    , m_pBtnUpgrade(NULL)
    , m_pBtnUpgrade1(NULL)
    , m_pBtnUpgradeRe(NULL)
    , m_pBtnUpgradePo(NULL)
    , m_pLabelLevel(NULL)
    , m_pLabelQuality(NULL)
    , m_pLabelAttack(NULL)
    , m_pLabelDefense(NULL)
    , m_pLabelChakra(NULL)
    , m_pLabelWarPower(NULL)
    , m_pLabelDesc(NULL)
    , m_pSpritePortrait(NULL)
{
}

CNinjaDetailView::~CNinjaDetailView() {
    CC_SAFE_RELEASE_NULL(m_pNinja);
    CC_SAFE_RELEASE_NULL(m_pLabelName);
    CC_SAFE_RELEASE_NULL(m_pLayerScrollView);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnChange);
    CC_SAFE_RELEASE_NULL(m_pBtnUpgrade);
    CC_SAFE_RELEASE_NULL(m_pBtnUpgrade1);
    CC_SAFE_RELEASE_NULL(m_pBtnUpgradeRe);
    CC_SAFE_RELEASE_NULL(m_pBtnUpgradePo);
}

CNinjaDetailView* CNinjaDetailView::create() {
    CNinjaDetailView* pRet = new CNinjaDetailView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CNinjaDetailView* CNinjaDetailView::createWithNinja(CPlayerNinja* pNinja, NinjaDetailDelegate* pDelegate) {
    CNinjaDetailView* pRet = new CNinjaDetailView();
    if (pRet && pRet->init()) {
        pRet->SetNinja(pNinja);
        pRet->SetDelegate(pDelegate);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CNinjaDetailView::init() {
    // Nền tối mờ toàn màn hình dạng Modal
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    setTouchPriority(-128);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Thử nạp từ CCBI
    CCNode* pCcbNode = CCBManager::sharedManager()->loadNodeFromCCBI("dlg_ui/NinjaDetailView.ccbi", this);
    if (!pCcbNode) {
        pCcbNode = CCBManager::sharedManager()->loadNodeFromCCBI("NinjaDetailView.ccbi", this);
    }

    if (pCcbNode) {
        pCcbNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
        this->addChild(pCcbNode);
    } else {
        // Fallback Native UI hoàn chỉnh nếu CCBI chưa nạp được
        CCNode* pDialogBox = CCNode::create();
        pDialogBox->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
        this->addChild(pDialogBox);

        // Nền bảng thông tin
        CCLayerColor* pPanel = CCLayerColor::initWithColor(ccc4(20, 24, 38, 245), 680, 800) ? CCLayerColor::create(ccc4(20, 24, 38, 245), 680, 800) : NULL;
        if (pPanel) {
            pPanel->ignoreAnchorPointForPosition(false);
            pPanel->setAnchorPoint(ccp(0.5f, 0.5f));
            pDialogBox->addChild(pPanel);
        }

        // Tiêu đề
        m_pLabelName = CCLabelTTF::create("Chi Tiết Nhẫn Giả", "Helvetica-Bold", 32.0f);
        m_pLabelName->setPosition(ccp(0.0f, 350.0f));
        m_pLabelName->setColor(ccc3(250, 204, 21));
        m_pLabelName->retain();
        pDialogBox->addChild(m_pLabelName);

        // Nút Đóng (X)
        CCControlButton* pCloseBtn = CCControlButton::create(
            CCLabelTTF::create("✕", "Helvetica-Bold", 28.0f),
            CCScale9Sprite::create()
        );
        pCloseBtn->setPreferredSize(CCSizeMake(50, 50));
        pCloseBtn->setPosition(ccp(300.0f, 350.0f));
        pCloseBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(CNinjaDetailView::onBtnClose), CCControlEventTouchUpInside);
        pDialogBox->addChild(pCloseBtn);

        // Khung hiển thị thông số bên trong
        m_pLayerScrollView = CCNode::create();
        m_pLayerScrollView->setPosition(ccp(-300.0f, -220.0f));
        m_pLayerScrollView->retain();
        pDialogBox->addChild(m_pLayerScrollView);

        // Thanh nút bấm thao tác ở chân hộp thoại (Change, Upgrade, Breakthrough, Reincarnation)
        const char* btnTitles[] = { "Đổi Tướng", "Cường Hóa", "Đột Phá", "Trùng Sinh", "Tu Luyện" };
        SEL_CCControlHandler handlers[] = {
            cccontrol_selector(CNinjaDetailView::onBtnChange),
            cccontrol_selector(CNinjaDetailView::onBtnUpgrade),
            cccontrol_selector(CNinjaDetailView::onBtnBreakthrough),
            cccontrol_selector(CNinjaDetailView::onBtnReincarnation),
            cccontrol_selector(CNinjaDetailView::onBtnPotential)
        };

        float startX = -240.0f;
        for (int i = 0; i < 5; ++i) {
            CCControlButton* btn = CCControlButton::create(
                CCLabelTTF::create(btnTitles[i], "Helvetica-Bold", 20.0f),
                CCScale9Sprite::create()
            );
            btn->setPreferredSize(CCSizeMake(110, 48));
            btn->setPosition(ccp(startX + i * 120.0f, -340.0f));
            btn->addTargetWithActionForControlEvents(this, handlers[i], CCControlEventTouchUpInside);
            pDialogBox->addChild(btn);
        }
    }

    return true;
}

void CNinjaDetailView::onEnter() {
    CCLayerColor::onEnter();
    InitUI();
}

void CNinjaDetailView::onExit() {
    CCLayerColor::onExit();
}

void CNinjaDetailView::SetNinja(CPlayerNinja* pNinja) {
    if (m_pNinja != pNinja) {
        CC_SAFE_RELEASE(m_pNinja);
        m_pNinja = pNinja;
        CC_SAFE_RETAIN(m_pNinja);
        InitUI();
    }
}

void CNinjaDetailView::InitUI() {
    if (!m_pNinja) return;

    if (m_pLabelName) {
        std::stringstream ss;
        ss << m_pNinja->firefly_GetName() << " [+" << m_pNinja->firefly_GetStrengthLevel() << "]";
        m_pLabelName->setString(ss.str().c_str());
    }

    if (m_pLayerScrollView) {
        m_pLayerScrollView->removeAllChildrenWithCleanup(true);

        // Hiển thị Avatar / Chân dung
        CCSprite* pPortrait = m_pNinja->createPortraitSprite();
        if (pPortrait) {
            pPortrait->setAnchorPoint(ccp(0.0f, 0.5f));
            pPortrait->setPosition(ccp(0.0f, 250.0f));
            pPortrait->setScale(0.8f);
            m_pLayerScrollView->addChild(pPortrait);
        }

        // Cột thông số bên phải chân dung
        float textX = 260.0f;
        float startY = 380.0f;
        float gapY = 36.0f;

        std::stringstream ssLvl;
        ssLvl << "Cấp độ: Lv." << m_pNinja->firefly_GetLevel() << " / 100";
        CCLabelTTF* pLvl = CCLabelTTF::create(ssLvl.str().c_str(), "Helvetica-Bold", 22.0f);
        pLvl->setAnchorPoint(ccp(0.0f, 0.5f));
        pLvl->setPosition(ccp(textX, startY));
        pLvl->setColor(ccc3(255, 255, 255));
        m_pLayerScrollView->addChild(pLvl);

        std::stringstream ssQuality;
        ssQuality << "Phẩm chất: " << m_pNinja->firefly_GetQuality() << " Sao (Cấp " << m_pNinja->GetTupoLevel() << ")";
        CCLabelTTF* pQua = CCLabelTTF::create(ssQuality.str().c_str(), "Helvetica", 20.0f);
        pQua->setAnchorPoint(ccp(0.0f, 0.5f));
        pQua->setPosition(ccp(textX, startY - gapY));
        pQua->setColor(ccc3(250, 204, 21)); // Vàng
        m_pLayerScrollView->addChild(pQua);

        std::stringstream ssAtk;
        ssAtk << "Tấn công: " << m_pNinja->firefly_GetAttackMin() << " - " << m_pNinja->firefly_GetAttackMax();
        CCLabelTTF* pAtk = CCLabelTTF::create(ssAtk.str().c_str(), "Helvetica", 20.0f);
        pAtk->setAnchorPoint(ccp(0.0f, 0.5f));
        pAtk->setPosition(ccp(textX, startY - gapY * 2));
        pAtk->setColor(ccc3(239, 68, 68)); // Đỏ
        m_pLayerScrollView->addChild(pAtk);

        std::stringstream ssDef;
        ssDef << "Phòng thủ: " << m_pNinja->firefly_GetDefenseMin() << " - " << m_pNinja->firefly_GetDefenseMax();
        CCLabelTTF* pDef = CCLabelTTF::create(ssDef.str().c_str(), "Helvetica", 20.0f);
        pDef->setAnchorPoint(ccp(0.0f, 0.5f));
        pDef->setPosition(ccp(textX, startY - gapY * 3));
        pDef->setColor(ccc3(59, 130, 246)); // Xanh lam
        m_pLayerScrollView->addChild(pDef);

        std::stringstream ssCha;
        ssCha << "Chakra: " << m_pNinja->firefly_GetChakraMin() << " - " << m_pNinja->firefly_GetChakraMax();
        CCLabelTTF* pCha = CCLabelTTF::create(ssCha.str().c_str(), "Helvetica", 20.0f);
        pCha->setAnchorPoint(ccp(0.0f, 0.5f));
        pCha->setPosition(ccp(textX, startY - gapY * 4));
        pCha->setColor(ccc3(168, 85, 247)); // Tím
        m_pLayerScrollView->addChild(pCha);

        std::stringstream ssWar;
        ssWar << "Lực chiến: " << m_pNinja->getWarPower();
        CCLabelTTF* pWar = CCLabelTTF::create(ssWar.str().c_str(), "Helvetica-Bold", 22.0f);
        pWar->setAnchorPoint(ccp(0.0f, 0.5f));
        pWar->setPosition(ccp(textX, startY - gapY * 5));
        pWar->setColor(ccc3(249, 115, 22)); // Cam
        m_pLayerScrollView->addChild(pWar);

        // Tiểu sử & Nhẫn thuật
        std::string desc = m_pNinja->firefly_GetDesc();
        if (desc.empty()) desc = "Nhẫn giả tinh nhuệ của làng.";
        CCLabelTTF* pDesc = CCLabelTTF::create(desc.c_str(), "Helvetica", 18.0f, CCSizeMake(580, 80), kCCTextAlignmentLeft);
        pDesc->setAnchorPoint(ccp(0.0f, 1.0f));
        pDesc->setPosition(ccp(10.0f, 120.0f));
        pDesc->setColor(ccc3(209, 213, 219));
        m_pLayerScrollView->addChild(pDesc);
    }
}

void CNinjaDetailView::Show(CCNode* pParent, int zOrder) {
    if (!pParent) return;
    pParent->addChild(this, zOrder);
    this->setScale(0.8f);
    this->runAction(CCEaseBackOut::create(CCScaleTo::create(0.2f, 1.0f)));
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CNinjaDetailView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CNinjaDetailView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBtnClose", CNinjaDetailView::onBtnClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnClose", CNinjaDetailView::onBtnClose);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnChange", CNinjaDetailView::onBtnChange);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnUpgrade", CNinjaDetailView::onBtnUpgrade);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnUpgrade1", CNinjaDetailView::onBtnBreakthrough);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnUpgradeRe", CNinjaDetailView::onBtnReincarnation);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnUpgradePo", CNinjaDetailView::onBtnPotential);
    return NULL;
}

bool CNinjaDetailView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name", CCLabelTTF*, m_pLabelName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_scollview", CCNode*, m_pLayerScrollView);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnChange", CCControlButton*, m_pBtnChange);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnUpgrade", CCControlButton*, m_pBtnUpgrade);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnUpgrade1", CCControlButton*, m_pBtnUpgrade1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnUpgradeRe", CCControlButton*, m_pBtnUpgradeRe);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnUpgradePo", CCControlButton*, m_pBtnUpgradePo);
    return false;
}

// -------------------------------------------------------------
// XỬ LÝ SỰ KIỆN NÚT BẤM
// -------------------------------------------------------------
void CNinjaDetailView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    if (m_pDelegate) {
        m_pDelegate->onNinjaDetailClose(this);
    }
    this->runAction(CCSequence::create(
        CCScaleTo::create(0.15f, 0.7f),
        CCCallFunc::create(this, callfunc_selector(CCNode::removeFromParent)),
        NULL
    ));
}

void CNinjaDetailView::onBtnChange(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CNinjaDetailView] Nhan nut Doi Tuong");
    if (m_pDelegate) {
        m_pDelegate->onNinjaDetailChange(this);
    }
}

void CNinjaDetailView::onBtnUpgrade(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CNinjaDetailView] Nhan nut Cuong Hoa");
    if (m_pDelegate) {
        m_pDelegate->onNinjaDetailUpgrade(this);
    }
}

void CNinjaDetailView::onBtnBreakthrough(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CNinjaDetailView] Nhan nut Dot Pha (Tupo)");
}

void CNinjaDetailView::onBtnReincarnation(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CNinjaDetailView] Nhan nut Trung Sinh");
}

void CNinjaDetailView::onBtnPotential(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CNinjaDetailView] Nhan nut Tu Luyen Tiem Nang");
}
