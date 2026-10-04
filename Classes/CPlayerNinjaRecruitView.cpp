#include "CPlayerNinjaRecruitView.h"
#include "CMainMenu.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CPlayerNinjaRecruitView::CPlayerNinjaRecruitView()
    : m_pBtnPray(NULL)
    , m_pBtnConsum(NULL)
    , m_pBtnGiftPack(NULL)
    , m_pBtnVipInfo(NULL)
    , m_pNodeTableContent(NULL)
    , m_pNodeTouchContent(NULL)
    , m_pNodeCardContent(NULL)
    , m_selectedTier(1)
{
    // Cấu hình chuẩn 3 cấp chiêu mộ theo server & client table
    RecruitTierData t1;
    t1.recruitId = 1;
    t1.name = "Triệu Hồi Hạ Đẳng";
    t1.costSingle = 300;
    t1.costTen = 2700;
    t1.cooldownSeconds = 86400; // 24h
    t1.timeLeft = 0;
    t1.protectComing = 10;
    t1.isFree = true;
    m_tiers.push_back(t1);

    RecruitTierData t2;
    t2.recruitId = 2;
    t2.name = "Triệu Hồi Trung Đẳng";
    t2.costSingle = 150;
    t2.costTen = 1350;
    t2.cooldownSeconds = 172800; // 48h
    t2.timeLeft = 1800;
    t2.protectComing = 10;
    t2.isFree = false;
    m_tiers.push_back(t2);

    RecruitTierData t3;
    t3.recruitId = 3;
    t3.name = "Thần Khí Triệu Hồi";
    t3.costSingle = 280;
    t3.costTen = 2520;
    t3.cooldownSeconds = 432000; // 5 ngày
    t3.timeLeft = 36000;
    t3.protectComing = 10;
    t3.isFree = false;
    m_tiers.push_back(t3);
}

CPlayerNinjaRecruitView::~CPlayerNinjaRecruitView() {
    CC_SAFE_RELEASE_NULL(m_pBtnPray);
    CC_SAFE_RELEASE_NULL(m_pBtnConsum);
    CC_SAFE_RELEASE_NULL(m_pBtnGiftPack);
    CC_SAFE_RELEASE_NULL(m_pBtnVipInfo);
    CC_SAFE_RELEASE_NULL(m_pNodeTableContent);
    CC_SAFE_RELEASE_NULL(m_pNodeTouchContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCardContent);
}

CPlayerNinjaRecruitView* CPlayerNinjaRecruitView::create() {
    CPlayerNinjaRecruitView* pRet = new CPlayerNinjaRecruitView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CPlayerNinjaRecruitView::init() {
    if (!CCLayer::init()) return false;

    // Nạp giao diện StoreItemsView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("StoreItemsView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CPlayerNinjaRecruitView] Canh bao: Khong the tai StoreItemsView.ccbi");
    }

    buildRecruitBanners();
    return true;
}

void CPlayerNinjaRecruitView::onEnter() {
    CCLayer::onEnter();
    requestRecruitInfo();
}

void CPlayerNinjaRecruitView::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO StoreItemsView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CPlayerNinjaRecruitView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CPlayerNinjaRecruitView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnPray", CPlayerNinjaRecruitView::onBtnPrayClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnConsum", CPlayerNinjaRecruitView::onBtnConsumClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnGiftPack", CPlayerNinjaRecruitView::onBtnGiftPackClicked);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnVipInfo", CPlayerNinjaRecruitView::onBtnVipInfoClicked);
    return NULL;
}

bool CPlayerNinjaRecruitView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnPray", CCControlButton*, this->m_pBtnPray);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnConsum", CCControlButton*, this->m_pBtnConsum);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnGiftPack", CCControlButton*, this->m_pBtnGiftPack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnVipInfo", CCControlButton*, this->m_pBtnVipInfo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_tablecontent", CCNode*, this->m_pNodeTableContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_touchcontent", CCNode*, this->m_pNodeTouchContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cardcontent", CCNode*, this->m_pNodeCardContent);
    return false;
}

