#include "CLoginScene.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include "CMainMenu.h"
#include "SelectLoginVIew.h"
#include "CBindAccountView.h"
#include "CRegisterView.h"
#include <sstream>
#include <ctime>

CLoginScene::CLoginScene()
    : m_pLabelServerName(NULL)
    , m_pLabelVersionInfo(NULL)
    , m_pLabelIdName(NULL)
    , m_pBtnLogin(NULL)
    , m_pBtnReg(NULL)
    , m_pBtnSelectServer(NULL)
    , m_username("admin")
    , m_password("123456")
{
}

CLoginScene::~CLoginScene() {
    CC_SAFE_RELEASE_NULL(m_pLabelServerName);
    CC_SAFE_RELEASE_NULL(m_pLabelVersionInfo);
    CC_SAFE_RELEASE_NULL(m_pLabelIdName);
    CC_SAFE_RELEASE_NULL(m_pBtnLogin);
    CC_SAFE_RELEASE_NULL(m_pBtnReg);
    CC_SAFE_RELEASE_NULL(m_pBtnSelectServer);
}

CCScene* CLoginScene::scene() {
    CCScene* pScene = CCScene::create();
    CLoginScene* pLayer = CLoginScene::create();
    if (pScene && pLayer) {
        pScene->addChild(pLayer);
        return pScene;
    }
    return NULL;
}

bool CLoginScene::init() {
    if (!CCLayer::init()) {
        return false;
    }

    this->setTouchEnabled(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Nạp giao diện LoginView.ccbi với this làm Owner
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("LoginView.ccbi", this);
    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 0);
        CCLog("[CLoginScene] Nạp LoginView.ccbi thành công!");
    } else {
        CCLog("[CLoginScene] Dùng UI Fallback cơ bản");
        CCLabelTTF* pTitle = CCLabelTTF::create("NINJA WORLD - LOGIN", "Helvetica-Bold", 36.0f);
        pTitle->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.7f));
        this->addChild(pTitle, 1);

        CCMenuItemFont* pItemLogin = CCMenuItemFont::create("VÀO GAME", this, menu_selector(CLoginScene::onBtnLoginMenu));
        pItemLogin->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.4f));

        CCMenu* pMenu = CCMenu::create(pItemLogin, NULL);
        pMenu->setPosition(CCPointZero);
        this->addChild(pMenu, 2);
    }

    return true;
}

void CLoginScene::onEnter() {
    CCLayer::onEnter();

    // 1. Nạp tài khoản đã lưu
    std::string savedAcc = CCUserDefault::sharedUserDefault()->getStringForKey("last_account", "admin");
    if (!savedAcc.empty()) {
        m_username = savedAcc;
    }

    // 2. Lấy Server đang được chọn từ CServerListMgr
    ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();

    if (m_pLabelVersionInfo) {
        m_pLabelVersionInfo->setString("Phiên bản: 1.0.4");
    }
    if (m_pLabelServerName) {
        m_pLabelServerName->setString(curServer.name.c_str());
    }
    if (m_pLabelIdName) {
        m_pLabelIdName->setString(m_username.c_str());
    }

    // 3. Đảm bảo toàn bộ CCControlButton nhận được sự kiện chạm (Bảo vệ đa tầng)
    if (m_pBtnLogin) {
        m_pBtnLogin->setTouchPriority(-1);
        m_pBtnLogin->addTargetWithActionForControlEvents(this, cccontrol_selector(CLoginScene::onBtnLogin), CCControlEventTouchUpInside);
    }
    if (m_pBtnReg) {
        m_pBtnReg->setTouchPriority(-1);
        m_pBtnReg->addTargetWithActionForControlEvents(this, cccontrol_selector(CLoginScene::onBtnRegist), CCControlEventTouchUpInside);
    }
    if (m_pBtnSelectServer) {
        m_pBtnSelectServer->setTouchPriority(-1);
        m_pBtnSelectServer->addTargetWithActionForControlEvents(this, cccontrol_selector(CLoginScene::onBtnSelectServer), CCControlEventTouchUpInside);
    }

    // 4. Tự động gọi API lấy danh sách server động từ Gateway nếu danh sách chưa có
    if (CServerListMgr::sharedManager()->getServerList().empty()) {
        requestServerList();
    }
}

