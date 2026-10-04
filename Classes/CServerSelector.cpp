#include "CServerSelector.h"
#include <sstream>

CServerSelector::CServerSelector()
    : m_pDelegate(NULL)
    , m_pServerMenu(NULL)
{
}

CServerSelector::~CServerSelector() {
}

CServerSelector* CServerSelector::create(ServerSelectDelegate* pDelegate) {
    CServerSelector* pLayer = new CServerSelector();
    if (pLayer && pLayer->init(pDelegate)) {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool CServerSelector::init(ServerSelectDelegate* pDelegate) {
    // Nền tối mờ 80% che lớp dưới
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 200))) {
        return false;
    }

    m_pDelegate = pDelegate;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Khung popup trung tâm (Background Dialog)
    const float dialogWidth = 560.0f;
    const float dialogHeight = 680.0f;
    const float dialogX = (winSize.width - dialogWidth) * 0.5f;
    const float dialogY = (winSize.height - dialogHeight) * 0.5f;

    CCLayerColor* pDialogBg = CCLayerColor::create(ccc4(24, 32, 47, 245), dialogWidth, dialogHeight);
    pDialogBg->setPosition(ccp(dialogX, dialogY));
    this->addChild(pDialogBg, 1);

    // Tiêu đề Dialog
    CCLabelTTF* pTitle = CCLabelTTF::create("CHỌN CỤM MÁY CHỦ", "Helvetica-Bold", 28.0f);
    pTitle->setPosition(ccp(dialogX + dialogWidth * 0.5f, dialogY + dialogHeight - 50.0f));
    pTitle->setColor(ccc3(245, 158, 11)); // Vàng cam
    this->addChild(pTitle, 2);

    // Nút Đóng / Quay lại
    CCMenuItemFont* pBtnClose = CCMenuItemFont::create("ĐÓNG", this, menu_selector(CServerSelector::onBtnClose));
    pBtnClose->setFontSize(22.0f);
    pBtnClose->setColor(ccc3(239, 68, 68)); // Đỏ
    pBtnClose->setPosition(ccp(dialogX + dialogWidth * 0.5f, dialogY + 50.0f));

    CCMenu* pCloseMenu = CCMenu::create(pBtnClose, NULL);
    pCloseMenu->setPosition(CCPointZero);
    this->addChild(pCloseMenu, 3);

    // Menu danh sách Server bên trong Dialog
    m_pServerMenu = CCMenu::create();
    m_pServerMenu->setPosition(CCPointZero);
    this->addChild(m_pServerMenu, 2);

    // Khởi tạo danh sách mặc định nếu chưa có
    ServerItemInfo s1;
    s1.id = 1;
    s1.name = "S1. Làng Lá (Đề Cử)";
    s1.ip = "160.22.123.62";
    s1.port = 8088;
    s1.state = 1;

    ServerItemInfo s2;
    s2.id = 2;
    s2.name = "S2. Làng Cát (Mới)";
    s2.ip = "160.22.123.62";
    s2.port = 8088;
    s2.state = 1;

    ServerItemInfo s3;
    s3.id = 3;
    s3.name = "S3. Làng Mây (Mới)";
    s3.ip = "160.22.123.62";
    s3.port = 8088;
    s3.state = 1;

    m_serverList.push_back(s1);
    m_serverList.push_back(s2);
    m_serverList.push_back(s3);

    refreshUI();
    return true;
}

void CServerSelector::setServerList(const std::vector<ServerItemInfo>& list) {
    if (!list.empty()) {
        m_serverList = list;
        refreshUI();
    }
}

void CServerSelector::refreshUI() {
    if (!m_pServerMenu) return;
    m_pServerMenu->removeAllChildren();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float startY = (winSize.height + 680.0f) * 0.5f - 140.0f;
    float centerX = winSize.width * 0.5f;

    for (size_t i = 0; i < m_serverList.size(); ++i) {
        const ServerItemInfo& info = m_serverList[i];

        CCMenuItemFont* pItem = CCMenuItemFont::create(info.name.c_str(), this, menu_selector(CServerSelector::onBtnSelectServer));
        pItem->setFontSize(24.0f);
        pItem->setTag(info.id);
        pItem->setPosition(ccp(centerX, startY - i * 75.0f));
        pItem->setColor(ccc3(255, 255, 255));

        m_pServerMenu->addChild(pItem);
    }
}

void CServerSelector::onBtnSelectServer(CCObject* pSender) {
    CCNode* pNode = dynamic_cast<CCNode*>(pSender);
    if (!pNode) return;

    int selectedId = pNode->getTag();
    for (size_t i = 0; i < m_serverList.size(); ++i) {
        if (m_serverList[i].id == selectedId) {
            std::stringstream ss;
            ss << "http://" << m_serverList[i].ip << ":" << m_serverList[i].port;
            CCLog("[CServerSelector] Da chon Server: %s (%s)", m_serverList[i].name.c_str(), ss.str().c_str());

            if (m_pDelegate) {
                m_pDelegate->onServerSelected(m_serverList[i].id, m_serverList[i].name, ss.str());
            }
            break;
        }
    }

    this->removeFromParentAndCleanup(true);
}

void CServerSelector::onBtnClose(CCObject* pSender) {
    this->removeFromParentAndCleanup(true);
}
