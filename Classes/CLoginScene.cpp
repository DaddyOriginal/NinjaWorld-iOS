#include "CLoginScene.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include "CMainMenu.h"
#include <sstream>

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

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Nạp giao diện LoginView.ccbi với this làm Owner
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("LoginView.ccbi", this);
    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode);
        CCLog("[CLoginScene] Nap LoginView.ccbi thanh cong!");
    } else {
        CCLog("[CLoginScene] Dung UI Fallback co ban");
        CCLabelTTF* pTitle = CCLabelTTF::create("NINJA WORLD - LOGIN", "Helvetica-Bold", 36.0f);
        pTitle->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.7f));
        this->addChild(pTitle);

        CCMenuItemFont* pItemLogin = CCMenuItemFont::create("VÀO GAME", this, menu_selector(CLoginScene::onBtnLogin));
        pItemLogin->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.4f));

        CCMenu* pMenu = CCMenu::create(pItemLogin, NULL);
        pMenu->setPosition(CCPointZero);
        this->addChild(pMenu);
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

    // 3. Tự động gọi API lấy danh sách server động từ Gateway nếu danh sách chưa có
    if (CServerListMgr::sharedManager()->getServerList().empty()) {
        requestServerList();
    }
}

void CLoginScene::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CLoginScene::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CLoginScene::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnLogin") == 0) {
        return cccontrol_selector(CLoginScene::onBtnLogin);
    }
    if (strcmp(pSelectorName, "onBtnRegist") == 0) {
        return cccontrol_selector(CLoginScene::onBtnRegist);
    }
    if (strcmp(pSelectorName, "onBtnSelectServer") == 0) {
        return cccontrol_selector(CLoginScene::onBtnSelectServer);
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
// SỰ KIỆN NÚT BẤM
// -------------------------------------------------------------
void CLoginScene::onBtnLogin(CCObject* pSender, CCControlEvent pCCControlEvent) {
    doLogin(m_username, m_password);
}

void CLoginScene::onBtnRegist(CCObject* pSender, CCControlEvent pCCControlEvent) {
    doRegister(m_username, m_password);
}

void CLoginScene::onBtnSelectServer(CCObject* pSender, CCControlEvent pCCControlEvent) {
    // Mở popup chọn máy chủ
    CServerSelector* pSelector = CServerSelector::create(this);
    if (pSelector) {
        // Nạp danh sách server hiện có
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

// -------------------------------------------------------------
// SERVER SELECT DELEGATE
// -------------------------------------------------------------
void CLoginScene::onServerSelected(int serverId, const std::string& serverName, const std::string& hostUrl) {
    CServerListMgr::sharedManager()->setSelectConfigById(serverId);
    if (m_pLabelServerName) {
        m_pLabelServerName->setString(serverName.c_str());
    }
    CCLog("[CLoginScene] Nguoi choi da chuyen sang Server ID: %d (%s)", serverId, serverName.c_str());
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
    CCLog("[CLoginScene] Dang nhap vao Server ID: %d (%s) tai URL: %s/xk_w_login", 
          curServer.id, curServer.name.c_str(), curServer.domain.c_str());

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
        // Lưu tài khoản hợp lệ
        CCUserDefault::sharedUserDefault()->setStringForKey("last_account", m_username);
        CCUserDefault::sharedUserDefault()->flush();

        // Nạp dữ liệu người chơi vào CPlayerDataMgr của Server đã chọn
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
        CCLog("[CLoginScene] Dang ky thanh cong! Tu dong dang nhap...");
        doLogin(m_username, m_password);
    } else if (cmd == 1003) { // Server List (/xk_r_dir)
        CCLog("[CLoginScene] Nhan danh sach server tu Gateway thanh cong");
        CServerListMgr::sharedManager()->parseServerListXml(responseData);
        
        // Cập nhật lại nhãn tên server
        ServerInfoData curServer = CServerListMgr::sharedManager()->getSelectConfig();
        if (m_pLabelServerName) {
            m_pLabelServerName->setString(curServer.name.c_str());
        }
    }
}

void CLoginScene::onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg) {
    CCLog("[CLoginScene] Ket noi that bai! Code: %d, Msg: %s", errorCode, errorMsg.c_str());
    if (m_pLabelVersionInfo) {
        std::stringstream ss;
        ss << "Lỗi kết nối Server (" << errorCode << ")! Kiểm tra mạng.";
        m_pLabelVersionInfo->setString(ss.str().c_str());
    }
}

void CLoginScene::enterMainGame() {
    CCLog("[CLoginScene] Chuyen sang Sanh Chinh (CMainMenu)!");
    CCScene* pScene = CMainMenu::scene();
    if (pScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pScene));
    }
}
