#include "CSubChapterView.h"
#include "CChapterView.h"
#include "CRoundTeamFightView.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CSubChapterView::CSubChapterView()
    : m_chapterId(1)
    , m_selectedStageId(1)
    , m_selectedRoundIndex(0)
    , m_pBtnBack(NULL)
    , m_pNodeBtnListContainer(NULL)
    , m_pNodeSectionContainer(NULL)
    , m_pNodeCardContent(NULL)
    , m_pSubChapterDetailNode(NULL)
    , m_pLabelTitle(NULL)
    , m_pLabelSectionNo(NULL)
    , m_pLabelNeedEnergy(NULL)
    , m_pLabelExp(NULL)
    , m_pLabelSilver(NULL)
    , m_pLabelDesc(NULL)
    , m_pSectionNormalContainer(NULL)
    , m_pSectionBossContainer(NULL)
    , m_pNodeAwardContainer(NULL)
{
}

CSubChapterView::~CSubChapterView() {
    CC_SAFE_RELEASE_NULL(m_pBtnBack);
    CC_SAFE_RELEASE_NULL(m_pNodeBtnListContainer);
    CC_SAFE_RELEASE_NULL(m_pNodeSectionContainer);
    CC_SAFE_RELEASE_NULL(m_pNodeCardContent);
}

CSubChapterView* CSubChapterView::createWithChapter(int chapterId) {
    CSubChapterView* pRet = new CSubChapterView();
    if (pRet && pRet->initWithChapter(chapterId)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CSubChapterView::initWithChapter(int chapterId) {
    if (!CCLayer::init()) return false;

    m_chapterId = chapterId;

    // Nạp giao diện khung SubChapterListView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("SubChapterListView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CSubChapterView] Canh bao: Khong the tai SubChapterListView.ccbi");
    }

    // Lấy danh sách các Ải / Tab trong chương này
    m_stages = CStageTableMgr::sharedManager()->getDistinctStagesForStory(m_chapterId);
    if (m_stages.empty()) {
        m_stages.push_back(1);
    }

    int currentSec = CChapterMgr::sharedManager()->getCurrentSection();
    m_selectedStageId = currentSec > 0 ? currentSec : m_stages[0];

    buildStageTabs();
    loadStageDetail(m_selectedStageId);

    return true;
}

void CSubChapterView::onEnter() {
    CCLayer::onEnter();
}

void CSubChapterView::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO SubChapterListView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CSubChapterView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CSubChapterView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickBack", CSubChapterView::onClickBack);
    return NULL;
}

bool CSubChapterView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_back", CCControlButton*, this->m_pBtnBack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_btnlist_container", CCNode*, this->m_pNodeBtnListContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_section_container", CCNode*, this->m_pNodeSectionContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cardcontent", CCNode*, this->m_pNodeCardContent);
    return false;
}

void CSubChapterView::onClickBack(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CSubChapterView] Quay lai man hinh Chon Chuong");
    CCNode* pParent = this->getParent();
    if (pParent) {
        CChapterView* pChapView = CChapterView::create();
        if (pChapView) {
            pParent->addChild(pChapView, this->getZOrder());
        }
        this->removeFromParentAndCleanup(true);
    }
}

