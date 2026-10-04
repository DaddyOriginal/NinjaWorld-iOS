#include "CSelectRoleAvatorScene.h"
#include "CSelectAvatorScene.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include "CServerListMgr.h"
#include "CMainMenu.h"
#include "CNinjaTableMgr.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

CSelectRoleAvatorScene::CSelectRoleAvatorScene()
    : m_country(1)
    , m_selectedNinjaIndex(4) // Mặc định Sakura Haruno (004)
    , m_selectedCardId(56)
    , m_pBtnRole003(NULL)
    , m_pBtnRole004(NULL)
    , m_pBtnRole005(NULL)
    , m_pBtnRollName(NULL)
    , m_pBtnOk(NULL)
    , m_pDesc003(NULL)
    , m_pDesc004(NULL)
    , m_pDesc005(NULL)
    , m_pCardFixNode(NULL)
    , m_pNameFixNode(NULL)
    , m_pInputNameNode(NULL)
    , m_pCurrentPortrait(NULL)
    , m_pEditName(NULL)
    , m_pLabelStatus(NULL)
{
}

CSelectRoleAvatorScene::~CSelectRoleAvatorScene() {
    CC_SAFE_RELEASE_NULL(m_pBtnRole003);
    CC_SAFE_RELEASE_NULL(m_pBtnRole004);
    CC_SAFE_RELEASE_NULL(m_pBtnRole005);
    CC_SAFE_RELEASE_NULL(m_pBtnRollName);
    CC_SAFE_RELEASE_NULL(m_pBtnOk);

    CC_SAFE_RELEASE_NULL(m_pDesc003);
    CC_SAFE_RELEASE_NULL(m_pDesc004);
    CC_SAFE_RELEASE_NULL(m_pDesc005);

    CC_SAFE_RELEASE_NULL(m_pCardFixNode);
    CC_SAFE_RELEASE_NULL(m_pNameFixNode);
    CC_SAFE_RELEASE_NULL(m_pInputNameNode);

    CC_SAFE_RELEASE_NULL(m_pLabelStatus);
}

CCScene* CSelectRoleAvatorScene::scene(int country) {
    CCScene* pScene = CCScene::create();
    CSelectRoleAvatorScene* pLayer = CSelectRoleAvatorScene::createWithCountry(country);
    if (pScene && pLayer) {
        pScene->addChild(pLayer);
        return pScene;
    }
    return NULL;
}

