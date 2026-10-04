#include "CRegisterView.h"
#include "CCBManager.h"
#include "CLoginScene.h"
#include "CBindAccountView.h"

CRegisterView::CRegisterView()
    : m_pLoginScene(NULL)
    , m_pFixNode(NULL)
    , m_pSpriteName(NULL)
    , m_pSpritePwd(NULL)
    , m_pSpritePwd1(NULL)
    , m_pBtnReg(NULL)
    , m_pBtnBack(NULL)
    , m_pBtnClose(NULL)
    , m_pLabelTip(NULL)
    , m_pEditUser(NULL)
    , m_pEditPwd(NULL)
    , m_pEditPwd1(NULL)
{
}

CRegisterView::~CRegisterView() {
    CC_SAFE_RELEASE_NULL(m_pFixNode);
    CC_SAFE_RELEASE_NULL(m_pSpriteName);
    CC_SAFE_RELEASE_NULL(m_pSpritePwd);
    CC_SAFE_RELEASE_NULL(m_pSpritePwd1);
    CC_SAFE_RELEASE_NULL(m_pBtnReg);
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pLabelTip);
}

CRegisterView* CRegisterView::create(CLoginScene* pScene) {
    CRegisterView* p = new CRegisterView();
    if (p && p->init(pScene)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return NULL;
}

bool CRegisterView::init(CLoginScene* pScene) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 190))) {
        return false;
    }

    m_pLoginScene = pScene;
    this->setTouchEnabled(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Nạp sẵn sprite frames trước khi CCBReader nạp RegisterView.ccbi
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ccbResources/regist.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("regist.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("com_res/Resident.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Resident.plist");

    // Nạp giao diện nguyên bản RegisterView.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("RegisterView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/RegisterView.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(CCPointZero);
        this->addChild(pNode, 1);
        if (!m_pFixNode && pNode->getChildren() && pNode->getChildren()->count() > 0) {
            m_pFixNode = (CCNode*)pNode->getChildren()->objectAtIndex(0);
        }
        if (m_pFixNode) {
            m_pFixNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        }
        CCLog("[CRegisterView] Nạp RegisterView.ccbi thành công và căn giữa màn hình!");
    }

    // Nhãn báo lỗi / trạng thái
    m_pLabelTip = CCLabelTTF::create("", "Helvetica", 20.0f);
    m_pLabelTip->setColor(ccc3(239, 68, 68)); // Màu đỏ cảnh báo
    m_pLabelTip->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f - 180.0f));
    this->addChild(m_pLabelTip, 2);

    // CCEditBox: Tên tài khoản mới
    if (m_pSpriteName) {
        CCSize boxSize = CCSizeMake(280.0f, 48.0f);
        CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("reg_inputbtn");
        CCScale9Sprite* pBg = pFrame ? CCScale9Sprite::createWithSpriteFrame(pFrame) : CCScale9Sprite::create("com_res/reg_inputbtn.png");
        if (!pBg) pBg = CCScale9Sprite::create();
        m_pEditUser = CCEditBox::create(boxSize, pBg);
        if (m_pEditUser) {
            m_pEditUser->setPosition(ccp(m_pSpriteName->getContentSize().width * 0.5f, m_pSpriteName->getContentSize().height * 0.5f));
            m_pEditUser->setFontName("Helvetica");
            m_pEditUser->setFontSize(22);
            m_pEditUser->setFontColor(ccWHITE);
            m_pEditUser->setPlaceholderFontColor(ccc3(180, 180, 180));
            m_pEditUser->setPlaceHolder("Tài khoản mới (>=3 ký tự)...");
            m_pEditUser->setMaxLength(24);
            m_pEditUser->setInputMode(kEditBoxInputModeSingleLine);
            m_pEditUser->setReturnType(kKeyboardReturnTypeDone);
            m_pEditUser->setDelegate(this);
            m_pEditUser->setTouchPriority(-131);
            m_pSpriteName->addChild(m_pEditUser);
        }
    }

    // CCEditBox: Mật khẩu mới
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
            m_pEditPwd->setPlaceHolder("Mật khẩu mới (>=4 ký tự)...");
            m_pEditPwd->setMaxLength(24);
            m_pEditPwd->setInputFlag(kEditBoxInputFlagPassword);
            m_pEditPwd->setInputMode(kEditBoxInputModeSingleLine);
            m_pEditPwd->setReturnType(kKeyboardReturnTypeDone);
            m_pEditPwd->setDelegate(this);
            m_pEditPwd->setTouchPriority(-131);
            m_pSpritePwd->addChild(m_pEditPwd);
        }
    }

    // CCEditBox: Xác nhận mật khẩu
    if (m_pSpritePwd1) {
        CCSize boxSize = CCSizeMake(280.0f, 48.0f);
        CCSpriteFrame* pFramePwd1 = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("reg_inputbtn");
        CCScale9Sprite* pBgPwd1 = pFramePwd1 ? CCScale9Sprite::createWithSpriteFrame(pFramePwd1) : CCScale9Sprite::create("com_res/reg_inputbtn.png");
        if (!pBgPwd1) pBgPwd1 = CCScale9Sprite::create();
        m_pEditPwd1 = CCEditBox::create(boxSize, pBgPwd1);
        if (m_pEditPwd1) {
            m_pEditPwd1->setPosition(ccp(m_pSpritePwd1->getContentSize().width * 0.5f, m_pSpritePwd1->getContentSize().height * 0.5f));
            m_pEditPwd1->setFontName("Helvetica");
            m_pEditPwd1->setFontSize(22);
            m_pEditPwd1->setFontColor(ccWHITE);
            m_pEditPwd1->setPlaceholderFontColor(ccc3(180, 180, 180));
            m_pEditPwd1->setPlaceHolder("Nhập lại mật khẩu...");
            m_pEditPwd1->setMaxLength(24);
            m_pEditPwd1->setInputFlag(kEditBoxInputFlagPassword);
            m_pEditPwd1->setInputMode(kEditBoxInputModeSingleLine);
            m_pEditPwd1->setReturnType(kKeyboardReturnTypeDone);
            m_pEditPwd1->setDelegate(this);
            m_pEditPwd1->setTouchPriority(-131);
            m_pSpritePwd1->addChild(m_pEditPwd1);
        }
    }

    // Ưu tiên sự kiện nút bấm trên modal
    if (m_pBtnReg) {
        m_pBtnReg->setTouchPriority(-131);
        m_pBtnReg->addTargetWithActionForControlEvents(this, cccontrol_selector(CRegisterView::onBtnReg), CCControlEventTouchUpInside);
    }
    if (m_pBtnBack) {
        m_pBtnBack->setTouchPriority(-131);
        m_pBtnBack->addTargetWithActionForControlEvents(this, cccontrol_selector(CRegisterView::onBtnBack), CCControlEventTouchUpInside);
    }
    if (m_pBtnClose) {
        m_pBtnClose->setTouchPriority(-131);
        m_pBtnClose->addTargetWithActionForControlEvents(this, cccontrol_selector(CRegisterView::onBtnClose), CCControlEventTouchUpInside);
    }

    return true;
}

