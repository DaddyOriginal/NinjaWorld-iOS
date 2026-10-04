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

    m_country = country;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Nạp giao diện SelectorRoleAvatar.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("SelectorRoleAvatar.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/SelectorRoleAvatar.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 0);
        CCLog("[CSelectRoleAvatorScene] Nap thanh cong SelectorRoleAvatar.ccbi!");
    } else {
        // Fallback UI
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg, 0);

        CCLabelTTF* pTitle = CCLabelTTF::create("CHỌN NHẪN GIẢ KHỞI ĐẦU", "Helvetica-Bold", 32.0f);
        pTitle->setPosition(ccp(winSize.width * 0.5f, winSize.height - 100.0f));
        pTitle->setColor(ccc3(250, 204, 21));
        this->addChild(pTitle, 1);
    }

    // 2. Thiết lập CCEditBox cho ô nhập tên nhân vật
    CCPoint namePos = ccp(winSize.width * 0.5f - 40.0f, winSize.height * 0.22f);
    CCSize nameSize = CCSizeMake(260.0f, 44.0f);

    if (m_pInputNameNode) {
        namePos = m_pInputNameNode->getParent()->convertToWorldSpace(m_pInputNameNode->getPosition());
        if (m_pInputNameNode->getContentSize().width > 100) {
            nameSize = m_pInputNameNode->getContentSize();
        }
    }

    m_pEditName = CCEditBox::create(nameSize, CCScale9Sprite::create("com_res/reg_inputbtn.png"));
    if (!m_pEditName) {
        m_pEditName = CCEditBox::create(nameSize, CCScale9Sprite::create());
    }
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

    // 3. Nhãn thông báo trạng thái
    m_pLabelStatus = CCLabelTTF::create("", "Helvetica", 20.0f);
    m_pLabelStatus->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.16f));
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
    if (strcmp(pSelectorName, "BtnRole_003") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja003);
    if (strcmp(pSelectorName, "BtnRole_004") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja004);
    if (strcmp(pSelectorName, "BtnRole_005") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnSelectNinja005);
    if (strcmp(pSelectorName, "BtnRollName") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnRandName);
    if (strcmp(pSelectorName, "BtnOk") == 0) return menu_selector(CSelectRoleAvatorScene::onBtnEnterGame);
    return NULL;
}

SEL_CCControlHandler CSelectRoleAvatorScene::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnRole_003") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja003);
    if (strcmp(pSelectorName, "BtnRole_004") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja004);
    if (strcmp(pSelectorName, "BtnRole_005") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnSelectNinja005);
    if (strcmp(pSelectorName, "BtnRollName") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnRandName);
    if (strcmp(pSelectorName, "BtnOk") == 0) return cccontrol_selector(CSelectRoleAvatorScene::onBtnEnterGame);
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

void CSelectRoleAvatorScene::onBtnEnterGame(CCObject* pSender, CCControlEvent pEvent) {
    std::string nickName = m_pEditName ? m_pEditName->getText() : "";
    if (nickName.empty() || nickName.length() < 2) {
        if (m_pLabelStatus) {
            m_pLabelStatus->setString("Tên nhân vật phải từ 2 - 16 ký tự!");
        }
        return;
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

    CCLog("[CSelectRoleAvatorScene] Gui goi tin tao nhan vat: Nick=%s, Country=%d, Card=%d, UID=%d",
          nickName.c_str(), m_country, m_selectedCardId, pData->getUserId());

    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(curServer.domain + "/rl_w_reg2");
    pReq->setCMD(2100);
    pReq->addData("Cmd", 2100);
    pReq->addData("Uin", pData->getUserId());
    pReq->addData("Session", pData->getSessionToken().c_str());
    pReq->addData("Nick", nickName.c_str());
    pReq->addData("Country", m_country);
    pReq->addData("Card", m_selectedCardId);
    pReq->addData("ServerID", curServer.id);
    pReq->setDelegate(this);
    pReq->start();
}

void CSelectRoleAvatorScene::sendFetchMainpageRequest() {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();

    if (m_pLabelStatus) {
        m_pLabelStatus->setString("Đang nạp dữ liệu Sảnh Làng...");
    }

    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(curServer.domain + "/rl_r_mainpage");
    pReq->setCMD(1302);
    pReq->addData("Cmd", 1302);
    pReq->addData("Uin", pData->getUserId());
    pReq->addData("Session", pData->getSessionToken().c_str());
    pReq->addData("ServerID", curServer.id);
    pReq->setDelegate(this);
    pReq->start();
}

void CSelectRoleAvatorScene::onHttpSuccess(CRLRequest* pRequest, const std::string& responseData) {
    int cmd = pRequest->getCMD();

    if (cmd == 2100) {
        // Tạo nhân vật thành công -> Tiếp tục nạp thông tin Sảnh Làng
        CCLog("[CSelectRoleAvatorScene] Tao nhan vat thanh cong! Dang nap du lieu Sanh Lang...");
        sendFetchMainpageRequest();
    } else if (cmd == 1302) {
        // Nạp thông tin Sảnh Làng thành công -> Phân tích và chuyển cảnh
        CCLog("[CSelectRoleAvatorScene] Nap thanh cong du lieu nguoi choi tu /rl_r_mainpage!");
        CPlayerDataMgr::sharedManager()->parseLoginXml(responseData);
        enterMainGame();
    }
}

void CSelectRoleAvatorScene::onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg) {
    CCLog("[CSelectRoleAvatorScene] Loi ket noi mang: Code=%d, Msg=%s", errorCode, errorMsg.c_str());
    // Fallback: Nếu mạng máy chủ bận hoặc offline, tự động khởi tạo dữ liệu mặc định và vào game
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (m_pEditName) {
        pData->setNickname(m_pEditName->getText());
    }
    pData->setCountryType(m_country);
    pData->setAvatarId(m_selectedCardId);

    if (m_pLabelStatus) {
        m_pLabelStatus->setString("Đang vào Làng...");
    }
    enterMainGame();
}

void CSelectRoleAvatorScene::enterMainGame() {
    CCLog("[CSelectRoleAvatorScene] Chuyen canh vao Sanh Chinh (CMainMenu)!");
    CCScene* pScene = CMainMenu::scene();
    if (pScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pScene));
    }
}
