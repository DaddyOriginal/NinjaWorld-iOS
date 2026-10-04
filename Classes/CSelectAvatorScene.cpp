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
    , m_pCountryEmblem(NULL)
    , m_pCountryMap(NULL)
    , m_pLabelCountryName(NULL)
    , m_pLabelCountryDesc(NULL)
{
    for (int i = 0; i < 5; ++i) {
        m_pIcons[i] = NULL;
        m_pIconMaps[i] = NULL;
        m_pCountryBtnItems[i] = NULL;
    }
}

CSelectAvatorScene::~CSelectAvatorScene() {
    CC_SAFE_RELEASE_NULL(m_pBtnNext);
    CC_SAFE_RELEASE_NULL(m_pCountryNode);
    CC_SAFE_RELEASE_NULL(m_pIconMapNow);
    CC_SAFE_RELEASE_NULL(m_pIconNow);
    CC_SAFE_RELEASE_NULL(m_pIconMapPre);
    CC_SAFE_RELEASE_NULL(m_pIconMapNext);
    CC_SAFE_RELEASE_NULL(m_pCountryEmblem);
    CC_SAFE_RELEASE_NULL(m_pCountryMap);

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

    // 0. Nạp sẵn các sprite frames nguyên bản của phần chọn quốc gia
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate_1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate_1.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("com_res/Resident.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Resident.plist");

    // 1. Nạp giao diện SelectorCountryAvatar.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("SelectorCountryAvatar.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/SelectorCountryAvatar.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 0);
        CCLog("[CSelectAvatorScene] Nạp thành công SelectorCountryAvatar.ccbi!");
    } else {
        // Fallback nền
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg, 0);
    }

    // 2. Bản đồ & Biểu tượng Quốc gia động (Dynamic Authentic Country Sprites)
    m_pCountryMap = CCSprite::create();
    m_pCountryMap->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.58f));
    m_pCountryMap->retain();
    this->addChild(m_pCountryMap, 5);

    m_pCountryEmblem = CCSprite::create();
    m_pCountryEmblem->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.44f));
    m_pCountryEmblem->retain();
    this->addChild(m_pCountryEmblem, 6);

    // 3. Tiêu đề và Mô tả quốc gia
    m_pLabelCountryName = CCLabelTTF::create("HỎA QUỐC (LÀNG LÁ - KONOHA)", "Helvetica-Bold", 26.0f);
    m_pLabelCountryName->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.32f));
    m_pLabelCountryName->setColor(ccc3(245, 158, 11));
    m_pLabelCountryName->retain();
    this->addChild(m_pLabelCountryName, 10);

    m_pLabelCountryDesc = CCLabelTTF::create("Vùng đất của ý chí lửa rực cháy, cái nôi của những Hokage huyền thoại.", "Helvetica", 18.0f);
    m_pLabelCountryDesc->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.26f));
    m_pLabelCountryDesc->setColor(ccc3(203, 213, 225));
    m_pLabelCountryDesc->retain();
    this->addChild(m_pLabelCountryDesc, 10);

    // 4. Thanh 5 Nút chọn trực tiếp 5 Quốc gia (HỎA, THỦY, PHONG, THỔ, LÔI)
    CCMenu* pCountryMenu = CCMenu::create();
    pCountryMenu->setPosition(CCPointZero);
    pCountryMenu->setHandlerPriority(-128);

    const char* tabNames[5] = { "1. HỎA", "2. THỦY", "3. PHONG", "4. THỔ", "5. LÔI" };
    float tabStartX = winSize.width * 0.5f - 240.0f;
    float tabGap = 120.0f;

    for (int i = 0; i < 5; ++i) {
        m_pCountryBtnItems[i] = CCMenuItemFont::create(tabNames[i], this, menu_selector(CSelectAvatorScene::onSelectCountryTab));
        if (m_pCountryBtnItems[i]) {
            m_pCountryBtnItems[i]->setTag(i + 1);
            m_pCountryBtnItems[i]->setFontSize(22);
            m_pCountryBtnItems[i]->setPosition(ccp(tabStartX + i * tabGap, winSize.height * 0.18f));
            pCountryMenu->addChild(m_pCountryBtnItems[i]);
        }
    }
    this->addChild(pCountryMenu, 15);

    // 5. Nút điều hướng 2 bên trái/phải và Nút TIẾP TỤC
    CCMenu* pNavMenu = CCMenu::create();
    pNavMenu->setPosition(CCPointZero);
    pNavMenu->setHandlerPriority(-128);

    CCMenuItemFont* pLeft = CCMenuItemFont::create("◀ Trước", this, menu_selector(CSelectAvatorScene::onBtnPreCountryMenu));
    pLeft->setFontSize(24);
    pLeft->setColor(ccc3(255, 255, 255));
    pLeft->setPosition(ccp(70.0f, winSize.height * 0.58f));
    pNavMenu->addChild(pLeft);

    CCMenuItemFont* pRight = CCMenuItemFont::create("Sau ▶", this, menu_selector(CSelectAvatorScene::onBtnNextCountryMenu));
    pRight->setFontSize(24);
    pRight->setColor(ccc3(255, 255, 255));
    pRight->setPosition(ccp(winSize.width - 70.0f, winSize.height * 0.58f));
    pNavMenu->addChild(pRight);

    CCMenuItemFont* pBtnConfirm = CCMenuItemFont::create("TIẾP TỤC  ▶", this, menu_selector(CSelectAvatorScene::onBtnNextMenu));
    pBtnConfirm->setFontSize(24);
    pBtnConfirm->setColor(ccc3(245, 158, 11)); // Màu vàng gold nổi bật
    pBtnConfirm->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.08f));
    pNavMenu->addChild(pBtnConfirm);

    this->addChild(pNavMenu, 20);

    // Cập nhật hiển thị quốc gia ban đầu
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

