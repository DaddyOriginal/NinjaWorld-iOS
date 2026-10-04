#include "CDailyTaskView.h"
#include "CDailyRewardView.h"
#include "CCBManager.h"
#include "CMainMenu.h"
#include "CPlayerDataMgr.h"
#include <sstream>

// =============================================================
// CDailyTaskCell
// =============================================================
CDailyTaskCell::CDailyTaskCell()
    : m_pParentView(NULL)
    , m_pSprTaskIcon(NULL)
    , m_pLabelName(NULL)
    , m_pLabelProgress(NULL)
    , m_pLabelDesc(NULL)
    , m_pLabelScore(NULL)
    , m_pBtnGoto(NULL)
    , m_pSprTaskDone(NULL)
{
}

CDailyTaskCell::~CDailyTaskCell() {
    CC_SAFE_RELEASE_NULL(m_pSprTaskIcon);
    CC_SAFE_RELEASE_NULL(m_pLabelName);
    CC_SAFE_RELEASE_NULL(m_pLabelProgress);
    CC_SAFE_RELEASE_NULL(m_pLabelDesc);
    CC_SAFE_RELEASE_NULL(m_pLabelScore);
    CC_SAFE_RELEASE_NULL(m_pBtnGoto);
    CC_SAFE_RELEASE_NULL(m_pSprTaskDone);
}

CDailyTaskCell* CDailyTaskCell::create(const DailyTaskItem& item, CDailyTaskView* pParentView) {
    CDailyTaskCell* pCell = new CDailyTaskCell();
    if (pCell && pCell->init(item, pParentView)) {
        pCell->autorelease();
        return pCell;
    }
    CC_SAFE_DELETE(pCell);
    return NULL;
}

bool CDailyTaskCell::init(const DailyTaskItem& item, CDailyTaskView* pParentView) {
    m_item = item;
    m_pParentView = pParentView;

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("DailyTaskCell.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/DailyTaskCell.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        this->setContentSize(pNode->getContentSize());
    }

    if (m_pLabelName) {
        m_pLabelName->setString(m_item.title.c_str());
    }
    if (m_pLabelDesc) {
        m_pLabelDesc->setString(m_item.desc.c_str());
    }
    if (m_pLabelScore) {
        std::stringstream ss;
        ss << "+" << m_item.points;
        m_pLabelScore->setString(ss.str().c_str());
    }
    if (m_pLabelProgress) {
        std::stringstream ss;
        ss << m_item.curProcess << "/" << m_item.needProcess;
        m_pLabelProgress->setString(ss.str().c_str());
    }

    if (m_item.isDone) {
        if (m_pSprTaskDone) m_pSprTaskDone->setVisible(true);
        if (m_pBtnGoto) m_pBtnGoto->setVisible(false);
    } else {
        if (m_pSprTaskDone) m_pSprTaskDone->setVisible(false);
        if (m_pBtnGoto) m_pBtnGoto->setVisible(true);
    }

    // Tự động gán icon nhiệm vụ nếu có frame
    if (m_pSprTaskIcon) {
        char iconName[64];
        snprintf(iconName, sizeof(iconName), "daily_icon_%d", m_item.id);
        CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(iconName);
        if (pFrame) {
            CCSprite* pIcon = CCSprite::createWithSpriteFrame(pFrame);
            if (pIcon) {
                CCSize iconSize = m_pSprTaskIcon->getContentSize();
                pIcon->setPosition(ccp(iconSize.width * 0.5f, iconSize.height * 0.5f));
                m_pSprTaskIcon->addChild(pIcon);
            }
        }
    }

    return true;
}

SEL_MenuHandler CDailyTaskCell::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CDailyTaskCell::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnGoto") == 0 || strcmp(pSelectorName, "btn_goto") == 0) {
        return cccontrol_selector(CDailyTaskCell::onBtnGotoClicked);
    }
    return NULL;
}

bool CDailyTaskCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_task_icon", CCSprite*, this->m_pSprTaskIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_name", CCLabelTTF*, this->m_pLabelName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_progress", CCLabelTTF*, this->m_pLabelProgress);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_desc", CCLabelTTF*, this->m_pLabelDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_score", CCLabelTTF*, this->m_pLabelScore);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_goto", CCControlButton*, this->m_pBtnGoto);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_task_done", CCSprite*, this->m_pSprTaskDone);
    return false;
}