void CSubChapterView::buildStageTabs() {
    if (!m_pNodeBtnListContainer) return;
    m_pNodeBtnListContainer->removeAllChildrenWithCleanup(true);

    CCMenu* pMenu = CCMenu::create();
    pMenu->setPosition(CCPointZero);

    for (size_t i = 0; i < m_stages.size(); ++i) {
        int stgId = m_stages[i];
        std::string stgName = CStageTableMgr::sharedManager()->getStageName(m_chapterId, stgId);
        if (stgName.empty()) {
            std::stringstream ss;
            ss << "Ải " << stgId;
            stgName = ss.str();
        }

        bool isUnlocked = CChapterMgr::sharedManager()->isSectionUnlocked(m_chapterId, stgId);
        bool isSelected = (stgId == m_selectedStageId);

        CCMenuItemFont* pItem = CCMenuItemFont::create(stgName.c_str(), this, menu_selector(CSubChapterView::onSelectStageTab));
        pItem->setFontName("Helvetica-Bold");
        pItem->setFontSize(16);
        pItem->setTag(stgId);
        pItem->setPosition(ccp(70.0f + i * 110.0f, 30.0f));

        if (isSelected) {
            pItem->setColor(ccc3(255, 215, 0)); // Vàng sáng
        } else if (isUnlocked) {
            pItem->setColor(ccc3(230, 230, 230)); // Trắng
        } else {
            pItem->setColor(ccc3(120, 120, 120)); // Xám khóa
        }

        pMenu->addChild(pItem);
    }

    m_pNodeBtnListContainer->addChild(pMenu);
}

void CSubChapterView::onSelectStageTab(CCObject* pSender) {
    CCMenuItem* pItem = dynamic_cast<CCMenuItem*>(pSender);
    if (!pItem) return;

    int stgId = pItem->getTag();
    if (!CChapterMgr::sharedManager()->isSectionUnlocked(m_chapterId, stgId)) {
        CCLog("[CSubChapterView] Ai %d chua duoc mo!", stgId);
        return;
    }

    m_selectedStageId = stgId;
    m_selectedRoundIndex = 0;
    buildStageTabs();
    loadStageDetail(m_selectedStageId);
}

void CSubChapterView::loadStageDetail(int stageId) {
    if (!m_pNodeSectionContainer) return;
    m_pNodeSectionContainer->removeAllChildrenWithCleanup(true);

    m_roundsInStage = CStageTableMgr::sharedManager()->getRoundsForStoryAndStage(m_chapterId, stageId);
    if (m_roundsInStage.empty()) {
        CCLog("[CSubChapterView] Khong tim thay thong tin round cho chuong %d, ai %d", m_chapterId, stageId);
        return;
    }

    // Nạp SubChapter.ccbi
    m_pSubChapterDetailNode = CCBManager::sharedManager()->loadNodeFromCCBI("SubChapter.ccbi", NULL, NULL);
    if (!m_pSubChapterDetailNode) {
        CCLog("[CSubChapterView] Canh bao: Khong the tai SubChapter.ccbi");
        return;
    }

    m_pNodeSectionContainer->addChild(m_pSubChapterDetailNode);

    // Lấy round đầu tiên trong stage
    const RoundTableEntry& round = m_roundsInStage[0];
    updateDetailUI(round);
}

