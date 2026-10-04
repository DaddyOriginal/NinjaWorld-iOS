#include "CSelectAvatorScene.h"
#include "CSelectRoleAvatorScene.h"
#include "CCBManager.h"
#include <sstream>

CSelectAvatorScene::CSelectAvatorScene()
    : m_selectedCountry(1) // Mặc định Làng Lá (Hỏa Quốc)
    , m_pBtnNext(NULL)
    , m_pCountryNode(NULL)
    , m_pIconMapNow(NULL)
    , m_pIconNow(NULL)
    , m_pIconMapPre(NULL)
    , m_pIconMapNext(NULL)
    , m_pLabelCountryName(NULL)
    , m_pLabelCountryDesc(NULL)
{
    for (int i = 0; i < 5; ++i) {
        m_pIcons[i] = NULL;
        m_pIconMaps[i] = NULL;
    }
}

CSelectAvatorScene::~CSelectAvatorScene() {
    CC_SAFE_RELEASE_NULL(m_pBtnNext);
    CC_SAFE_RELEASE_NULL(m_pCountryNode);
    CC_SAFE_RELEASE_NULL(m_pIconMapNow);
    CC_SAFE_RELEASE_NULL(m_pIconNow);
    CC_SAFE_RELEASE_NULL(m_pIconMapPre);
    CC_SAFE_RELEASE_NULL(m_pIconMapNext);

    for (int i = 0; i < 5; ++i) {
        CC_SAFE_RELEASE_NULL(m_pIcons[i]);
        CC_SAFE_RELEASE_NULL(m_pIconMaps[i]);
    }

    CC_SAFE_RELEASE_NULL(m_pLabelCountryName);
    CC_SAFE_RELEASE_NULL(m_pLabelCountryDesc);
}

CCScene* CSelectAvatorScene::scene() {
    CCScene* pScene = CCScene::create();
    CSelectAvatorScene* pLayer = CSelectAvatorScene::create();
    if (pScene && pLayer) {
        pScene->addChild(pLayer);
        return pScene;
    }
    return NULL;
}

bool CSelectAvatorScene::init() {
    if (!CCLayer::init()) {
        return false;
    }

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Nạp giao diện SelectorCountryAvatar.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("SelectorCountryAvatar.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/SelectorCountryAvatar.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 0);
        CCLog("[CSelectAvatorScene] Nap thanh cong SelectorCountryAvatar.ccbi!");
    } else {
        // Fallback UI
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg, 0);

        CCLabelTTF* pTitle = CCLabelTTF::create("CHỌN QUỐC GIA KHỞI ĐẦU", "Helvetica-Bold", 32.0f);
        pTitle->setPosition(ccp(winSize.width * 0.5f, winSize.height - 100.0f));
        pTitle->setColor(ccc3(250, 204, 21));
        this->addChild(pTitle, 1);
    }

    // Nhãn mô tả quốc gia động
    m_pLabelCountryName = CCLabelTTF::create("HỎA QUỐC (LÀNG LÁ)", "Helvetica-Bold", 26.0f);
    m_pLabelCountryName->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.35f));
    m_pLabelCountryName->setColor(ccc3(245, 158, 11));
    m_pLabelCountryName->retain();
    this->addChild(m_pLabelCountryName, 10);

    m_pLabelCountryDesc = CCLabelTTF::create("Vùng đất của ý chí lửa rực cháy, cái nôi của những Hokage vĩ đại.", "Helvetica", 18.0f);
    m_pLabelCountryDesc->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.30f));
    m_pLabelCountryDesc->setColor(ccc3(203, 213, 225));
    m_pLabelCountryDesc->retain();
    this->addChild(m_pLabelCountryDesc, 10);

    // Nút điều hướng 2 bên trái/phải nếu CCBI không có sẵn nút bấm
    CCMenuItemFont* pLeft = CCMenuItemFont::create("◀ Trước", this, menu_selector(CSelectAvatorScene::onBtnPreCountry));
    pLeft->setFontSize(22);
    pLeft->setColor(ccc3(255, 255, 255));
    pLeft->setPosition(ccp(80.0f, winSize.height * 0.55f));

    CCMenuItemFont* pRight = CCMenuItemFont::create("Sau ▶", this, menu_selector(CSelectAvatorScene::onBtnNextCountry));
    pRight->setFontSize(22);
    pRight->setColor(ccc3(255, 255, 255));
    pRight->setPosition(ccp(winSize.width - 80.0f, winSize.height * 0.55f));

    CCMenu* pArrowsMenu = CCMenu::create(pLeft, pRight, NULL);
    pArrowsMenu->setPosition(CCPointZero);
    this->addChild(pArrowsMenu, 15);

    updateCountryDisplay();

    return true;
}