CSelectRoleAvatorScene* CSelectRoleAvatorScene::createWithCountry(int country) {
    CSelectRoleAvatorScene* pRet = new CSelectRoleAvatorScene();
    if (pRet && pRet->initWithCountry(country)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CSelectRoleAvatorScene::initWithCountry(int country) {
    if (!CCLayer::init()) {
        return false;
    }

    m_country = (country >= 1 && country <= 5) ? country : 1;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 0. Nạp sẵn sprite frames cần thiết cho chọn tướng và mô tả
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate_003_desc.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate_003_desc.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate_004_desc.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate_004_desc.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/candidate/candidate_005_desc.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("candidate_005_desc.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("com_res/Resident.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Resident.plist");

    // 1. Nạp giao diện SelectorRoleAvatar.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("SelectorRoleAvatar.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/SelectorRoleAvatar.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 0);
        CCLog("[CSelectRoleAvatorScene] Nạp thành công SelectorRoleAvatar.ccbi!");
    } else {
        // Fallback UI nền
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg, 0);

        CCLabelTTF* pTitle = CCLabelTTF::create("CHỌN NHẪN GIẢ KHỞI ĐẦU", "Helvetica-Bold", 32.0f);
        pTitle->setPosition(ccp(winSize.width * 0.5f, winSize.height - 80.0f));
        pTitle->setColor(ccc3(250, 204, 21));
        this->addChild(pTitle, 1);
    }

    // 2. Thiết lập CCEditBox cho ô nhập tên nhân vật (Dùng sprite frame an toàn)
    CCPoint namePos = ccp(winSize.width * 0.5f - 40.0f, winSize.height * 0.22f);
    CCSize nameSize = CCSizeMake(260.0f, 44.0f);

    if (m_pInputNameNode) {
        namePos = m_pInputNameNode->getParent()->convertToWorldSpace(m_pInputNameNode->getPosition());
        if (m_pInputNameNode->getContentSize().width > 100) {
            nameSize = m_pInputNameNode->getContentSize();
        }
    }

    CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("reg_inputbtn");
    CCScale9Sprite* pBgBox = pFrame ? CCScale9Sprite::createWithSpriteFrame(pFrame) : CCScale9Sprite::create("com_res/reg_inputbtn.png");
    if (!pBgBox) {
        pBgBox = CCScale9Sprite::create();
    }

    m_pEditName = CCEditBox::create(nameSize, pBgBox);
    if (m_pEditName) {
        m_pEditName->setPosition(namePos);
        m_pEditName->setPlaceholderFontColor(ccc3(148, 163, 184));
        m_pEditName->setPlaceHolder("Nhập tên Nhẫn Giả...");
        m_pEditName->setFontColor(ccc3(255, 255, 255));
        m_pEditName->setFontSize(22);
        m_pEditName->setMaxLength(16);
        m_pEditName->setInputMode(kEditBoxInputModeSingleLine);
        m_pEditName->setReturnType(kKeyboardReturnTypeDone);
        m_pEditName->setTouchPriority(-10);
        m_pEditName->setDelegate(this);
        this->addChild(m_pEditName, 20);
    }

    // 3. Menu điều khiển bổ trợ (Chọn 3 Tướng, Đổi tên, và Nút Tạo)
    CCMenu* pActionMenu = CCMenu::create();
    pActionMenu->setPosition(CCPointZero);
    pActionMenu->setHandlerPriority(-128);

    // 3 Nút chọn trực tiếp 3 Nhẫn Giả
    CCMenuItemFont* pItem003 = CCMenuItemFont::create("Neji Hyuga", this, menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja003Menu));
    pItem003->setFontSize(20);
    pItem003->setColor(ccc3(255, 255, 255));
    pItem003->setPosition(ccp(winSize.width * 0.5f - 180.0f, winSize.height * 0.35f));
    pActionMenu->addChild(pItem003);

    CCMenuItemFont* pItem004 = CCMenuItemFont::create("Sakura Haruno", this, menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja004Menu));
    pItem004->setFontSize(20);
    pItem004->setColor(ccc3(245, 158, 11)); // Mặc định chọn Sakura
    pItem004->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.35f));
    pActionMenu->addChild(pItem004);

    CCMenuItemFont* pItem005 = CCMenuItemFont::create("Shikamaru", this, menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja005Menu));
    pItem005->setFontSize(20);
    pItem005->setColor(ccc3(255, 255, 255));
    pItem005->setPosition(ccp(winSize.width * 0.5f + 180.0f, winSize.height * 0.35f));
    pActionMenu->addChild(pItem005);

    // Nút Xúc xắc đổi tên ngẫu nhiên
    CCMenuItemFont* pItemRoll = CCMenuItemFont::create("🎲 Đổi Tên", this, menu_selector(CSelectRoleAvatorScene::onBtnRandNameMenu));
    pItemRoll->setFontSize(20);
    pItemRoll->setColor(ccc3(59, 130, 246));
    pItemRoll->setPosition(ccp(namePos.x + nameSize.width * 0.5f + 65.0f, namePos.y));
    pActionMenu->addChild(pItemRoll);

    // Nút TẠO NHÂN VẬT lớn
    CCMenuItemFont* pItemOk = CCMenuItemFont::create("TẠO NHÂN VẬT  ▶", this, menu_selector(CSelectRoleAvatorScene::onBtnEnterGameMenu));
    pItemOk->setFontSize(24);
    pItemOk->setColor(ccc3(245, 158, 11));
    pItemOk->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.08f));
    pActionMenu->addChild(pItemOk);

    this->addChild(pActionMenu, 25);

    // 4. Nhãn thông báo trạng thái
    m_pLabelStatus = CCLabelTTF::create("", "Helvetica", 20.0f);
    m_pLabelStatus->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.15f));
    m_pLabelStatus->setColor(ccc3(239, 68, 68));
    m_pLabelStatus->retain();
    this->addChild(m_pLabelStatus, 20);

    // Sinh ngay một tên ngẫu nhiên ban đầu
    rollRandomName();

    // Chọn Ninja mặc định Sakura (004)
    selectNinja(4);

    return true;
}