void CLoginScene::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS (Hỗ trợ chuẩn cả CCControlButton và CCMenuItem)
// -------------------------------------------------------------
SEL_CCControlHandler CLoginScene::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnLogin") == 0 || strcmp(pSelectorName, "onBtnLogin") == 0) {
        return cccontrol_selector(CLoginScene::onBtnLogin);
    }
    if (strcmp(pSelectorName, "BtnReg") == 0 || strcmp(pSelectorName, "BtnRegist") == 0 || strcmp(pSelectorName, "onBtnRegist") == 0) {
        return cccontrol_selector(CLoginScene::onBtnRegist);
    }
    if (strcmp(pSelectorName, "BtnSelectServer") == 0 || strcmp(pSelectorName, "onBtnSelectServer") == 0) {
        return cccontrol_selector(CLoginScene::onBtnSelectServer);
    }
    return NULL;
}

SEL_MenuHandler CLoginScene::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnLogin") == 0 || strcmp(pSelectorName, "onBtnLogin") == 0) {
        return menu_selector(CLoginScene::onBtnLoginMenu);
    }
    if (strcmp(pSelectorName, "BtnReg") == 0 || strcmp(pSelectorName, "BtnRegist") == 0 || strcmp(pSelectorName, "onBtnRegist") == 0) {
        return menu_selector(CLoginScene::onBtnRegistMenu);
    }
    if (strcmp(pSelectorName, "BtnSelectServer") == 0 || strcmp(pSelectorName, "onBtnSelectServer") == 0) {
        return menu_selector(CLoginScene::onBtnSelectServerMenu);
    }
    return NULL;
}

bool CLoginScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_servername", CCLabelTTF*, this->m_pLabelServerName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_versioninfo", CCLabelTTF*, this->m_pLabelVersionInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_idname", CCLabelTTF*, this->m_pLabelIdName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnLogin", CCControlButton*, this->m_pBtnLogin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReg", CCControlButton*, this->m_pBtnReg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnSelectServer", CCControlButton*, this->m_pBtnSelectServer);
    
    return false;
}

// -------------------------------------------------------------
// SỰ KIỆN NÚT BẤM (CCControl & CCMenuItem Callbacks)
// -------------------------------------------------------------
void CLoginScene::onBtnLogin(CCObject* pSender, CCControlEvent pCCControlEvent) {
    CCLog("[CLoginScene] Nút ĐĂNG NHẬP / BẮT ĐẦU được nhấn!");
    doLogin(m_username, m_password);
}

void CLoginScene::onBtnLoginMenu(CCObject* pSender) {
    onBtnLogin(pSender, CCControlEventTouchUpInside);
}

void CLoginScene::onBtnRegist(CCObject* pSender, CCControlEvent pCCControlEvent) {
    CCLog("[CLoginScene] Nút ĐỔI TÀI KHOẢN / ĐĂNG KÝ được nhấn!");
    SelectLoginVIew* pSelect = SelectLoginVIew::create(this);
    if (pSelect) {
        this->addChild(pSelect, 999);
    }
}

void CLoginScene::onBtnRegistMenu(CCObject* pSender) {
    onBtnRegist(pSender, CCControlEventTouchUpInside);
}

void CLoginScene::onBtnSelectServer(CCObject* pSender, CCControlEvent pCCControlEvent) {
    CCLog("[CLoginScene] Nút CHỌN SERVER được nhấn!");
    CServerSelector* pSelector = CServerSelector::create(this);
    if (pSelector) {
        std::vector<ServerItemInfo> items;
        const std::vector<ServerInfoData>& list = CServerListMgr::sharedManager()->getServerList();
        for (size_t i = 0; i < list.size(); ++i) {
            ServerItemInfo it;
            it.id = list[i].id;
            it.name = list[i].name;
            it.ip = list[i].domain;
            it.port = list[i].port;
            it.state = list[i].state;
            items.push_back(it);
        }
        if (!items.empty()) {
            pSelector->setServerList(items);
        }
        this->addChild(pSelector, 999);
    }
}

void CLoginScene::onBtnSelectServerMenu(CCObject* pSender) {
    onBtnSelectServer(pSender, CCControlEventTouchUpInside);
}

// -------------------------------------------------------------
// ĐỔI TÀI KHOẢN / ĐĂNG KÝ (BindAccountView & RegisterView)
// -------------------------------------------------------------
void CLoginScene::showAccountDialog() {
    CBindAccountView* pDlg = CBindAccountView::create(this);
    if (pDlg) {
        this->addChild(pDlg, 999);
    }
}