void CDailyTaskCell::onBtnGotoClicked(CCObject* pSender, CCControlEvent pEvent) {
    if (m_pParentView) {
        m_pParentView->onGotoTask(m_item.id);
    }
}

// =============================================================
// CDailyTaskAwardCell
// =============================================================
CDailyTaskAwardCell::CDailyTaskAwardCell()
    : m_pParentView(NULL)
    , m_pSprBox(NULL)
    , m_pLabelNeedScore(NULL)
{
}

CDailyTaskAwardCell::~CDailyTaskAwardCell() {
    CC_SAFE_RELEASE_NULL(m_pSprBox);
    CC_SAFE_RELEASE_NULL(m_pLabelNeedScore);
}

CDailyTaskAwardCell* CDailyTaskAwardCell::create(const DailyTaskAwardBox& box, CDailyTaskView* pParentView) {
    CDailyTaskAwardCell* pCell = new CDailyTaskAwardCell();
    if (pCell && pCell->init(box, pParentView)) {
        pCell->autorelease();
        return pCell;
    }
    CC_SAFE_DELETE(pCell);
    return NULL;
}

bool CDailyTaskAwardCell::init(const DailyTaskAwardBox& box, CDailyTaskView* pParentView) {
    m_box = box;
    m_pParentView = pParentView;

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("DailyTaskAwardCell.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/DailyTaskAwardCell.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        this->setContentSize(pNode->getContentSize());
    }

    if (m_pLabelNeedScore) {
        std::stringstream ss;
        ss << m_box.cost << " Điểm";
        m_pLabelNeedScore->setString(ss.str().c_str());
        m_pLabelNeedScore->setColor(ccc3(119, 52, 28));
    }

    if (m_pSprBox) {
        const char* frameName = "daily_box";
        if (m_box.status == 2) {
            frameName = "daily_box_open";
        } else if (m_box.status == 1) {
            frameName = "daily_box_close";
        }
        CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        if (pFrame) {
            m_pSprBox->setDisplayFrame(pFrame);
        }
    }

    return true;
}

SEL_MenuHandler CDailyTaskAwardCell::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CDailyTaskAwardCell::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

bool CDailyTaskAwardCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_box", CCSprite*, this->m_pSprBox);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_need_score", CCLabelTTF*, this->m_pLabelNeedScore);
    return false;
}

// =============================================================
// CDailyTaskView
// =============================================================
CDailyTaskView::CDailyTaskView()
    : m_pLabelCurScore(NULL)
    , m_pLabelTitle(NULL)
    , m_pNodeContent(NULL)
    , m_pNodeCell(NULL)
    , m_pNodeAwardContent(NULL)
    , m_pNodeAwardCell(NULL)
    , m_pBtnClose(NULL)
    , m_pBtnPreAward(NULL)
    , m_pBtnNextAward(NULL)
    , m_pSprProgress(NULL)
    , m_pScrollTasks(NULL)
    , m_pScrollAwards(NULL)
    , m_curAwardIndex(0)
{
}

CDailyTaskView::~CDailyTaskView() {
    CC_SAFE_RELEASE_NULL(m_pLabelCurScore);
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);
    CC_SAFE_RELEASE_NULL(m_pNodeContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCell);
    CC_SAFE_RELEASE_NULL(m_pNodeAwardContent);
    CC_SAFE_RELEASE_NULL(m_pNodeAwardCell);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnPreAward);
    CC_SAFE_RELEASE_NULL(m_pBtnNextAward);
    CC_SAFE_RELEASE_NULL(m_pSprProgress);

    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationDailyTasksUpdated);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, kNotificationDailyBoxClaimed);
}

CDailyTaskView* CDailyTaskView::create() {
    CDailyTaskView* pView = new CDailyTaskView();
    if (pView && pView->init()) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CDailyTaskView::init() {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("DailyTaskView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/DailyTaskView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    // Đăng ký nhận thông báo mạng từ CDailyTaskMgr
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CDailyTaskView::onDailyTasksUpdated), kNotificationDailyTasksUpdated, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CDailyTaskView::onDailyBoxClaimed), kNotificationDailyBoxClaimed, NULL);

    return true;
}

void CDailyTaskView::onEnter() {
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

    // Gửi request lấy dữ liệu nhiệm vụ hàng ngày từ server
    CDailyTaskMgr::sharedManager()->requestDailyTasks();
}