void CSelectRoleAvatorScene::onEnter() {
    CCLayer::onEnter();

    // Gán sự kiện nút bấm chuẩn
    if (m_pBtnRole003) {
        m_pBtnRole003->setTouchPriority(-1);
        m_pBtnRole003->addTargetWithActionForControlEvents(this, cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja003), CCControlEventTouchUpInside);
    }
    if (m_pBtnRole004) {
        m_pBtnRole004->setTouchPriority(-1);
        m_pBtnRole004->addTargetWithActionForControlEvents(this, cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja004), CCControlEventTouchUpInside);
    }
    if (m_pBtnRole005) {
        m_pBtnRole005->setTouchPriority(-1);
        m_pBtnRole005->addTargetWithActionForControlEvents(this, cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja005), CCControlEventTouchUpInside);
    }
    if (m_pBtnRollName) {
        m_pBtnRollName->setTouchPriority(-1);
        m_pBtnRollName->addTargetWithActionForControlEvents(this, cccontrol_selector(CSelectRoleAvatorScene::onBtnRandName), CCControlEventTouchUpInside);
    }
    if (m_pBtnOk) {
        m_pBtnOk->setTouchPriority(-1);
        m_pBtnOk->addTargetWithActionForControlEvents(this, cccontrol_selector(CSelectRoleAvatorScene::onBtnEnterGame), CCControlEventTouchUpInside);
    }
}

void CSelectRoleAvatorScene::onExit() {
    CCLayer::onExit();
}

void CSelectRoleAvatorScene::selectNinja(int roleIndex) {
    m_selectedNinjaIndex = roleIndex;
    if (roleIndex == 3) {
        m_selectedCardId = 55; // Neji Hyuga
    } else if (roleIndex == 4) {
        m_selectedCardId = 56; // Sakura Haruno
    } else if (roleIndex == 5) {
        m_selectedCardId = 57; // Shikamaru Nara
    }

    updateUI();
}

void CSelectRoleAvatorScene::updateUI() {
    // 1. Cập nhật hiển thị khung mô tả
    if (m_pDesc003) m_pDesc003->setVisible(m_selectedNinjaIndex == 3);
    if (m_pDesc004) m_pDesc004->setVisible(m_selectedNinjaIndex == 4);
    if (m_pDesc005) m_pDesc005->setVisible(m_selectedNinjaIndex == 5);

    // 2. Hiển thị chân dung lớn tại m_pCardFixNode
    if (m_pCardFixNode) {
        m_pCardFixNode->removeAllChildrenWithCleanup(true);

        const NinjaTableEntry* pEntry = CNinjaTableMgr::sharedManager()->getNinjaEntry(m_selectedCardId);
        std::string iconName = pEntry ? pEntry->icon : "npc30_6";

        CCSprite* pPortrait = NULL;
        CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(iconName.c_str());
        if (!pFrame) {
            pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((iconName + ".png").c_str());
        }
        if (!pFrame) {
            std::string plistPath = "npc/" + iconName + ".plist";
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str());
            pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(iconName.c_str());
            if (!pFrame) {
                pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName((iconName + ".png").c_str());
            }
        }

        if (pFrame) {
            pPortrait = CCSprite::createWithSpriteFrame(pFrame);
        } else {
            pPortrait = CCSprite::create("0V.png");
        }

        if (pPortrait) {
            pPortrait->setPosition(CCPointZero);
            pPortrait->setScale(0.9f);
            m_pCardFixNode->addChild(pPortrait);
        }
    }
}

void CSelectRoleAvatorScene::rollRandomName() {
    static const char* s_names[] = {
        "Naruto_VN", "Sasuke_Uchiha", "Itachi_Konoha", "Kakashi_Sensei",
        "Minato_Flash", "Gaara_Suna", "Hinata_Hyuga", "Jiraiya_Sannin",
        "Tsunade_Hime", "Madara_Legend", "Obito_Kamui", "Shikamaru_IQ",
        "Neji_Byakugan", "Sakura_Haruno", "RockLee_Tai", "Kiba_Fang",
        "Shino_Bug", "Temari_Wind", "Kankuro_Karasu", "Hashirama_Mokuton"
    };

    int count = sizeof(s_names) / sizeof(s_names[0]);
    int randIdx = rand() % count;
    int randNum = 10 + (rand() % 90);

    std::stringstream ss;
    ss << s_names[randIdx] << randNum;

    if (m_pEditName) {
        m_pEditName->setText(ss.str().c_str());
    }
    if (m_pLabelStatus) {
        m_pLabelStatus->setString("");
    }
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO SelectorRoleAvatar.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CSelectRoleAvatorScene::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnRole_003") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja003Menu);
    if (strcmp(pSelectorName, "BtnRole_004") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja004Menu);
    if (strcmp(pSelectorName, "BtnRole_005") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja005Menu);
    if (strcmp(pSelectorName, "BtnRollName") == 0 || strcmp(pSelectorName, "selector_role_rollNameBtn") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnRandNameMenu);
    if (strcmp(pSelectorName, "BtnOk") == 0 || strcmp(pSelectorName, "Candidate_okBtn") == 0 || strcmp(pSelectorName, "selector_role_okBtn") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnEnterGameMenu);
    return NULL;
}

