#include "CBindAccountView.h"
#include "CCBManager.h"
#include "CLoginScene.h"
#include "CRegisterView.h"

CBindAccountView::CBindAccountView()
    : m_pLoginScene(NULL)
    , m_pSpriteName(NULL)
    , m_pSpritePwd(NULL)
    , m_pBtnLogin(NULL)
    , m_pBtnRegist(NULL)
    , m_pBtnClose(NULL)
    , m_pLabelTitle(NULL)
    , m_pEditUser(NULL)
    , m_pEditPwd(NULL)
{
}

CBindAccountView::~CBindAccountView() {
    CC_SAFE_RELEASE_NULL(m_pSpriteName);
    CC_SAFE_RELEASE_NULL(m_pSpritePwd);
    CC_SAFE_RELEASE_NULL(m_pBtnLogin);
    CC_SAFE_RELEASE_NULL(m_pBtnRegist);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);
}

CBindAccountView* CBindAccountView::create(CLoginScene* pScene) {
    CBindAccountView* p = new CBindAccountView();
    if (p && p->init(pScene)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return NULL;
}

bool CBindAccountView::init(CLoginScene* pScene) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 190))) {
        return false;
    }

    m_pLoginScene = pScene;
    this->setTouchEnabled(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Nạp sẵn sprite frames trước khi CCBReader nạp BindAccountView.ccbi
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/regist.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("regist.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("com_res/Resident.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Resident.plist");

    // Nạp giao diện nguyên bản BindAccountView.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("BindAccountView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/BindAccountView.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 1);
        CCLog("[CBindAccountView] Nạp BindAccountView.ccbi thành công!");
    }

    // Thiết lập CCEditBox cho Ô Tên tài khoản
    std::string savedUser = CCUserDefault::sharedUserDefault()->getStringForKey("last_account", "admin");
    if (m_pSpriteName) {
        CCSize boxSize = CCSizeMake(280.0f, 48.0f);
        CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("reg_inputbtn");
        CCScale9Sprite* pBgUser = pFrame ? CCScale9Sprite::createWithSpriteFrame(pFrame) : CCScale9Sprite::create("com_res/reg_inputbtn.png");
        if (!pBgUser) pBgUser = CCScale9Sprite::create();
        m_pEditUser = CCEditBox::create(boxSize, pBgUser);
        if (m_pEditUser) {
            m_pEditUser->setPosition(ccp(m_pSpriteName->getContentSize().width * 0.5f, m_pSpriteName->getContentSize().height * 0.5f));
            m_pEditUser->setFontName("Helvetica");
            m_pEditUser->setFontSize(22);
            m_pEditUser->setFontColor(ccWHITE);
            m_pEditUser->setPlaceholderFontColor(ccc3(180, 180, 180));
            m_pEditUser->setPlaceHolder("Nhập tài khoản...");
            m_pEditUser->setText(savedUser.c_str());
            m_pEditUser->setMaxLength(24);
            m_pEditUser->setInputMode(kEditBoxInputModeSingleLine);
            m_pEditUser->setReturnType(kKeyboardReturnTypeDone);
            m_pEditUser->setDelegate(this);
            m_pEditUser->setTouchPriority(-131);
            m_pSpriteName->addChild(m_pEditUser);
        }
    }

    // Thiết lập CCEditBox cho Ô Mật khẩu
    std::string savedPwd = CCUserDefault::sharedUserDefault()->getStringForKey("last_password", "123456");
    if (m_pSpritePwd) {
        CCSize boxSize = CCSizeMake(280.0f, 48.0f);
        CCSpriteFrame* pFramePwd = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("reg_inputbtn");
        CCScale9Sprite* pBgPwd = pFramePwd ? CCScale9Sprite::createWithSpriteFrame(pFramePwd) : CCScale9Sprite::create("com_res/reg_inputbtn.png");
        if (!pBgPwd) pBgPwd = CCScale9Sprite::create();
        m_pEditPwd = CCEditBox::create(boxSize, pBgPwd);
        if (m_pEditPwd) {
            m_pEditPwd->setPosition(ccp(m_pSpritePwd->getContentSize().width * 0.5f, m_pSpritePwd->getContentSize().height * 0.5f));
            m_pEditPwd->setFontName("Helvetica");
            m_pEditPwd->setFontSize(22);
            m_pEditPwd->setFontColor(ccWHITE);
            m_pEditPwd->setPlaceholderFontColor(ccc3(180, 180, 180));
            m_pEditPwd->setPlaceHolder("Nhập mật khẩu...");
            m_pEditPwd->setText(savedPwd.c_str());
            m_pEditPwd->setMaxLength(24);
            m_pEditPwd->setInputFlag(kEditBoxInputFlagPassword);
            m_pEditPwd->setInputMode(kEditBoxInputModeSingleLine);
            m_pEditPwd->setReturnType(kKeyboardReturnTypeDone);
            m_pEditPwd->setDelegate(this);
            m_pEditPwd->setTouchPriority(-131);
            m_pSpritePwd->addChild(m_pEditPwd);
        }
    }

    // Ưu tiên sự kiện nút bấm trên modal
    if (m_pBtnLogin) {
        m_pBtnLogin->setTouchPriority(-131);
        m_pBtnLogin->addTargetWithActionForControlEvents(this, cccontrol_selector(CBindAccountView::onBtnLogin), CCControlEventTouchUpInside);
    }
    if (m_pBtnRegist) {
        m_pBtnRegist->setTouchPriority(-131);
        m_pBtnRegist->addTargetWithActionForControlEvents(this, cccontrol_selector(CBindAccountView::onBtnRegist), CCControlEventTouchUpInside);
    }
    if (m_pBtnClose) {
        m_pBtnClose->setTouchPriority(-131);
        m_pBtnClose->addTargetWithActionForControlEvents(this, cccontrol_selector(CBindAccountView::onBtnClose), CCControlEventTouchUpInside);
    }

    return true;
}

