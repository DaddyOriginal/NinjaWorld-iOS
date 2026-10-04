#include "CPlayerArenaView.h"
#include "CMainMenu.h"
#include "CRoundResultView.h"
#include "support/tinyxml2/tinyxml2.h"
#include <sstream>

using namespace tinyxml2;

CPlayerArenaView::CPlayerArenaView()
    : m_myRank(1)
    , m_myRenown(0)
    , m_remainingFights(15)
    , m_maxFights(15)
    , m_cdRemaining(0)
    , m_pNodeTableContent(NULL)
    , m_pNodeCardContent(NULL)
    , m_pBtnRank(NULL)
    , m_pBtnChest(NULL)
    , m_pBtnMore(NULL)
    , m_pLabelRank(NULL)
    , m_pLabelRemainTimes(NULL)
    , m_pLabelRestTime(NULL)
    , m_pLabelAwardType(NULL)
{
}

CPlayerArenaView::~CPlayerArenaView() {
    CC_SAFE_RELEASE_NULL(m_pNodeTableContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCardContent);
    CC_SAFE_RELEASE_NULL(m_pBtnRank);
    CC_SAFE_RELEASE_NULL(m_pBtnChest);
    CC_SAFE_RELEASE_NULL(m_pBtnMore);
    CC_SAFE_RELEASE_NULL(m_pLabelRank);
    CC_SAFE_RELEASE_NULL(m_pLabelRemainTimes);
    CC_SAFE_RELEASE_NULL(m_pLabelRestTime);
    CC_SAFE_RELEASE_NULL(m_pLabelAwardType);
}

CPlayerArenaView* CPlayerArenaView::create() {
    CPlayerArenaView* pRet = new CPlayerArenaView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CPlayerArenaView::init() {
    if (!CCLayer::init()) return false;

    // Nạp giao diện ArenaView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("ArenaView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CPlayerArenaView] Canh bao: Khong the tai ArenaView.ccbi");
    }

    return true;
}

void CPlayerArenaView::onEnter() {
    CCLayer::onEnter();
    requestArenaInfo();
}

void CPlayerArenaView::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO ArenaView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CPlayerArenaView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CPlayerArenaView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickRankList", CPlayerArenaView::onClickRankList);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickChest", CPlayerArenaView::onClickChest);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickAwardRule", CPlayerArenaView::onClickAwardRule);
    return NULL;
}

bool CPlayerArenaView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_tablecontent", CCNode*, this->m_pNodeTableContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cardcontent", CCNode*, this->m_pNodeCardContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnRank", CCControlButton*, this->m_pBtnRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_chest", CCControlButton*, this->m_pBtnChest);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnMore", CCControlButton*, this->m_pBtnMore);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_rank", CCLabelTTF*, this->m_pLabelRank);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_remain_times", CCLabelTTF*, this->m_pLabelRemainTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_resttime", CCLabelTTF*, this->m_pLabelRestTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_award_type", CCLabelTTF*, this->m_pLabelAwardType);
    return false;
}

void CPlayerArenaView::onClickRankList(CCObject* pSender, CCControlEvent pEvent) {
    showRankListDialog();
}

void CPlayerArenaView::onClickChest(CCObject* pSender, CCControlEvent pEvent) {
    showRewardDialog();
}

void CPlayerArenaView::onClickAwardRule(CCObject* pSender, CCControlEvent pEvent) {
    showRuleDialog();
}

void CPlayerArenaView::requestArenaInfo() {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_r_sport");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "8100");

    req->setCallback(this, callfuncND_selector(CPlayerArenaView::onArenaInfoResp));
    req->send();
    CCLog("[CPlayerArenaView] Gui yeu cau thong tin Loi Dai /rl_r_sport (CMD 8100)");
}