void CSelectAvatorScene::onEnter() {
    CCLayer::onEnter();
    if (m_pBtnNext) {
        m_pBtnNext->setTouchPriority(-1);
        m_pBtnNext->addTargetWithActionForControlEvents(this, cccontrol_selector(CSelectAvatorScene::onBtnNext), CCControlEventTouchUpInside);
    }
}

void CSelectAvatorScene::onExit() {
    CCLayer::onExit();
}

void CSelectAvatorScene::updateCountryDisplay() {
    const char* names[] = {
        "HỎA QUỐC (LÀNG LÁ - KONOHA)",
        "THỦY QUỐC (LÀNG SƯƠNG MÙ - KIRI)",
        "PHONG QUỐC (LÀNG CÁT - SUNA)",
        "THỔ QUỐC (LÀNG ĐÁ - IWA)",
        "LÔI QUỐC (LÀNG MÂY - KUMO)"
    };

    const char* descs[] = {
        "Vùng đất của ý chí lửa rực cháy, cái nôi của những Hokage huyền thoại.",
        "Ẩn hiện trong sương mù dày đặc, nơi xuất thân của Thất Kiếm trứ danh.",
        "Vương quốc giữa sa mạc cằn cỗi, kiên định và bất khuất như cát vàng.",
        "Vững chãi tựa núi đá ngàn năm, nơi hội tụ ý chí phòng ngự thép.",
        "Rền vang sấm sét đỉnh núi cao, sở hữu thể thuật lôi độn thần tốc."
    };

    int idx = m_selectedCountry - 1;
    if (idx >= 0 && idx < 5) {
        if (m_pLabelCountryName) m_pLabelCountryName->setString(names[idx]);
        if (m_pLabelCountryDesc) m_pLabelCountryDesc->setString(descs[idx]);

        // Cập nhật hiển thị biểu tượng quốc gia trong CCBI
        for (int i = 0; i < 5; ++i) {
            if (m_pIcons[i]) m_pIcons[i]->setVisible(i == idx);
            if (m_pIconMaps[i]) m_pIconMaps[i]->setVisible(i == idx);
        }
    }
}

void CSelectAvatorScene::nextCountry() {
    m_selectedCountry++;
    if (m_selectedCountry > 5) m_selectedCountry = 1;
    updateCountryDisplay();
}

void CSelectAvatorScene::prevCountry() {
    m_selectedCountry--;
    if (m_selectedCountry < 1) m_selectedCountry = 5;
    updateCountryDisplay();
}

SEL_MenuHandler CSelectAvatorScene::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnNext") == 0 || strcmp(pSelectorName, "onBtnNext") == 0) {
        return menu_selector(CSelectAvatorScene::onBtnNext);
    }
    return NULL;
}

SEL_CCControlHandler CSelectAvatorScene::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnNext") == 0 || strcmp(pSelectorName, "onBtnNext") == 0) {
        return cccontrol_selector(CSelectAvatorScene::onBtnNext);
    }
    return NULL;
}

bool CSelectAvatorScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_nextBtn", CCControlButton*, this->m_pBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnNext", CCControlButton*, this->m_pBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_node", CCNode*, this->m_pCountryNode);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_icon_01", CCSprite*, this->m_pIcons[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_icon_02", CCSprite*, this->m_pIcons[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_icon_03", CCSprite*, this->m_pIcons[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_icon_04", CCSprite*, this->m_pIcons[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_icon_05", CCSprite*, this->m_pIcons[4]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_01", CCSprite*, this->m_pIconMaps[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_02", CCSprite*, this->m_pIconMaps[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_03", CCSprite*, this->m_pIconMaps[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_04", CCSprite*, this->m_pIconMaps[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_05", CCSprite*, this->m_pIconMaps[4]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_now", CCSprite*, this->m_pIconMapNow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_icon_now", CCSprite*, this->m_pIconNow);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_pre", CCSprite*, this->m_pIconMapPre);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_iconMap_next", CCSprite*, this->m_pIconMapNext);

    return false;
}

void CSelectAvatorScene::onBtnNext(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CSelectAvatorScene] Nguoi choi chon Quoc Gia ID=%d -> Chuyen sang chon Tuong & Dat Ten", m_selectedCountry);
    CCScene* pRoleScene = CSelectRoleAvatorScene::scene(m_selectedCountry);
    if (pRoleScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.4f, pRoleScene));
    }
}

void CSelectAvatorScene::onBtnNextCountry(CCObject* pSender, CCControlEvent pEvent) {
    nextCountry();
}

void CSelectAvatorScene::onBtnPreCountry(CCObject* pSender, CCControlEvent pEvent) {
    prevCountry();
}

void CSelectAvatorScene::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool CSelectAvatorScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    m_touchStart = pTouch->getLocation();
    return true;
}

void CSelectAvatorScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
    CCPoint endPoint = pTouch->getLocation();
    float dx = endPoint.x - m_touchStart.x;
    if (dx > 50.0f) {
        prevCountry();
    } else if (dx < -50.0f) {
        nextCountry();
    }
}