void CRegisterView::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -130, true);
}

bool CRegisterView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; // Nuốt touch nền
}

SEL_MenuHandler CRegisterView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnReg") == 0 || strcmp(pSelectorName, "ctrl_reg") == 0) return menu_selector(CRegisterView::onBtnRegMenu);
    if (strcmp(pSelectorName, "BtnBack") == 0 || strcmp(pSelectorName, "ctrl_back") == 0) return menu_selector(CRegisterView::onBtnBackMenu);
    if (strcmp(pSelectorName, "BtnClose") == 0 || strcmp(pSelectorName, "ctrl_close") == 0) return menu_selector(CRegisterView::onBtnCloseMenu);
    return NULL;
}

SEL_CCControlHandler CRegisterView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnReg") == 0 || strcmp(pSelectorName, "ctrl_reg") == 0) return cccontrol_selector(CRegisterView::onBtnReg);
    if (strcmp(pSelectorName, "BtnBack") == 0 || strcmp(pSelectorName, "ctrl_back") == 0) return cccontrol_selector(CRegisterView::onBtnBack);
    if (strcmp(pSelectorName, "BtnClose") == 0 || strcmp(pSelectorName, "ctrl_close") == 0) return cccontrol_selector(CRegisterView::onBtnClose);
    return NULL;
}

bool CRegisterView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "fix_node", CCNode*, this->m_pFixNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_name", CCNode*, this->m_pSpriteName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_pwd", CCNode*, this->m_pSpritePwd);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_pwd1", CCNode*, this->m_pSpritePwd1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnReg", CCControlButton*, this->m_pBtnReg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ctrl_reg", CCControlButton*, this->m_pBtnReg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBack", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ctrl_back", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ctrl_close", CCControlButton*, this->m_pBtnClose);
    return false;
}

void CRegisterView::onBtnReg(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRegisterView] onBtnReg");
    std::string user = m_pEditUser ? m_pEditUser->getText() : "";
    std::string pwd = m_pEditPwd ? m_pEditPwd->getText() : "";
    std::string pwd1 = m_pEditPwd1 ? m_pEditPwd1->getText() : "";

    if (user.length() < 3) {
        if (m_pLabelTip) m_pLabelTip->setString("Tên tài khoản tối thiểu 3 ký tự!");
        return;
    }
    if (pwd.length() < 4) {
        if (m_pLabelTip) m_pLabelTip->setString("Mật khẩu tối thiểu 4 ký tự!");
        return;
    }
    if (pwd != pwd1) {
        if (m_pLabelTip) m_pLabelTip->setString("Mật khẩu xác nhận không khớp!");
        return;
    }

    CCUserDefault::sharedUserDefault()->setStringForKey("last_account", user);
    CCUserDefault::sharedUserDefault()->setStringForKey("last_password", pwd);
    CCUserDefault::sharedUserDefault()->flush();

    if (m_pLoginScene) {
        m_pLoginScene->setAccount(user);
        m_pLoginScene->doRegister(user, pwd);
    }
    this->removeFromParentAndCleanup(true);
}

void CRegisterView::onBtnBack(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRegisterView] Quay lại màn hình Đăng Nhập (CBindAccountView)");
    if (m_pLoginScene) {
        CBindAccountView* pLogin = CBindAccountView::create(m_pLoginScene);
        if (pLogin) {
            m_pLoginScene->addChild(pLogin, 999);
        }
    }
    this->removeFromParentAndCleanup(true);
}

void CRegisterView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRegisterView] onBtnClose");
    this->removeFromParentAndCleanup(true);
}

void CRegisterView::onBtnRegMenu(CCObject* pSender) {
    onBtnReg(pSender, CCControlEventTouchUpInside);
}

void CRegisterView::onBtnBackMenu(CCObject* pSender) {
    onBtnBack(pSender, CCControlEventTouchUpInside);
}

void CRegisterView::onBtnCloseMenu(CCObject* pSender) {
    onBtnClose(pSender, CCControlEventTouchUpInside);
}