void CPlayerArenaView::onArenaInfoResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CPlayerArenaView] Loi goi /rl_r_sport: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    std::string xmlStr = pRequest->getResponseString();
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    m_opponents.clear();

    XMLElement* sportElem = root->FirstChildElement("sport");
    if (sportElem) {
        const char* rankStr = sportElem->Attribute("rank");
        if (rankStr) m_myRank = atoi(rankStr);

        const char* renownStr = sportElem->Attribute("renown");
        if (renownStr) m_myRenown = atoi(renownStr);

        const char* fightCountStr = sportElem->Attribute("fight_count");
        const char* maxFightStr = sportElem->Attribute("max_fight");
        if (fightCountStr) m_remainingFights = atoi(fightCountStr);
        if (maxFightStr) m_maxFights = atoi(maxFightStr);

        const char* cdStr = sportElem->Attribute("cd");
        if (cdStr) m_cdRemaining = atoi(cdStr);

        // Đọc danh sách đối thủ
        XMLElement* itemNode = sportElem->FirstChildElement("item");
        while (itemNode) {
            ArenaOpponentData opp;
            memset(&opp, 0, sizeof(opp));

            const char* pId = itemNode->Attribute("player_id");
            if (pId) opp.playerId = atoi(pId);

            const char* nName = itemNode->Attribute("nickname");
            if (nName) opp.nickName = nName;

            const char* rk = itemNode->Attribute("rank");
            if (rk) opp.rank = atoi(rk);

            const char* lvl = itemNode->Attribute("level");
            if (lvl) opp.level = atoi(lvl);

            const char* ldr = itemNode->Attribute("leader");
            if (ldr) opp.leaderNinjaId = atoi(ldr);

            const char* cn = itemNode->Attribute("coin");
            if (cn) opp.rewardCoin = atoi(cn);

            const char* rn = itemNode->Attribute("renown");
            if (rn) opp.rewardRenown = atoi(rn);

            const char* hst = itemNode->Attribute("host");
            if (hst) opp.isMe = (atoi(hst) == 1);

            m_opponents.push_back(opp);
            itemNode = itemNode->NextSiblingElement("item");
        }
    }

    // Cập nhật thông số hiển thị
    if (m_pLabelRank) {
        std::stringstream ss;
        ss << "Hạng hiện tại: " << m_myRank;
        m_pLabelRank->setString(ss.str().c_str());
    }

    if (m_pLabelRemainTimes) {
        std::stringstream ss;
        ss << "Lượt khiêu chiến: " << m_remainingFights << "/" << m_maxFights;
        m_pLabelRemainTimes->setString(ss.str().c_str());
    }

    if (m_pLabelRestTime) {
        if (m_cdRemaining <= 0) {
            m_pLabelRestTime->setString("Sẵn sàng chiến đấu!");
            m_pLabelRestTime->setColor(ccc3(52, 211, 153));
        } else {
            std::stringstream ss;
            ss << "Thời gian chờ: " << m_cdRemaining << "s";
            m_pLabelRestTime->setString(ss.str().c_str());
            m_pLabelRestTime->setColor(ccc3(239, 68, 68));
        }
    }

    if (m_pLabelAwardType) {
        std::stringstream ss;
        ss << "Huân chương: " << m_myRenown;
        m_pLabelAwardType->setString(ss.str().c_str());
    }

    buildOpponentList();
}

void CPlayerArenaView::buildOpponentList() {
    if (!m_pNodeTableContent) return;
    m_pNodeTableContent->removeAllChildrenWithCleanup(true);

    float startY = 220.0f;
    float gapY = 85.0f;

    for (size_t i = 0; i < m_opponents.size(); ++i) {
        float posY = startY - i * gapY;

        // Nạp ArenaCellView.ccbi cho mỗi đối thủ
        CCNode* pCell = CCBManager::sharedManager()->loadNodeFromCCBI("ArenaCellView.ccbi", NULL, NULL);
        if (pCell) {
            setupOpponentCell(pCell, m_opponents[i], posY);
            m_pNodeTableContent->addChild(pCell);
        }
    }
}