void CDailyTaskView::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool CDailyTaskView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void CDailyTaskView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint loc = pTouch->getLocation();

    // Kiểm tra xem người chơi có chạm vào ô rương nào không
    for (size_t i = 0; i < m_awardCells.size(); ++i) {
        CDailyTaskAwardCell* cell = m_awardCells[i];
        if (cell && cell->getBoxSprite()) {
            CCPoint localPoint = cell->getBoxSprite()->convertToNodeSpace(loc);
            CCSize s = cell->getBoxSprite()->getContentSize();
            CCRect rect = CCRectMake(0, 0, s.width, s.height);
            if (rect.containsPoint(localPoint)) {
                onOpenAwardBox(cell->getBoxId());
                break;
            }
        }
    }
}

void CDailyTaskView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CDailyTaskView::onDailyTasksUpdated(CCObject* pObj) {
    updateScoreHeader();
    buildAwardBoxList();
    buildTaskList();
}

void CDailyTaskView::onDailyBoxClaimed(CCObject* pObj) {
    updateScoreHeader();
    buildAwardBoxList();
}

void CDailyTaskView::updateScoreHeader() {
    int cur = CDailyTaskMgr::sharedManager()->getCurScore();
    int total = CDailyTaskMgr::sharedManager()->getTotalScore();

    if (m_pLabelCurScore) {
        std::stringstream ss;
        ss << cur << "/" << total;
        m_pLabelCurScore->setString(ss.str().c_str());
    }

    if (m_pSprProgress) {
        float scale = (total > 0) ? ((float)cur / (float)total) : 0.0f;
        if (scale > 1.0f) scale = 1.0f;
        m_pSprProgress->setScaleX(scale);
    }
}

void CDailyTaskView::buildAwardBoxList() {
    if (!m_pNodeAwardContent) return;

    m_pNodeAwardContent->removeAllChildrenWithCleanup(true);
    m_awardCells.clear();

    const std::vector<DailyTaskAwardBox>& boxes = CDailyTaskMgr::sharedManager()->getBoxList();
    if (boxes.empty()) return;

    CCSize containerSize = m_pNodeAwardContent->getContentSize();
    CCSize cellSize = m_pNodeAwardCell ? m_pNodeAwardCell->getContentSize() : CCSizeMake(110, 100);

    float totalWidth = cellSize.width * boxes.size();
    CCNode* pScrollContainer = CCNode::create();
    pScrollContainer->setContentSize(CCSizeMake(totalWidth, containerSize.height));

    for (size_t i = 0; i < boxes.size(); ++i) {
        CDailyTaskAwardCell* pCell = CDailyTaskAwardCell::create(boxes[i], this);
        if (pCell) {
            pCell->setPosition(ccp(i * cellSize.width + cellSize.width * 0.5f, containerSize.height * 0.5f));
            pScrollContainer->addChild(pCell);
            m_awardCells.push_back(pCell);
        }
    }

    m_pScrollAwards = CCScrollView::create(containerSize, pScrollContainer);
    m_pScrollAwards->setDirection(kCCScrollViewDirectionHorizontal);
    m_pScrollAwards->setBounceable(true);
    m_pNodeAwardContent->addChild(m_pScrollAwards);
}

void CDailyTaskView::buildTaskList() {
    if (!m_pNodeContent) return;

    m_pNodeContent->removeAllChildrenWithCleanup(true);

    const std::vector<DailyTaskItem>& tasks = CDailyTaskMgr::sharedManager()->getTaskList();
    if (tasks.empty()) return;

    CCSize containerSize = m_pNodeContent->getContentSize();
    CCSize cellSize = m_pNodeCell ? m_pNodeCell->getContentSize() : CCSizeMake(containerSize.width, 105);

    float totalHeight = cellSize.height * tasks.size();
    if (totalHeight < containerSize.height) totalHeight = containerSize.height;

    CCNode* pScrollContainer = CCNode::create();
    pScrollContainer->setContentSize(CCSizeMake(containerSize.width, totalHeight));

    for (size_t i = 0; i < tasks.size(); ++i) {
        CDailyTaskCell* pCell = CDailyTaskCell::create(tasks[i], this);
        if (pCell) {
            float yPos = totalHeight - (i + 1) * cellSize.height;
            pCell->setPosition(ccp(0, yPos));
            pScrollContainer->addChild(pCell);
        }
    }

    m_pScrollTasks = CCScrollView::create(containerSize, pScrollContainer);
    m_pScrollTasks->setDirection(kCCScrollViewDirectionVertical);
    m_pScrollTasks->setBounceable(true);
    m_pScrollTasks->setContentOffset(ccp(0, containerSize.height - totalHeight));
    m_pNodeContent->addChild(m_pScrollTasks);
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CDailyTaskView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CDailyTaskView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnClose") == 0 || strcmp(pSelectorName, "dialogClose") == 0) {
        return cccontrol_selector(CDailyTaskView::onBtnClose);
    }
    if (strcmp(pSelectorName, "onBtnPreAward") == 0 || strcmp(pSelectorName, "btn_pre_award") == 0) {
        return cccontrol_selector(CDailyTaskView::onBtnPreAward);
    }
    if (strcmp(pSelectorName, "onBtnNextAward") == 0 || strcmp(pSelectorName, "btn_next_award") == 0) {
        return cccontrol_selector(CDailyTaskView::onBtnNextAward);
    }
    return NULL;
}