void CPlayerNinjaRecruitView::onBtnPrayClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CPlayerNinjaRecruitView] Tab Chieu Mo / Cau Nguyen");
    buildRecruitBanners();
}

void CPlayerNinjaRecruitView::onBtnConsumClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CPlayerNinjaRecruitView] Tab Vat Pham Tieu Hao");
}

void CPlayerNinjaRecruitView::onBtnGiftPackClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CPlayerNinjaRecruitView] Tab Goi Qua");
}

void CPlayerNinjaRecruitView::onBtnVipInfoClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CPlayerNinjaRecruitView] Tab Thong Tin VIP");
}

void CPlayerNinjaRecruitView::requestRecruitInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_shoplist");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "2800");

    req->setCallback(this, callfuncND_selector(CPlayerNinjaRecruitView::onRecruitInfoResp));
    req->send();
    CCLog("[CPlayerNinjaRecruitView] Gui yeu cau thong tin chiêu mộ /rl_r_shoplist (CMD 2800)");
}

void CPlayerNinjaRecruitView::onRecruitInfoResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CPlayerNinjaRecruitView] Loi goi /rl_r_shoplist: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    std::string xmlStr = pRequest->getResponseString();
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    XMLElement* shopList = root->FirstChildElement("shoplist");
    if (shopList) {
        XMLElement* recruitElem = shopList->FirstChildElement("recruit");
        while (recruitElem) {
            XMLElement* idElem = recruitElem->FirstChildElement("recruit_id");
            XMLElement* timeElem = recruitElem->FirstChildElement("free_recruit_time_left");
            XMLElement* costElem = recruitElem->FirstChildElement("recruit_yuanbao");

            if (idElem && idElem->GetText()) {
                int rId = atoi(idElem->GetText());
                for (size_t i = 0; i < m_tiers.size(); ++i) {
                    if (m_tiers[i].recruitId == rId) {
                        if (timeElem && timeElem->GetText()) {
                            m_tiers[i].timeLeft = atoi(timeElem->GetText());
                            m_tiers[i].isFree = (m_tiers[i].timeLeft <= 0);
                        }
                        if (costElem && costElem->GetText()) {
                            m_tiers[i].costSingle = atoi(costElem->GetText());
                        }
                    }
                }
            }
            recruitElem = recruitElem->NextSiblingElement("recruit");
        }
    }

    buildRecruitBanners();
}

void CPlayerNinjaRecruitView::buildRecruitBanners() {
    if (!m_pNodeTableContent) return;
    m_pNodeTableContent->removeAllChildrenWithCleanup(true);

    float startX = 60.0f;
    float gapX = 180.0f;

    for (size_t i = 0; i < m_tiers.size(); ++i) {
        float posX = startX + i * gapX;

        // Nạp StoreNinjaView.ccbi cho mỗi cấp chiêu mộ
        CCNode* pBanner = CCBManager::sharedManager()->loadNodeFromCCBI("StoreNinjaView.ccbi", NULL, NULL);
        if (pBanner) {
            setupBannerCell(pBanner, m_tiers[i], posX);
            m_pNodeTableContent->addChild(pBanner);
        }
    }
}