SEL_CCControlHandler CSelectRoleAvatorScene::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnRole_003") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja003);
    if (strcmp(pSelectorName, "BtnRole_004") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja004);
    if (strcmp(pSelectorName, "BtnRole_005") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja005);
    if (strcmp(pSelectorName, "BtnRollName") == 0 || strcmp(pSelectorName, "selector_role_rollNameBtn") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnRandName);
    if (strcmp(pSelectorName, "BtnOk") == 0 || strcmp(pSelectorName, "Candidate_okBtn") == 0 || strcmp(pSelectorName, "selector_role_okBtn") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnEnterGame);
    return NULL;
}

bool CSelectRoleAvatorScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_003_btn", CCControlButton*, this->m_pBtnRole003);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRole_003", CCControlButton*, this->m_pBtnRole003);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_004_btn", CCControlButton*, this->m_pBtnRole004);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRole_004", CCControlButton*, this->m_pBtnRole004);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_005_btn", CCControlButton*, this->m_pBtnRole005);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRole_005", CCControlButton*, this->m_pBtnRole005);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_rollNameBtn", CCControlButton*, this->m_pBtnRollName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRollName", CCControlButton*, this->m_pBtnRollName);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_okBtn", CCControlButton*, this->m_pBtnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Candidate_okBtn", CCControlButton*, this->m_pBtnOk);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnOk", CCControlButton*, this->m_pBtnOk);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_003_desc", CCNode*, this->m_pDesc003);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Candidate_003_desc", CCNode*, this->m_pDesc003);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_004_desc", CCNode*, this->m_pDesc004);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Candidate_004_desc", CCNode*, this->m_pDesc004);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_005_desc", CCNode*, this->m_pDesc005);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Candidate_005_desc", CCNode*, this->m_pDesc005);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_cardFixNode", CCNode*, this->m_pCardFixNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_nameFixNode", CCNode*, this->m_pNameFixNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "selector_role_inputName", CCNode*, this->m_pInputNameNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Candidate_inputname", CCNode*, this->m_pInputNameNode);

    return false;
}

// -------------------------------------------------------------
// SỰ KIỆN NÚT BẤM (Actions)
// -------------------------------------------------------------
void CSelectRoleAvatorScene::onBtnSelectNinja003(CCObject* pSender, CCControlEvent pEvent) {
    selectNinja(3);
}

void CSelectRoleAvatorScene::onBtnSelectNinja004(CCObject* pSender, CCControlEvent pEvent) {
    selectNinja(4);
}

void CSelectRoleAvatorScene::onBtnSelectNinja005(CCObject* pSender, CCControlEvent pEvent) {
    selectNinja(5);
}

void CSelectRoleAvatorScene::onBtnRandName(CCObject* pSender, CCControlEvent pEvent) {
    rollRandomName();
}

void CSelectRoleAvatorScene::onBtnSelectNinja003Menu(CCObject* pSender) {
    selectNinja(3);
}

void CSelectRoleAvatorScene::onBtnSelectNinja004Menu(CCObject* pSender) {
    selectNinja(4);
}

void CSelectRoleAvatorScene::onBtnSelectNinja005Menu(CCObject* pSender) {
    selectNinja(5);
}

void CSelectRoleAvatorScene::onBtnRandNameMenu(CCObject* pSender) {
    rollRandomName();
}

void CSelectRoleAvatorScene::onBtnEnterGameMenu(CCObject* pSender) {
    onBtnEnterGame(pSender, CCControlEventTouchUpInside);
}

void CSelectRoleAvatorScene::onBtnBackMenu(CCObject* pSender) {
    onBtnBack(pSender, CCControlEventTouchUpInside);
}

void CSelectRoleAvatorScene::onBtnEnterGame(CCObject* pSender, CCControlEvent pEvent) {
    std::string nickName = m_pEditName ? m_pEditName->getText() : "";
    if (nickName.empty() || nickName.length() < 2) {
        rollRandomName();
        nickName = m_pEditName ? m_pEditName->getText() : "Ninja_Konoha";
    }

    if (m_pLabelStatus) {
        m_pLabelStatus->setColor(ccc3(245, 158, 11));
        m_pLabelStatus->setString("Đang khởi tạo nhân vật...");
    }

    sendCreateRoleRequest(nickName);
}

void CSelectRoleAvatorScene::onBtnBack(CCObject* pSender, CCControlEvent pEvent) {
    CCScene* pCountryScene = CSelectAvatorScene::scene();
    if (pCountryScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.4f, pCountryScene));
    }
}