void CSelectAvatorScene::selectCountry(int countryIndex) {
    if (countryIndex < 1) countryIndex = 1;
    if (countryIndex > 5) countryIndex = 5;
    m_selectedCountry = countryIndex;
    updateCountryDisplay();
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

    const char* emblemFrames[] = {
        "Candidate_fire",
        "Candidate_water",
        "Candidate_wind",
        "Candidate_soil",
        "Candidate_thunder"
    };

    const char* mapFrames[] = {
        "Candidate_mapFire",
        "Candidate_mapWater",
        "Candidate_mapWind",
        "Candidate_mapSoil",
        "Candidate_mapThunder"
    };

    int idx = m_selectedCountry - 1;
    if (idx >= 0 && idx < 5) {
        if (m_pLabelCountryName) m_pLabelCountryName->setString(names[idx]);
        if (m_pLabelCountryDesc) m_pLabelCountryDesc->setString(descs[idx]);

        // Cập nhật biểu tượng Bản đồ & Huy hiệu quốc gia
        CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        if (m_pCountryMap) {
            CCSpriteFrame* pMapFrame = pCache->spriteFrameByName(mapFrames[idx]);
            if (pMapFrame) {
                m_pCountryMap->setDisplayFrame(pMapFrame);
                m_pCountryMap->setVisible(true);
            }
        }
        if (m_pCountryEmblem) {
            CCSpriteFrame* pEmblemFrame = pCache->spriteFrameByName(emblemFrames[idx]);
            if (pEmblemFrame) {
                m_pCountryEmblem->setDisplayFrame(pEmblemFrame);
                m_pCountryEmblem->setVisible(true);
            }
        }

        // Cập nhật màu sắc các nút tab (Màu vàng cho quốc gia đang chọn, màu xám nhạt cho còn lại)
        for (int i = 0; i < 5; ++i) {
            if (m_pCountryBtnItems[i]) {
                if (i == idx) {
                    m_pCountryBtnItems[i]->setColor(ccc3(245, 158, 11)); // Gold
                    m_pCountryBtnItems[i]->setScale(1.15f);
                } else {
                    m_pCountryBtnItems[i]->setColor(ccc3(148, 163, 184)); // Muted
                    m_pCountryBtnItems[i]->setScale(1.0f);
                }
            }
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

void CSelectAvatorScene::onSelectCountryTab(CCObject* pSender) {
    CCNode* pNode = dynamic_cast<CCNode*>(pSender);
    if (pNode) {
        selectCountry(pNode->getTag());
    }
}

SEL_MenuHandler CSelectAvatorScene::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnNext") == 0 || strcmp(pSelectorName, "onBtnNext") == 0 || strcmp(pSelectorName, "Candidate_nextBtn") == 0) {
        return menu_selector(CSelectAvatorScene::onBtnNextMenu);
    }
    return NULL;
}

SEL_CCControlHandler CSelectAvatorScene::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnNext") == 0 || strcmp(pSelectorName, "onBtnNext") == 0 || strcmp(pSelectorName, "Candidate_nextBtn") == 0) {
        return cccontrol_selector(CSelectAvatorScene::onBtnNext);
    }
    return NULL;
}

bool CSelectAvatorScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_country_nextBtn", CCControlButton*, this->m_pBtnNext);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Candidate_nextBtn", CCControlButton*, this->m_pBtnNext);
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
    CCLog("[CSelectAvatorScene] Người chơi chọn Quốc Gia ID=%d -> Chuyển sang chọn Tướng & Đặt Tên", m_selectedCountry);
    CCScene* pRoleScene = CSelectRoleAvatorScene::scene(m_selectedCountry);
    if (pRoleScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.4f, pRoleScene));
    }
}

void CSelectAvatorScene::onBtnNextMenu(CCObject* pSender) {
    onBtnNext(pSender, CCControlEventTouchUpInside);
}

void CSelectAvatorScene::onBtnNextCountry(CCObject* pSender, CCControlEvent pEvent) {
    nextCountry();
}

void CSelectAvatorScene::onBtnNextCountryMenu(CCObject* pSender) {
    nextCountry();
}

void CSelectAvatorScene::onBtnPreCountry(CCObject* pSender, CCControlEvent pEvent) {
    prevCountry();
}

void CSelectAvatorScene::onBtnPreCountryMenu(CCObject* pSender) {
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