void CPlayerNinjaRecruitView::setupBannerCell(CCNode* cellNode, const RecruitTierData& tier, float posX) {
    cellNode->setPosition(ccp(posX, 140.0f));

    // Nút Chiêu Mộ 1 Lần
    std::string singleText = tier.isFree ? "Miễn Phí" : "Rút 1 Lần";
    CCMenuItemFont* pBtnSingle = CCMenuItemFont::create(singleText.c_str(), this, menu_selector(CPlayerNinjaRecruitView::onRecruitSingleClicked));
    pBtnSingle->setFontName("Helvetica-Bold");
    pBtnSingle->setFontSize(16);
    pBtnSingle->setColor(tier.isFree ? ccc3(52, 211, 153) : ccc3(255, 235, 120));
    pBtnSingle->setTag(tier.recruitId);
    pBtnSingle->setPosition(ccp(60.0f, -40.0f));

    // Nút Chiêu Mộ 10 Lần
    std::stringstream ssTen;
    ssTen << "Rút 10 Lần (" << tier.costTen << " Vàng)";
    CCMenuItemFont* pBtnTen = CCMenuItemFont::create(ssTen.str().c_str(), this, menu_selector(CPlayerNinjaRecruitView::onRecruitTenClicked));
    pBtnTen->setFontName("Helvetica-Bold");
    pBtnTen->setFontSize(14);
    pBtnTen->setColor(ccc3(255, 180, 50));
    pBtnTen->setTag(tier.recruitId);
    pBtnTen->setPosition(ccp(60.0f, -75.0f));

    CCMenu* pMenu = CCMenu::create(pBtnSingle, pBtnTen, NULL);
    pMenu->setPosition(CCPointZero);
    cellNode->addChild(pMenu, 20);

    // Tiêu đề banner & giá vàng
    CCLabelTTF* pLblTitle = CCLabelTTF::create(tier.name.c_str(), "Helvetica-Bold", 16.0f);
    pLblTitle->setPosition(ccp(60.0f, 150.0f));
    pLblTitle->setColor(ccc3(255, 215, 0));
    cellNode->addChild(pLblTitle);

    std::stringstream ssCost;
    if (tier.isFree) {
        ssCost << "Miễn phí lượt này!";
    } else {
        ssCost << "Giá: " << tier.costSingle << " Vàng";
    }
    CCLabelTTF* pLblCost = CCLabelTTF::create(ssCost.str().c_str(), "Helvetica", 14.0f);
    pLblCost->setPosition(ccp(60.0f, -15.0f));
    pLblCost->setColor(ccc3(240, 240, 240));
    cellNode->addChild(pLblCost);
}

void CPlayerNinjaRecruitView::onRecruitSingleClicked(CCObject* pSender) {
    CCMenuItem* pItem = dynamic_cast<CCMenuItem*>(pSender);
    if (!pItem) return;

    int recruitId = pItem->getTag();
    bool isFree = false;
    for (size_t i = 0; i < m_tiers.size(); ++i) {
        if (m_tiers[i].recruitId == recruitId) {
            isFree = m_tiers[i].isFree;
            break;
        }
    }

    sendRecruitRequest(recruitId, false, isFree);
}

void CPlayerNinjaRecruitView::onRecruitTenClicked(CCObject* pSender) {
    CCMenuItem* pItem = dynamic_cast<CCMenuItem*>(pSender);
    if (!pItem) return;

    int recruitId = pItem->getTag();
    sendRecruitRequest(recruitId, true, false);
}

void CPlayerNinjaRecruitView::sendRecruitRequest(int recruitId, bool isTen, bool isFree) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_recruit");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", isTen ? "1901" : "1900");
    req->setParam("Type", CCString::createWithFormat("%d", recruitId)->getCString());
    if (isFree) {
        req->setParam("IsFree", "1");
    }

    req->setCallback(this, callfuncND_selector(CPlayerNinjaRecruitView::onRecruitResultResp));
    req->send();
    CCLog("[CPlayerNinjaRecruitView] Gui lenh chieu mo /rl_w_recruit: RecruitID=%d, IsTen=%d, IsFree=%d",
          recruitId, isTen ? 1 : 0, isFree ? 1 : 0);
}