void CBindAccountView::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -130, true);
}

bool CBindAccountView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; // Nuốt touch nền
}

SEL_MenuHandler CBindAccountView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnLogin") == 0 || strcmp(pSelectorName, "ctrl_login") == 0) return menu_selector(CBindAccountView::onBtnLoginMenu);
    if (strcmp(pSelectorName, "BtnRegist") == 0 || strcmp(pSelectorName, "ctrl_create") == 0) return menu_selector(CBindAccountView::onBtnRegistMenu);
    if (strcmp(pSelectorName, "BtnClose") == 0 || strcmp(pSelectorName, "ctrl_close") == 0) return menu_selector(CBindAccountView::onBtnCloseMenu);
    return NULL;
}

SEL_CCControlHandler CBindAccountView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnLogin") == 0 || strcmp(pSelectorName, "ctrl_login") == 0) return cccontrol_selector(CBindAccountView::onBtnLogin);
    if (strcmp(pSelectorName, "BtnRegist") == 0 || strcmp(pSelectorName, "ctrl_create") == 0) return cccontrol_selector(CBindAccountView::onBtnRegist);
    if (strcmp(pSelectorName, "BtnClose") == 0 || strcmp(pSelectorName, "ctrl_close") == 0) return cccontrol_selector(CBindAccountView::onBtnClose);
    return NULL;
}

bool CBindAccountView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_name", CCNode*, this->m_pSpriteName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_pwd", CCNode*, this->m_pSpritePwd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnLogin", CCControlButton*, this->m_pBtnLogin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ctrl_login", CCControlButton*, this->m_pBtnLogin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnRegist", CCControlButton*, this->m_pBtnRegist);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ctrl_create", CCControlButton*, this->m_pBtnRegist);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ctrl_close", CCControlButton*, this->m_pBtnClose);
    return false;
}

void CBindAccountView::onBtnLogin(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CBindAccountView] onBtnLogin");
    std::string user = m_pEditUser ? m_pEditUser->getText() : "";
    std::string pwd = m_pEditPwd ? m_pEditPwd->getText() : "";

    if (user.empty()) {
        CCLog("[CBindAccountView] Tài khoản không được để trống!");
        return;
    }
    if (pwd.empty()) {
        pwd = "123456";
    }

    CCUserDefault::sharedUserDefault()->setStringForKey("last_account", user);
    CCUserDefault::sharedUserDefault()->setStringForKey("last_password", pwd);
    CCUserDefault::sharedUserDefault()->flush();

    if (m_pLoginScene) {
        m_pLoginScene->setAccount(user);
        m_pLoginScene->doLogin(user, pwd);
    }
    this->removeFromParentAndCleanup(true);
}

void CBindAccountView::onBtnRegist(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CBindAccountView] Chuyển sang màn hình Đăng Ký (RegisterView)");
    if (m_pLoginScene) {
        CRegisterView* pReg = CRegisterView::create(m_pLoginScene);
        if (pReg) {
            m_pLoginScene->addChild(pReg, 999);
        }
    }
    this->removeFromParentAndCleanup(true);
}

void CBindAccountView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CBindAccountView] onBtnClose");
    this->removeFromParentAndCleanup(true);
}

void CBindAccountView::onBtnLoginMenu(CCObject* pSender) {
    onBtnLogin(pSender, CCControlEventTouchUpInside);
}

void CBindAccountView::onBtnRegistMenu(CCObject* pSender) {
    onBtnRegist(pSender, CCControlEventTouchUpInside);
}

void CBindAccountView::onBtnCloseMenu(CCObject* pSender) {
    onBtnClose(pSender, CCControlEventTouchUpInside);
}