bool CDailyTaskView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_cur_score", CCLabelTTF*, this->m_pLabelCurScore);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_title", CCLabelTTF*, this->m_pLabelTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cell", CCNode*, this->m_pNodeCell);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_award_content", CCNode*, this->m_pNodeAwardContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_award_cell", CCNode*, this->m_pNodeAwardCell);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "closeButton", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_pre_award", CCControlButton*, this->m_pBtnPreAward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_next_award", CCControlButton*, this->m_pBtnNextAward);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_progress", CCNode*, this->m_pSprProgress);
    return false;
}

void CDailyTaskView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CDailyTaskView::onBtnPreAward(CCObject* pSender, CCControlEvent pEvent) {
    if (m_pScrollAwards) {
        CCPoint offset = m_pScrollAwards->getContentOffset();
        offset.x += 110.0f;
        if (offset.x > 0) offset.x = 0;
        m_pScrollAwards->setContentOffsetInDuration(offset, 0.3f);
    }
}

void CDailyTaskView::onBtnNextAward(CCObject* pSender, CCControlEvent pEvent) {
    if (m_pScrollAwards) {
        CCPoint offset = m_pScrollAwards->getContentOffset();
        offset.x -= 110.0f;
        float minX = m_pScrollAwards->getViewSize().width - m_pScrollAwards->getContainer()->getContentSize().width;
        if (minX > 0) minX = 0;
        if (offset.x < minX) offset.x = minX;
        m_pScrollAwards->setContentOffsetInDuration(offset, 0.3f);
    }
}

void CDailyTaskView::onOpenAwardBox(int boxId) {
    const DailyTaskAwardBox* pBox = CDailyTaskMgr::sharedManager()->getBoxById(boxId);
    if (!pBox) return;

    CDailyRewardView* pRewardView = CDailyRewardView::createWithBox(*pBox);
    if (pRewardView) {
        pRewardView->Show(this, 100);
    }
}

void CDailyTaskView::onGotoTask(int taskId) {
    CCLog("[CDailyTaskView] Nguoi choi bam 'Di Ngay' den nhiem vu ID=%d", taskId);

    CMainMenu* pMenu = CMainMenu::sharedMainMenu();
    if (!pMenu) {
        this->removeFromParentAndCleanup(true);
        return;
    }

    switch (taskId) {
        case 1:  // Vượt ải cốt truyện
        case 2:  // Leo tháp Thí Luyện
            if (taskId == 1) pMenu->changeToSub(SUBMENU_DUNGEON);
            else pMenu->changeToSub(SUBMENU_TOWER);
            break;
        case 3:  // Ấn ký vĩ thú
        case 5:  // Cường hóa trang bị
        case 7:  // Bát môn độn giáp
        case 20: // Nâng cấp Nhẫn Giả
        case 21: // Trùng sinh
            pMenu->changeToSub(SUBMENU_NINJA);
            break;
        case 4:  // Đấu trường Lôi Đài
        case 19: // Uy danh Lôi Đài
            pMenu->changeToSub(SUBMENU_ARENA);
            break;
        case 14: // Chiêu mộ 1 vạn dặm
        case 18: // Mua đồ tại Tiệm
            pMenu->changeToSub(SUBMENU_SHOP);
            break;
        default:
            pMenu->changeToSub(SUBMENU_HOME);
            break;
    }

    this->removeFromParentAndCleanup(true);
}
