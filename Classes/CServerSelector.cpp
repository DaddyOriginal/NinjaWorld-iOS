#include "CServerSelector.h"
#include "CCBManager.h"
#include <sstream>

CServerSelector::CServerSelector()
    : m_pDelegate(NULL)
    , m_pLabelServerName1(NULL)
    , m_pLabelServerName2(NULL)
    , m_pSpriteServerState1(NULL)
    , m_pSpriteServerState2(NULL)
    , m_pNodeListContent(NULL)
    , m_pBtnClose(NULL)
    , m_pBtnServer1(NULL)
    , m_pBtnServer2(NULL)
{
}

CServerSelector::~CServerSelector() {
    CC_SAFE_RELEASE_NULL(m_pLabelServerName1);
    CC_SAFE_RELEASE_NULL(m_pLabelServerName2);
    CC_SAFE_RELEASE_NULL(m_pSpriteServerState1);
    CC_SAFE_RELEASE_NULL(m_pSpriteServerState2);
    CC_SAFE_RELEASE_NULL(m_pNodeListContent);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnServer1);
    CC_SAFE_RELEASE_NULL(m_pBtnServer2);
}

CServerSelector* CServerSelector::create(ServerSelectDelegate* pDelegate) {
    CServerSelector* p = new CServerSelector();
    if (p && p->init(pDelegate)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return NULL;
}

bool CServerSelector::init(ServerSelectDelegate* pDelegate) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    m_pDelegate = pDelegate;
    this->setTouchEnabled(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Nạp giao diện ServerSelector.ccbi nguyên bản
    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("ServerSelector.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("serverinfo/ServerSelector.ccbi", this);
    }

    if (pNode) {
        pNode->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        this->addChild(pNode, 1);
        CCLog("[CServerSelector] Nạp ServerSelector.ccbi thành công!");
    }

    // 2. Bảo vệ gắn trực tiếp sự kiện chạm
    if (m_pBtnClose) {
        m_pBtnClose->setTouchPriority(-130);
        m_pBtnClose->addTargetWithActionForControlEvents(this, cccontrol_selector(CServerSelector::onBtnClose), CCControlEventTouchUpInside);
    }
    if (m_pBtnServer1) {
        m_pBtnServer1->setTouchPriority(-130);
        m_pBtnServer1->addTargetWithActionForControlEvents(this, cccontrol_selector(CServerSelector::onBtnServer1), CCControlEventTouchUpInside);
    }
    if (m_pBtnServer2) {
        m_pBtnServer2->setTouchPriority(-130);
        m_pBtnServer2->addTargetWithActionForControlEvents(this, cccontrol_selector(CServerSelector::onBtnServer2), CCControlEventTouchUpInside);
    }

    // 3. Khởi tạo danh sách mặc định nếu chưa có
    if (m_serverList.empty()) {
        ServerItemInfo s1;
        s1.id = 1;
        s1.name = "S1. Làng Lá";
        s1.ip = "160.22.123.62";
        s1.port = 8088;
        s1.state = 1;

        ServerItemInfo s2;
        s2.id = 2;
        s2.name = "S2. Làng Cát";
        s2.ip = "160.22.123.62";
        s2.port = 8088;
        s2.state = 1;

        m_serverList.push_back(s1);
        m_serverList.push_back(s2);
    }

    refreshUI();
    return true;
}

void CServerSelector::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool CServerSelector::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; // Nuốt touch nền
}

void CServerSelector::setServerList(const std::vector<ServerItemInfo>& list) {
    if (!list.empty()) {
        m_serverList = list;
        refreshUI();
    }
}

void CServerSelector::refreshUI() {
    if (!m_serverList.empty()) {
        if (m_pLabelServerName1) {
            m_pLabelServerName1->setString(m_serverList[0].name.c_str());
        }
    }
    if (m_serverList.size() >= 2) {
        if (m_pLabelServerName2) {
            m_pLabelServerName2->setString(m_serverList[1].name.c_str());
        }
    }
}

SEL_MenuHandler CServerSelector::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnClose") == 0) return menu_selector(CServerSelector::onBtnCloseMenu);
    if (strcmp(pSelectorName, "BtnServer1") == 0) return menu_selector(CServerSelector::onBtnServer1Menu);
    if (strcmp(pSelectorName, "BtnServer2") == 0) return menu_selector(CServerSelector::onBtnServer2Menu);
    return NULL;
}

SEL_CCControlHandler CServerSelector::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "BtnClose") == 0) return cccontrol_selector(CServerSelector::onBtnClose);
    if (strcmp(pSelectorName, "BtnServer1") == 0) return cccontrol_selector(CServerSelector::onBtnServer1);
    if (strcmp(pSelectorName, "BtnServer2") == 0) return cccontrol_selector(CServerSelector::onBtnServer2);
    return NULL;
}

bool CServerSelector::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnClose", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnServer1", CCControlButton*, this->m_pBtnServer1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnServer2", CCControlButton*, this->m_pBtnServer2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_servername1", CCLabelTTF*, this->m_pLabelServerName1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_servername2", CCLabelTTF*, this->m_pLabelServerName2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_serverstate1", CCSprite*, this->m_pSpriteServerState1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_serverstate2", CCSprite*, this->m_pSpriteServerState2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_listcontent", CCNode*, this->m_pNodeListContent);
    return false;
}

void CServerSelector::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CServerSelector::onBtnServer1(CCObject* pSender, CCControlEvent pEvent) {
    if (!m_serverList.empty() && m_pDelegate) {
        std::stringstream ss;
        ss << "http://" << m_serverList[0].ip << ":" << m_serverList[0].port;
        m_pDelegate->onServerSelected(m_serverList[0].id, m_serverList[0].name, ss.str());
    }
    this->removeFromParentAndCleanup(true);
}

void CServerSelector::onBtnServer2(CCObject* pSender, CCControlEvent pEvent) {
    if (m_serverList.size() >= 2 && m_pDelegate) {
        std::stringstream ss;
        ss << "http://" << m_serverList[1].ip << ":" << m_serverList[1].port;
        m_pDelegate->onServerSelected(m_serverList[1].id, m_serverList[1].name, ss.str());
    }
    this->removeFromParentAndCleanup(true);
}

void CServerSelector::onBtnCloseMenu(CCObject* pSender) {
    onBtnClose(pSender, CCControlEventTouchUpInside);
}

void CServerSelector::onBtnServer1Menu(CCObject* pSender) {
    onBtnServer1(pSender, CCControlEventTouchUpInside);
}

void CServerSelector::onBtnServer2Menu(CCObject* pSender) {
    onBtnServer2(pSender, CCControlEventTouchUpInside);
}