void CLoginScene::setAccount(const std::string& account) {
    m_username = account;
    CCUserDefault::sharedUserDefault()->setStringForKey("last_account", m_username);
    CCUserDefault::sharedUserDefault()->flush();
    if (m_pLabelIdName) {
        m_pLabelIdName->setString(m_username.c_str());
    }
    CCLog("[CLoginScene] Đã chuyển sang tài khoản: %s", m_username.c_str());
}

// -------------------------------------------------------------
// SERVER SELECT DELEGATE
// -------------------------------------------------------------
void CLoginScene::onServerSelected(int serverId, const std::string& serverName, const std::string& hostUrl) {
    CServerListMgr::sharedManager()->setSelectConfigById(serverId);
    if (m_pLabelServerName) {
        m_pLabelServerName->setString(serverName.c_str());
    }
    CCLog("[CLoginScene] Người chơi đã chọn Server ID: %d (%s)", serverId, serverName.c_str());
}

// -------------------------------------------------------------
// XỬ LÝ GIAO TIẾP MẠNG
// -------------------------------------------------------------
void CLoginScene::requestServerList() {
    std::string gateway = CServerListMgr::sharedManager()->getGatewayUrl();
    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(gateway + "/xk_r_dir");
    pReq->setCMD(1003); // CMD_DIR
    pReq->setDelegate(this);
    pReq->start();
}

void CLoginScene::doLogin(const std::string& account, const std::string& pwd) {
    ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();
    CCLog("[CLoginScene] Đăng nhập vào Server ID: %d (%s) tại URL: %s/xk_w_login (User: %s)", 
          curServer.id, curServer.name.c_str(), curServer.domain.c_str(), account.c_str());

    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(curServer.domain + "/xk_w_login");
    pReq->setCMD(1001); // CMD_LOGIN
    pReq->addData("user", account.c_str());
    pReq->addData("pwd", pwd.c_str());
    pReq->addData("area", curServer.id);
    pReq->setDelegate(this);
    pReq->start();

    if (m_pLabelVersionInfo) {
        m_pLabelVersionInfo->setString("Đang kết nối máy chủ...");
    }
}

void CLoginScene::doRegister(const std::string& account, const std::string& pwd) {
    ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();
    CRLRequest* pReq = CRLRequest::create();
    pReq->setURL(curServer.domain + "/xk_w_reg");
    pReq->setCMD(1002); // CMD_REG
    pReq->addData("user", account.c_str());
    pReq->addData("pwd", pwd.c_str());
    pReq->addData("area", curServer.id);
    pReq->setDelegate(this);
    pReq->start();
}

void CLoginScene::onHttpSuccess(CRLRequest* pRequest, const std::string& responseData) {
    int cmd = pRequest->getCMD();

    if (cmd == 1001) { // Login
        CCUserDefault::sharedUserDefault()->setStringForKey("last_account", m_username);
        CCUserDefault::sharedUserDefault()->flush();

        ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();
        CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
        pData->setServerId(curServer.id);
        pData->setServerName(curServer.name);
        pData->parseLoginXml(responseData);

        if (m_pLabelVersionInfo) {
            m_pLabelVersionInfo->setString("Đăng nhập thành công! Đang vào game...");
        }

        enterMainGame();
    } else if (cmd == 1002) { // Register
        CCLog("[CLoginScene] Đăng ký thành công! Đang đăng nhập...");
        doLogin(m_username, m_password);
    } else if (cmd == 1003) { // Server List (/xk_r_dir)
        CCLog("[CLoginScene] Nhận danh sách server từ Gateway thành công");
        CServerListMgr::sharedManager()->parseServerListXml(responseData);
        
        ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();
        if (m_pLabelServerName) {
            m_pLabelServerName->setString(curServer.name.c_str());
        }
    }
}

void CLoginScene::onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg) {
    CCLog("[CLoginScene] Kết nối thất bại! Code: %d, Msg: %s", errorCode, errorMsg.c_str());
    if (m_pLabelVersionInfo) {
        std::stringstream ss;
        ss << "Lỗi kết nối Server (" << errorCode << ")! Thử lại...";
        m_pLabelVersionInfo->setString(ss.str().c_str());
    }
}

void CLoginScene::enterMainGame() {
    CCLog("[CLoginScene] Chuyển sang Sảnh Chính (CMainMenu)!");
    CCScene* pScene = CMainMenu::scene();
    if (pScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pScene));
    }
}