void CSubChapterView::updateDetailUI(const RoundTableEntry& round) {
    if (!m_pSubChapterDetailNode) return;

    // Tìm và cập nhật các nhãn văn bản trong SubChapter.ccbi
    CCArray* allChildren = m_pSubChapterDetailNode->getChildren();
    if (allChildren) {
        for (unsigned int i = 0; i < allChildren->count(); ++i) {
            CCNode* child = (CCNode*)allChildren->objectAtIndex(i);

            // Kiểm tra CCLabelTTF
            CCLabelTTF* pTTF = dynamic_cast<CCLabelTTF*>(child);
            if (pTTF) {
                // Tiêu đề
                if (std::string(pTTF->getString()).find("SubChapter") != std::string::npos ||
                    pTTF->getContentSize().width > 200.0f) {
                    pTTF->setString(round.name.c_str());
                }
            }
        }
    }

    // Bổ sung các thông số chi tiết của Ải
    CCNode* pInfoLayer = CCNode::create();
    pInfoLayer->setPosition(ccp(50.0f, 100.0f));

    // Tiêu đề Ải & Tên Cốt truyện
    const StoryTableEntry* pStory = CStageTableMgr::sharedManager()->getStoryEntry(m_chapterId);
    std::string fullTitle = (pStory ? pStory->name : "Cốt truyện") + " - " + round.name;
    CCLabelTTF* pLblTitle = CCLabelTTF::create(fullTitle.c_str(), "Helvetica-Bold", 20.0f);
    pLblTitle->setPosition(ccp(220.0f, 180.0f));
    pLblTitle->setColor(ccc3(255, 235, 120));
    pInfoLayer->addChild(pLblTitle);

    // Thể lực cần
    std::stringstream ssEnergy;
    ssEnergy << "Thể lực tiêu hao: " << round.needPower;
    CCLabelTTF* pLblEnergy = CCLabelTTF::create(ssEnergy.str().c_str(), "Helvetica", 16.0f);
    pLblEnergy->setPosition(ccp(150.0f, 130.0f));
    pLblEnergy->setColor(ccc3(52, 211, 153)); // Xanh lục
    pInfoLayer->addChild(pLblEnergy);

    // Thưởng EXP & Bạc
    std::stringstream ssReward;
    ssReward << "Phần thưởng: " << round.addExp << " EXP, " << round.addSilver << " Bạc";
    CCLabelTTF* pLblReward = CCLabelTTF::create(ssReward.str().c_str(), "Helvetica", 16.0f);
    pLblReward->setPosition(ccp(180.0f, 95.0f));
    pLblReward->setColor(ccc3(255, 200, 50));
    pInfoLayer->addChild(pLblReward);

    // Mô tả ải
    CCLabelTTF* pLblDesc = CCLabelTTF::create(round.desc.c_str(), "Helvetica-Oblique", 14.0f, CCSizeMake(420.0f, 60.0f), kCCTextAlignmentLeft);
    pLblDesc->setPosition(ccp(220.0f, 40.0f));
    pLblDesc->setColor(ccc3(210, 210, 210));
    pInfoLayer->addChild(pLblDesc);

    // Nút "Khiêu Chiến / Vượt Ải"
    std::string btnText = (round.isBoss == 1) ? "ĐÁNH BOSS" : "KHIÊU CHIẾN";
    CCMenuItemFont* pFightBtn = CCMenuItemFont::create(btnText.c_str(), this, menu_selector(CSubChapterView::onStartBattleClicked));
    pFightBtn->setFontName("Helvetica-Bold");
    pFightBtn->setFontSize(20);
    pFightBtn->setColor(round.isBoss ? ccc3(239, 68, 68) : ccc3(255, 215, 0));
    pFightBtn->setPosition(ccp(220.0f, -20.0f));

    CCMenu* pFightMenu = CCMenu::create(pFightBtn, NULL);
    pFightMenu->setPosition(CCPointZero);
    pInfoLayer->addChild(pFightMenu);

    m_pSubChapterDetailNode->addChild(pInfoLayer, 20);
}

void CSubChapterView::onStartBattleClicked(CCObject* pSender) {
    if (m_roundsInStage.empty()) return;
    const RoundTableEntry& round = m_roundsInStage[0];

    // Kiểm tra thể lực người chơi
    CPlayerDataMgr* pPlayer = CPlayerDataMgr::sharedManager();
    if (pPlayer && pPlayer->firefly_GetBodyValue() < round.needPower) {
        CCLog("[CSubChapterView] Khong du the luc de vuot ai (Can %d, co %d)!",
              round.needPower, pPlayer->firefly_GetBodyValue());
        return;
    }

    CCLog("[CSubChapterView] Mo man hinh soan doi hinh CRoundTeamFightView: Chap=%d, Stage=%d, Round=%d",
          m_chapterId, m_selectedStageId, round.roundId);

    // Chuyển sang CRoundTeamFightView
    CRoundTeamFightView* pFightView = CRoundTeamFightView::createWithRound(m_chapterId, m_selectedStageId, round.roundId);
    if (pFightView && this->getParent()) {
        CCNode* pParent = this->getParent();
        pParent->addChild(pFightView, this->getZOrder() + 1);
        this->removeFromParentAndCleanup(false);
    }
}

void CSubChapterView::refreshView() {
    buildStageTabs();
    loadStageDetail(m_selectedStageId);
}