void CPlayerNinjaRecruitView::onRecruitResultResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CPlayerNinjaRecruitView] Loi goi chieu mo: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    std::string xmlStr = pRequest->getResponseString();
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    // 1. Kiểm tra Chiêu Mộ 10 Lần (<recruit_list>)
    XMLElement* listElem = root->FirstChildElement("recruit_list");
    if (listElem) {
        std::vector<int> tenNinjas;
        XMLElement* itemNode = listElem->FirstChildElement("item");
        while (itemNode) {
            const char* nIdStr = itemNode->Attribute("ninjaid");
            if (nIdStr) {
                int ninjaId = atoi(nIdStr);
                tenNinjas.push_back(ninjaId);

                // Thêm vào kho thẻ người chơi
                CPlayerNinja* pNewNinja = CPlayerNinja::create(0, ninjaId, 1, 0);
                if (pNewNinja) {
                    CPlayerDataMgr::sharedManager()->addPlayerNinja(pNewNinja);
                }
            }
            itemNode = itemNode->NextSiblingElement("item");
        }

        if (!tenNinjas.empty()) {
            CCLog("[CPlayerNinjaRecruitView] Chieu mo thanh cong 10 ninja!");
            showTenRecruitPopup(tenNinjas);
        }
        return;
    }

    // 2. Chiêu Mộ Đơn (<recruit>)
    XMLElement* recruitNode = root->FirstChildElement("recruit");
    if (recruitNode) {
        const char* nIdStr = recruitNode->Attribute("ninjaid");
        int ninjaId = nIdStr ? atoi(nIdStr) : 1;

        const NinjaTableEntry* pEntry = CNinjaTableMgr::sharedManager()->getNinjaEntry(ninjaId);
        int star = pEntry ? pEntry->star : 3;

        CPlayerNinja* pNewNinja = CPlayerNinja::create(0, ninjaId, 1, 0);
        if (pNewNinja) {
            CPlayerDataMgr::sharedManager()->addPlayerNinja(pNewNinja);
        }

        CCLog("[CPlayerNinjaRecruitView] Chieu mo thanh cong 1 ninja: ID=%d (%s)",
              ninjaId, pEntry ? pEntry->name.c_str() : "Ninja");
        showSingleRecruitPopup(ninjaId, star);
    }

    // Cập nhật Top HUD
    CMainMenu* pMenu = CMainMenu::sharedManager();
    if (pMenu) {
        pMenu->refreshTopHUD();
    }

    // Làm mới lại danh sách
    requestRecruitInfo();
}

void CPlayerNinjaRecruitView::showSingleRecruitPopup(int ninjaId, int star) {
    CCNode* pPopup = CCBManager::sharedManager()->loadNodeFromCCBI("ShowBoxView.ccbi", NULL, NULL);
    if (!pPopup) return;

    const NinjaTableEntry* pEntry = CNinjaTableMgr::sharedManager()->getNinjaEntry(ninjaId);
    std::string ninjaName = pEntry ? pEntry->name : "Ninja";

    // Tìm và cập nhật nhãn trong ShowBoxView
    CCArray* allChildren = pPopup->getChildren();
    if (allChildren) {
        for (unsigned int i = 0; i < allChildren->count(); ++i) {
            CCNode* child = (CCNode*)allChildren->objectAtIndex(i);
            CCLabelTTF* pLbl = dynamic_cast<CCLabelTTF*>(child);
            if (pLbl) {
                pLbl->setString(ninjaName.c_str());
            }
        }
    }

    // Bổ sung lớp hiển thị chi tiết thẻ vừa nhận
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));
    pMask->addChild(pPopup);
    pPopup->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2.0f,
                            CCDirector::sharedDirector()->getWinSize().height / 2.0f));

    // Thêm nút Đóng
    CCMenuItemFont* pClose = CCMenuItemFont::create("Nhận Thẻ", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(20);
    pClose->setColor(ccc3(255, 215, 0));
    pClose->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2.0f, 100.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu, 30);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);
}

void CPlayerNinjaRecruitView::showTenRecruitPopup(const std::vector<int>& ninjaIds) {
    CCNode* pPopup = CCBManager::sharedManager()->loadNodeFromCCBI("TurnTenDialogView.ccbi", NULL, NULL);
    if (!pPopup) return;

    // Cập nhật tên của 10 thẻ trong TurnTenDialogView
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));
    pMask->addChild(pPopup);
    pPopup->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2.0f,
                            CCDirector::sharedDirector()->getWinSize().height / 2.0f));

    // Nút đóng
    CCMenuItemFont* pClose = CCMenuItemFont::create("Nhận Tất Cả", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(22);
    pClose->setColor(ccc3(255, 215, 0));
    pClose->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2.0f, 60.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu, 30);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);
}

void CPlayerNinjaRecruitView::refreshView() {
    buildRecruitBanners();
}