void CPlayerArenaView::setupOpponentCell(CCNode* cellNode, const ArenaOpponentData& opp, float posY) {
    cellNode->setPosition(ccp(40.0f, posY));

    // Tìm và cập nhật nhãn trong cell
    CCArray* allChildren = cellNode->getChildren();
    if (allChildren) {
        for (unsigned int i = 0; i < allChildren->count(); ++i) {
            CCNode* child = (CCNode*)allChildren->objectAtIndex(i);
            CCLabelTTF* pLbl = dynamic_cast<CCLabelTTF*>(child);
            if (pLbl) {
                // Tùy chỉnh tên nếu có
            }
        }
    }

    // Hiển thị thông tin đối thủ
    std::stringstream ssRank, ssName, ssReward;
    ssRank << "#" << opp.rank;
    CCLabelTTF* pLblRank = CCLabelTTF::create(ssRank.str().c_str(), "Helvetica-Bold", 18.0f);
    pLblRank->setPosition(ccp(35.0f, 40.0f));
    pLblRank->setColor(opp.rank <= 3 ? ccc3(255, 215, 0) : ccc3(200, 200, 200));
    cellNode->addChild(pLblRank, 10);

    ssName << opp.nickName << " (Lv." << opp.level << ")";
    CCLabelTTF* pLblName = CCLabelTTF::create(ssName.str().c_str(), "Helvetica-Bold", 16.0f);
    pLblName->setPosition(ccp(180.0f, 50.0f));
    pLblName->setColor(opp.isMe ? ccc3(52, 211, 153) : ccc3(255, 235, 120));
    cellNode->addChild(pLblName, 10);

    ssReward << "Thưởng: +" << opp.rewardCoin << " Bạc, +" << opp.rewardRenown << " Huân chương";
    CCLabelTTF* pLblReward = CCLabelTTF::create(ssReward.str().c_str(), "Helvetica", 13.0f);
    pLblReward->setPosition(ccp(200.0f, 25.0f));
    pLblReward->setColor(ccc3(220, 220, 220));
    cellNode->addChild(pLblReward, 10);

    // Nút Khiêu Chiến
    if (!opp.isMe) {
        CCMenuItemFont* pFightBtn = CCMenuItemFont::create("Khiêu Chiến", this, menu_selector(CPlayerArenaView::onFightClicked));
        pFightBtn->setFontName("Helvetica-Bold");
        pFightBtn->setFontSize(16);
        pFightBtn->setColor(ccc3(239, 68, 68)); // Đỏ nổi bật
        pFightBtn->setTag(opp.rank);
        pFightBtn->setPosition(ccp(450.0f, 40.0f));

        CCMenu* pMenu = CCMenu::create(pFightBtn, NULL);
        pMenu->setPosition(CCPointZero);
        cellNode->addChild(pMenu, 15);
    } else {
        CCLabelTTF* pMeLbl = CCLabelTTF::create("[Bản thân]", "Helvetica-Bold", 16.0f);
        pMeLbl->setPosition(ccp(450.0f, 40.0f));
        pMeLbl->setColor(ccc3(52, 211, 153));
        cellNode->addChild(pMeLbl, 15);
    }
}

void CPlayerArenaView::onFightClicked(CCObject* pSender) {
    CCMenuItem* pItem = dynamic_cast<CCMenuItem*>(pSender);
    if (!pItem) return;

    int targetRank = pItem->getTag();
    if (m_remainingFights <= 0) {
        CCLog("[CPlayerArenaView] Het luot khieu chien trong ngay!");
        return;
    }

    sendChallengeRequest(targetRank);
}

void CPlayerArenaView::sendChallengeRequest(int targetRank) {
    CRLRequest* req = CRLRequest::create();
    if (!req) return;

    req->setTargetUrl("/rl_w_sport");
    req->setMethod(CRLRequest::METHOD_POST);
    req->setParam("cmd", "8200");
    req->setParam("ToRank", CCString::createWithFormat("%d", targetRank)->getCString());

    req->setCallback(this, callfuncND_selector(CPlayerArenaView::onChallengeResp));
    req->send();
    CCLog("[CPlayerArenaView] Gui lenh khieu chien Loi Dai /rl_w_sport (CMD 8200, ToRank=%d)", targetRank);
}

