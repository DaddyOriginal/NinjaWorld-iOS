#include "CChapterView.h"
#include "CSubChapterView.h"
#include "CActiveTeamMgr.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CChapterView::CChapterView()
    : m_pNodeSectionContainer(NULL)
    , m_pPageBtn1(NULL)
    , m_pPageBtn2(NULL)
    , m_pPageBtn3(NULL)
    , m_pPageBtn4(NULL)
    , m_pLabelMaxAttack(NULL)
    , m_pLabelMaxDefense(NULL)
    , m_pLabelMaxHonor(NULL)
    , m_currentPage(1)
    , m_pCurrentPageNode(NULL)
{
}

CChapterView::~CChapterView() {
    CC_SAFE_RELEASE_NULL(m_pNodeSectionContainer);
    CC_SAFE_RELEASE_NULL(m_pPageBtn1);
    CC_SAFE_RELEASE_NULL(m_pPageBtn2);
    CC_SAFE_RELEASE_NULL(m_pPageBtn3);
    CC_SAFE_RELEASE_NULL(m_pPageBtn4);
    CC_SAFE_RELEASE_NULL(m_pLabelMaxAttack);
    CC_SAFE_RELEASE_NULL(m_pLabelMaxDefense);
    CC_SAFE_RELEASE_NULL(m_pLabelMaxHonor);
}

CChapterView* CChapterView::create() {
    CChapterView* pRet = new CChapterView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CChapterView::init() {
    if (!CCLayer::init()) return false;

    // Nạp giao diện ChapterView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("ChapterView.ccbi", this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CChapterView] Canh bao: Khong the tai ChapterView.ccbi");
    }

    // Nạp trang chương hiện tại dựa trên tiến độ người chơi
    int currentChap = CChapterMgr::sharedManager()->getCurrentChapter();
    m_currentPage = (currentChap - 1) / 6 + 1;
    if (m_currentPage < 1) m_currentPage = 1;
    if (m_currentPage > 4) m_currentPage = 4;

    updatePageDisplay(m_currentPage);
    updateCombatPowerLabels();

    return true;
}

void CChapterView::onEnter() {
    CCLayer::onEnter();
    // Đồng bộ thông tin tiến độ ải mới nhất từ Server
    CChapterMgr::sharedManager()->requestDungeonInfo();
    updateCombatPowerLabels();
}

void CChapterView::onExit() {
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO ChapterView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CChapterView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CChapterView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickPageBtn1", CChapterView::onClickPageBtn1);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickPageBtn2", CChapterView::onClickPageBtn2);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "clickPageBtn3", CChapterView::onClickPageBtn3);
    return NULL;
}

bool CChapterView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_section_container", CCNode*, this->m_pNodeSectionContainer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page_btn1", CCControlButton*, this->m_pPageBtn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page_btn2", CCControlButton*, this->m_pPageBtn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page_btn3", CCControlButton*, this->m_pPageBtn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page_btn4", CCControlButton*, this->m_pPageBtn4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxattack", CCLabelTTF*, this->m_pLabelMaxAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxdefense", CCLabelTTF*, this->m_pLabelMaxDefense);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_maxhonor", CCLabelTTF*, this->m_pLabelMaxHonor);
    return false;
}

void CChapterView::onClickPageBtn1(CCObject* pSender, CCControlEvent pEvent) {
    updatePageDisplay(1);
}

void CChapterView::onClickPageBtn2(CCObject* pSender, CCControlEvent pEvent) {
    updatePageDisplay(2);
}

void CChapterView::onClickPageBtn3(CCObject* pSender, CCControlEvent pEvent) {
    updatePageDisplay(3);
}

void CChapterView::onClickPageBtn4(CCObject* pSender, CCControlEvent pEvent) {
    updatePageDisplay(4);
}

void CChapterView::updatePageDisplay(int page) {
    m_currentPage = page;

    if (!m_pNodeSectionContainer) return;
    m_pNodeSectionContainer->removeAllChildrenWithCleanup(true);

    // Nạp ChapterCellView.ccbi đại diện cho 1 trang chứa 6 chương
    m_pCurrentPageNode = CCBManager::sharedManager()->loadNodeFromCCBI("ChapterCellView.ccbi", NULL);
    if (!m_pCurrentPageNode) {
        CCLog("[CChapterView] Canh bao: Khong the tai ChapterCellView.ccbi");
        return;
    }

    m_pNodeSectionContainer->addChild(m_pCurrentPageNode);

    // Cài đặt 6 chương trong trang này
    for (int slot = 1; slot <= 6; ++slot) {
        int chapterId = (m_currentPage - 1) * 6 + slot;
        setupChapterContainer(m_pCurrentPageNode, slot, chapterId);
    }

    CCLog("[CChapterView] Da hien thi Trang Chuong %d (Chuong %d -> %d)",
          m_currentPage, (m_currentPage - 1) * 6 + 1, m_currentPage * 6);
}