// -------------------------------------------------------------
// GIAO TIẾP MẠNG (Tạo tướng -> Tải dữ liệu -> Vào Làng)
// -------------------------------------------------------------
void CSelectRoleAvatorScene::sendCreateRoleRequest(const std::string& nickName) {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();

    std::string domain = curServer.domain;
    if (domain.empty() || domain.find("http") == std::string::npos) {
        domain = "http://160.22.123.62:8088";
    }
    while (!domain.empty() && domain.back() == '/') {
        domain.pop_back();
    }

    if (pData->getUserId() == 0) {
        int tempUid = 10000 + (abs((int)time(NULL)) % 90000);
        pData->setUserId(tempUid);
    }
    if (pData->getSessionToken().empty()) {
        pData->setSessionToken("sess_local_init");
    }
    pData->setNickname(nickName);
    pData->setCountryType(m_country > 0 ? m_country : 1);
    pData->setAvatarId(m_selectedCardId > 0 ? m_selectedCardId : 56);

    CCLog("[CSelectRoleAvatorScene] Gửi gói tin tạo nhân vật: Nick=%s, Country=%d, Card=%d, UID=%d, URL=%s",
          nickName.c_str(), m_country, m_selectedCardId, pData->getUserId(), domain.c_str());

    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(domain + "/rl_w_reg2");
    pReq->setCMD(2100);
    pReq->addData("Cmd", 2100);
    pReq->addData("Uin", pData->getUserId());
    pReq->addData("Session", pData->getSessionToken().c_str());
    pReq->addData("Nick", nickName.c_str());
    pReq->addData("Country", m_country > 0 ? m_country : 1);
    pReq->addData("Card", m_selectedCardId > 0 ? m_selectedCardId : 56);
    pReq->addData("ServerID", curServer.id > 0 ? curServer.id : 1);
    pReq->setDelegate(this);
    pReq->start();
}

void CSelectRoleAvatorScene::sendFetchMainpageRequest() {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();

    std::string domain = curServer.domain;
    if (domain.empty() || domain.find("http") == std::string::npos) {
        domain = "http://160.22.123.62:8088";
    }
    while (!domain.empty() && domain.back() == '/') {
        domain.pop_back();
    }

    if (m_pLabelStatus) {
        m_pLabelStatus->setString("Đang nạp dữ liệu Sảnh Làng...");
    }

    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(domain + "/rl_r_mainpage");
    pReq->setCMD(1302);
    pReq->addData("Cmd", 1302);
    pReq->addData("Uin", pData->getUserId());
    pReq->addData("Session", pData->getSessionToken().c_str());
    pReq->addData("ServerID", curServer.id > 0 ? curServer.id : 1);
    pReq->setDelegate(this);
    pReq->start();
}

void CSelectRoleAvatorScene::onHttpSuccess(CRLRequest* pRequest, const std::string& responseData) {
    if (!pRequest) return;
    int cmd = pRequest->getCMD();
    CCLog("[CSelectRoleAvatorScene] onHttpSuccess: CMD=%d", cmd);

    if (cmd == 2100) {
        // Tạo nhân vật thành công -> Tiếp tục nạp thông tin Sảnh Làng
        CCLog("[CSelectRoleAvatorScene] Tạo nhân vật thành công! Đang nạp dữ liệu Sảnh Làng...");
        sendFetchMainpageRequest();
    } else if (cmd == 1302) {
        // Nạp thông tin Sảnh Làng thành công -> Phân tích và chuyển cảnh
        CCLog("[CSelectRoleAvatorScene] Nạp thành công dữ liệu người chơi từ /rl_r_mainpage!");
        CPlayerDataMgr::sharedManager()->parseLoginXml(responseData);
        enterMainGame();
    }
}

void CSelectRoleAvatorScene::onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg) {
    CCLog("[CSelectRoleAvatorScene] Lỗi kết nối mạng: Code=%d, Msg=%s -> Fallback vào Làng", errorCode, errorMsg.c_str());
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (m_pEditName && strlen(m_pEditName->getText()) > 0) {
        pData->setNickname(m_pEditName->getText());
    } else if (pData->getNickname().empty()) {
        pData->setNickname("Ninja_Konoha");
    }
    pData->setCountryType(m_country > 0 ? m_country : 1);
    pData->setAvatarId(m_selectedCardId > 0 ? m_selectedCardId : 56);

    if (m_pLabelStatus) {
        m_pLabelStatus->setString("Đang vào Làng...");
    }
    enterMainGame();
}

void CSelectRoleAvatorScene::enterMainGame() {
    CCLog("[CSelectRoleAvatorScene] Chuyển cảnh vào Sảnh Làng (CMainMenu)!");
    CCScene* pScene = CMainMenu::scene();
    if (pScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pScene));
    }
}