void CPlayerArenaView::onChallengeResp(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) {
        CCLog("[CPlayerArenaView] Loi goi khieu chien: %s", pRequest ? pRequest->getErrorMessage().c_str() : "NULL");
        return;
    }

    std::string xmlStr = pRequest->getResponseString();
    if (xmlStr.empty()) return;

    XMLDocument doc;
    if (doc.Parse(xmlStr.c_str()) != XML_SUCCESS) return;

    XMLElement* root = doc.RootElement();
    if (!root) return;

    bool isWin = false;
    int newRank = m_myRank;
    int coinGained = 0;
    int renownGained = 0;

    XMLElement* winElem = root->FirstChildElement("win");
    if (winElem && winElem->GetText()) {
        isWin = (atoi(winElem->GetText()) == 1);
    }

    XMLElement* rankElem = root->FirstChildElement("new_rank");
    if (rankElem && rankElem->GetText()) {
        newRank = atoi(rankElem->GetText());
    }

    XMLElement* coinElem = root->FirstChildElement("coin");
    if (coinElem && coinElem->GetText()) {
        coinGained = atoi(coinElem->GetText());
    }

    XMLElement* renownElem = root->FirstChildElement("renown");
    if (renownElem && renownElem->GetText()) {
        renownGained = atoi(renownElem->GetText());
    }

    CCLog("[CPlayerArenaView] Ket qua tran dau Loi Dai: Thang=%d, HangMoi=%d, Bac+=%d, HuanChuong+=%d",
          isWin ? 1 : 0, newRank, coinGained, renownGained);

    // Cập nhật ví tiền và dữ liệu người chơi
    CPlayerDataMgr* pPlayer = CPlayerDataMgr::sharedManager();
    if (pPlayer) {
        pPlayer->firefly_SetSilver(pPlayer->firefly_GetSilver() + coinGained);
    }

    // Hiển thị Popup kết quả trận đấu
    std::stringstream ssResult;
    if (isWin) {
        ssResult << "CHIẾN THẮNG!\nThăng hạng: #" << newRank << "\n+" << coinGained << " Bạc, +" << renownGained << " Huân chương";
    } else {
        ssResult << "THẤT BẠI!\nGiữ nguyên thứ hạng: #" << m_myRank << "\n+" << coinGained << " Bạc";
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));
    
    CCLabelTTF* pMsg = CCLabelTTF::create(ssResult.str().c_str(), "Helvetica-Bold", 22.0f, CCSizeMake(380.0f, 150.0f), kCCTextAlignmentCenter);
    pMsg->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f + 30.0f));
    pMsg->setColor(isWin ? ccc3(255, 215, 0) : ccc3(239, 68, 68));
    pMask->addChild(pMsg);

    CCMenuItemFont* pClose = CCMenuItemFont::create("Xác Nhận", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(20);
    pClose->setColor(ccc3(255, 235, 120));
    pClose->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f - 80.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);

    // Làm mới lại thông tin
    requestArenaInfo();
}

void CPlayerArenaView::showRuleDialog() {
    CCNode* pDialog = CCBManager::sharedManager()->loadNodeFromCCBI("ArenaRuleDialogView.ccbi", NULL, NULL);
    if (!pDialog) return;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));
    pDialog->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    pMask->addChild(pDialog);

    CCMenuItemFont* pClose = CCMenuItemFont::create("Đóng", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(18);
    pClose->setColor(ccc3(255, 215, 0));
    pClose->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f - 180.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu, 20);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);
}

void CPlayerArenaView::showRewardDialog() {
    CCNode* pDialog = CCBManager::sharedManager()->loadNodeFromCCBI("ArenaDialogReward.ccbi", NULL, NULL);
    if (!pDialog) return;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));
    pDialog->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    pMask->addChild(pDialog);

    CCMenuItemFont* pClose = CCMenuItemFont::create("Đóng", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(18);
    pClose->setColor(ccc3(255, 215, 0));
    pClose->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f - 180.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu, 20);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);
}

void CPlayerArenaView::showRankListDialog() {
    CCNode* pDialog = CCBManager::sharedManager()->loadNodeFromCCBI("ArenaRankDialogView.ccbi", NULL, NULL);
    if (!pDialog) return;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* pMask = CCLayerColor::create(ccc4(0, 0, 0, 180));
    pDialog->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    pMask->addChild(pDialog);

    CCMenuItemFont* pClose = CCMenuItemFont::create("Đóng", pMask, menu_selector(CCNode::removeFromParent));
    pClose->setFontName("Helvetica-Bold");
    pClose->setFontSize(18);
    pClose->setColor(ccc3(255, 215, 0));
    pClose->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f - 200.0f));

    CCMenu* pMenu = CCMenu::create(pClose, NULL);
    pMenu->setPosition(CCPointZero);
    pMask->addChild(pMenu, 20);

    CCDirector::sharedDirector()->getRunningScene()->addChild(pMask, 1000);
}

void CPlayerArenaView::refreshView() {
    requestArenaInfo();
}