void CChapterView::setupChapterContainer(CCNode* pageNode, int slotIndex, int chapterId) {
    if (!pageNode) return;

    std::stringstream ssContainer, ssLabel, ssSprite;
    ssContainer << "chapter_container" << slotIndex;
    ssLabel << "label_chapter" << slotIndex;
    ssSprite << "sprite_chapter" << slotIndex;

    CCNode* pContainer = pageNode->getChildByTag(slotIndex);
    // Hoặc duyệt tìm tên node theo hệ thống biến
    const StoryTableEntry* pStory = CStageTableMgr::sharedManager()->getStoryEntry(chapterId);
    bool isUnlocked = CChapterMgr::sharedManager()->isChapterUnlocked(chapterId);

    // Tìm CCLabelTTF theo cấu trúc phân cấp node
    CCArray* allChildren = pageNode->getChildren();
    if (allChildren) {
        for (unsigned int i = 0; i < allChildren->count(); ++i) {
            CCNode* child = (CCNode*)allChildren->objectAtIndex(i);
            // Gán nhãn tên chương nếu tìm thấy
            CCLabelTTF* pLabel = dynamic_cast<CCLabelTTF*>(child);
            if (pLabel && pStory) {
                // Kiểm tra tên nhãn tương ứng slot
            }
        }
    }

    // Tạo nút bấm cảm ứng trong vùng slot của chương
    // ChapterCellView bố trí 6 ô: 2 hàng x 3 cột
    float col = (slotIndex - 1) % 3;
    float row = (slotIndex - 1) / 3;
    CCPoint slotPos = ccp(120.0f + col * 180.0f, 320.0f - row * 200.0f);

    CCMenuItemImage* pItem = CCMenuItemImage::create();
    if (pItem) {
        pItem->setContentSize(CCSizeMake(150.0f, 180.0f));
        pItem->setPosition(slotPos);
        pItem->setTag(chapterId);
        pItem->setTarget(this, menu_selector(CChapterView::onChapterClicked));

        // Nhãn tên chương
        std::string chapName = pStory ? pStory->name : "Chưa mở";
        if (!isUnlocked) {
            chapName += " (Khóa)";
        }
        CCLabelTTF* pChapTitle = CCLabelTTF::create(chapName.c_str(), "Helvetica-Bold", 16.0f);
        pChapTitle->setPosition(ccp(75.0f, 25.0f));
        pChapTitle->setColor(isUnlocked ? ccc3(255, 235, 120) : ccc3(140, 140, 140));
        pItem->addChild(pChapTitle);

        // Icon minh họa chương
        CCSprite* pIcon = CCSprite::create("res/ui/chapter_icon_default.png");
        if (!pIcon) {
            // Icon fallback từ atlas giao diện
            pIcon = CCSprite::create();
        }
        if (pIcon) {
            pIcon->setPosition(ccp(75.0f, 100.0f));
            if (!isUnlocked) {
                pIcon->setColor(ccc3(90, 90, 90));
            }
            pItem->addChild(pIcon);
        }

        CCMenu* pMenu = CCMenu::create(pItem, NULL);
        pMenu->setPosition(CCPointZero);
        pageNode->addChild(pMenu, 10);
    }
}

void CChapterView::onChapterClicked(CCObject* pSender) {
    CCMenuItem* pItem = dynamic_cast<CCMenuItem*>(pSender);
    if (!pItem) return;

    int chapterId = pItem->getTag();
    if (!CChapterMgr::sharedManager()->isChapterUnlocked(chapterId)) {
        CCLog("[CChapterView] Chuong %d dang bi khoa!", chapterId);
        return;
    }

    CCLog("[CChapterView] Mo Chuong %d cot truyen", chapterId);
    CChapterMgr::sharedManager()->setSelectedChapter(chapterId);

    // Chuyển sang CSubChapterView
    CSubChapterView* pSubView = CSubChapterView::createWithChapter(chapterId);
    if (pSubView && this->getParent()) {
        CCNode* pParent = this->getParent();
        pParent->addChild(pSubView, this->getZOrder() + 1);
        this->removeFromParentAndCleanup(false);
    }
}

void CCombatPowerCalc(int& totalAtk, int& totalDef) {
    totalAtk = 0;
    totalDef = 0;
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData || !pData->getActiveTeam()) return;

    totalAtk = pData->getActiveTeam()->firefly_GetAttack();
    totalDef = pData->getActiveTeam()->firefly_GetDefense();
}

void CChapterView::updateCombatPowerLabels() {
    int totalAtk = 0, totalDef = 0;
    CCombatPowerCalc(totalAtk, totalDef);

    if (m_pLabelMaxAttack) {
        std::stringstream ss;
        ss << totalAtk;
        m_pLabelMaxAttack->setString(ss.str().c_str());
    }

    if (m_pLabelMaxDefense) {
        std::stringstream ss;
        ss << totalDef;
        m_pLabelMaxDefense->setString(ss.str().c_str());
    }

    if (m_pLabelMaxHonor) {
        CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
        if (pData) {
            std::stringstream ss;
            ss << pData->firefly_GetGold();
            m_pLabelMaxHonor->setString(ss.str().c_str());
        }
    }
}

void CChapterView::refreshView() {
    updatePageDisplay(m_currentPage);
    updateCombatPowerLabels();
}
