#include "SelectLoginVIew.h"
#include "CCBManager.h"
#include "CLoginScene.h"
#include <sstream>
#include <ctime>

SelectLoginVIew::SelectLoginVIew()
    : m_pLoginDelegate(NULL)
    , m_pBtnLoginAccount(NULL)
    , m_pBtnLoginGuest(NULL)
    , m_pBtnLoginFB(NULL)
    , m_pBtnClose(NULL)
{
}

SelectLoginVIew::~SelectLoginVIew() {
    CC_SAFE_RELEASE_NULL(m_pBtnLoginAccount);
    CC_SAFE_RELEASE_NULL(m_pBtnLoginGuest);
    CC_SAFE_RELEASE_NULL(m_pBtnLoginFB);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
}

SelectLoginVIew* SelectLoginVIew::create(CLoginScene* pDelegate) {
    SelectLoginVIew* p = new SelectLoginVIew();
    if (p && p->init(pDelegate)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return NULL;
}

bool SelectLoginVIew::init(CLoginScene* pDelegate) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    m_pLoginDelegate = pDelegate;
    this->setTouchEnabled(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Nạp giao diện nguyên bản SelectLoginView.ccbi
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("SelectLoginView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ccbi/SelectLoginView.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 1);
        CCLog("[SelectLoginVIew] Nạp SelectLoginView.ccbi thành công!");
    }

    // Bảo vệ gắn trực tiếp sự kiện vào nút bấm
    if (m_pBtnLoginAccount) {
        m_pBtnLoginAccount->setTouchPriority(-130);
        m_pBtnLoginAccount->addTargetWithActionForControlEvents(this, cccontrol_selector(SelectLoginVIew::onBtnLoginAccount), CCControlEventTouchUpInside);
    }
    if (m_pBtnLoginGuest) {
        m_pBtnLoginGuest->setTouchPriority(-130);
        m_pBtnLoginGuest->addTargetWithActionForControlEvents(this, cccontrol_selector(SelectLoginVIew::onBtnLoginGuest), CCControlEventTouchUpInside);
    }
    if (m_pBtnLoginFB) {
        m_pBtnLoginFB->setTouchPriority(-130);
        m_pBtnLoginFB->addTargetWithActionForControlEvents(this, cccontrol_selector(SelectLoginVIew::onBtnLoginFB), CCControlEventTouchUpInside);
    }
    if (m_pBtnClose) {
        m_pBtnClose->setTouchPriority(-130);
        m_pBtnClose->addTargetWithActionForControlEvents(this, cccontrol_selector(SelectLoginVIew::onBtnClose), CCControlEventTouchUpInside);
    }

    return true;
}

void SelectLoginVIew::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool SelectLoginVIew::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; // Nuốt touch nền modal
}

SEL_MenuHandler SelectLoginVIew::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnLoginAccount") == 0) return menu_selector(SelectLoginVIew::onBtnLoginAccountMenu);
    if (strcmp(pSelectorName, "BtnLoginGuest") == 0) return menu_selector(SelectLoginVIew::onBtnLoginGuestMenu);
    if (strcmp(pSelectorName, "BtnLoginFB") == 0) return menu_selector(SelectLoginVIew::onBtnLoginFBMenu);
    if (strcmp(pSelectorName, "BtnClose") == 0) return menu_selector(SelectLoginVIew::onBtnCloseMenu);
    return NULL;
}

SEL_CCControlHandler SelectLoginVIew::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnLoginAccount") == 0) return cccontrol_selector(SelectLoginVIew::onBtnLoginAccount);
    if (strcmp(pSelectorName, "BtnLoginGuest") == 0) return cccontrol_selector(SelectLoginVIew::onBtnLoginGuest);
    if (strcmp(pSelectorName, "BtnLoginFB") == 0) return cccontrol_selector(SelectLoginVIew::onBtnLoginFB);
    if (strcmp(pSelectorName, "BtnClose") == 0) return cccontrol_selector(SelectLoginVIew::onBtnClose);
    return NULL;
}

bool SelectLoginVIew::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnLoginAccount", CCControlButton*, this->m_pBtnLoginAccount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnLoginGuest", CCControlButton*, this->m_pBtnLoginGuest);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnLoginFB", CCControlButton*, this->m_pBtnLoginFB);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, this->m_pBtnClose);
    return false;
}

void SelectLoginVIew::onBtnLoginAccount(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[SelectLoginVIew] onBtnLoginAccount");
    if (m_pLoginDelegate) {
        m_pLoginDelegate->showAccountDialog();
    }
    this->removeFromParentAndCleanup(true);
}

void SelectLoginVIew::onBtnLoginGuest(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[SelectLoginVIew] onBtnLoginGuest");
    std::string savedGuest = CCUserDefault::sharedUserDefault()->getStringForKey("last_guest_account", "");
    if (savedGuest.empty()) {
        std::stringstream ss;
        ss << "guest_" << (time(NULL) % 100000);
        savedGuest = ss.str();
        CCUserDefault::sharedUserDefault()->setStringForKey("last_guest_account", savedGuest);
        CCUserDefault::sharedUserDefault()->flush();
    }

    if (m_pLoginDelegate) {
        m_pLoginDelegate->setAccount(savedGuest);
        m_pLoginDelegate->doLogin(savedGuest, "123456");
    }
    this->removeFromParentAndCleanup(true);
}

void SelectLoginVIew::onBtnLoginFB(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[SelectLoginVIew] onBtnLoginFB");
    onBtnLoginGuest(pSender, pEvent);
}

void SelectLoginVIew::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[SelectLoginVIew] onBtnClose");
    this->removeFromParentAndCleanup(true);
}

void SelectLoginVIew::onBtnLoginAccountMenu(CCObject* pSender) {
    onBtnLoginAccount(pSender, CCControlEventTouchUpInside);
}

void SelectLoginVIew::onBtnLoginGuestMenu(CCObject* pSender) {
    onBtnLoginGuest(pSender, CCControlEventTouchUpInside);
}

void SelectLoginVIew::onBtnLoginFBMenu(CCObject* pSender) {
    onBtnLoginFB(pSender, CCControlEventTouchUpInside);
}

void SelectLoginVIew::onBtnCloseMenu(CCObject* pSender) {
    onBtnClose(pSender, CCControlEventTouchUpInside);
}
