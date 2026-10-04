#include "CAwardCenterView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include <ctime>

// =============================================================
// CAwardCenterCell
// =============================================================
CAwardCenterCell::CAwardCenterCell()
    : m_pParentView(NULL)
    , m_pNodeContent(NULL)
    , m_pNodeCell(NULL)
    , m_pLabelItemTitle(NULL)
    , m_pLabelAwardTime(NULL)
    , m_pLabelDesc(NULL)
    , m_pBtnGet(NULL)
{
}

CAwardCenterCell::~CAwardCenterCell() {
    CC_SAFE_RELEASE_NULL(m_pNodeContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCell);
    CC_SAFE_RELEASE_NULL(m_pLabelItemTitle);
    CC_SAFE_RELEASE_NULL(m_pLabelAwardTime);
    CC_SAFE_RELEASE_NULL(m_pLabelDesc);
    CC_SAFE_RELEASE_NULL(m_pBtnGet);
}

CAwardCenterCell* CAwardCenterCell::create(const SAwardMsg& msg, CAwardCenterView* pParentView) {
    CAwardCenterCell* pCell = new CAwardCenterCell();
    if (pCell && pCell->init(msg, pParentView)) {
        pCell->autorelease();
        return pCell;
    }
    CC_SAFE_DELETE(pCell);
    return NULL;
}

bool CAwardCenterCell::init(const SAwardMsg& msg, CAwardCenterView* pParentView) {
    m_pParentView = pParentView;
    m_msg = msg;

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("AwardCenterCell.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("activity/AwardCenterCell.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        this->setContentSize(pNode->getContentSize());
    }

    setMsgData(msg);
    return true;
}

void CAwardCenterCell::setMsgData(const SAwardMsg& msg) {
    m_msg = msg;

    if (m_pLabelItemTitle) {
        m_pLabelItemTitle->setString(m_msg.title.c_str());
    }

    if (m_pLabelDesc) {
        m_pLabelDesc->setString(m_msg.content.c_str());
    }

    if (m_pLabelAwardTime) {
        time_t t = (time_t)m_msg.timestamp;
        struct tm* timeinfo = localtime(&t);
        char buf[64];
        if (timeinfo) {
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", timeinfo);
            m_pLabelAwardTime->setString(buf);
        } else {
            m_pLabelAwardTime->setString("Hôm nay");
        }
    }
}

SEL_MenuHandler CAwardCenterCell::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CAwardCenterCell::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "btn_get") == 0 || strcmp(pSelectorName, "onBtnGet") == 0) {
        return cccontrol_selector(CAwardCenterCell::onBtnGet);
    }
    return NULL;
}

bool CAwardCenterCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cell", CCNode*, this->m_pNodeCell);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_item_title", CCLabelTTF*, this->m_pLabelItemTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_award_time", CCLabelTTF*, this->m_pLabelAwardTime);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_desc", CCLabelTTF*, this->m_pLabelDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_get", CCControlButton*, this->m_pBtnGet);

    return false;
}

void CAwardCenterCell::onBtnGet(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CAwardCenterCell] Nhan thuong cho MsgID=%d", m_msg.id);
    CAwardCenterMgr::sharedManager()->requestClaimAward(m_msg.id, m_pParentView, callfuncO_selector(CAwardCenterView::onClaimSuccess));
}

// =============================================================
// CAwardCenterView
// =============================================================
CAwardCenterView::CAwardCenterView()
    : m_pNodeContent(NULL)
    , m_pNodeCell(NULL)
    , m_pLabelTitle(NULL)
    , m_pLabelExpiry(NULL)
    , m_pBtnClose(NULL)
    , m_pBtnGetAll(NULL)
    , m_pTableView(NULL)
    , m_cellSize(CCSizeMake(480, 100))
{
}

CAwardCenterView::~CAwardCenterView() {
    CC_SAFE_RELEASE_NULL(m_pNodeContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCell);
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);
    CC_SAFE_RELEASE_NULL(m_pLabelExpiry);
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pBtnGetAll);
}

bool CAwardCenterView::init() {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("AwardCenterView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("activity/AwardCenterView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    }

    if (m_pLabelTitle) {
        m_pLabelTitle->setString("Trung Tâm Trao Thưởng");
    }

    if (m_pNodeCell) {
        m_cellSize = m_pNodeCell->getContentSize();
    }

    if (m_pNodeContent) {
        m_pTableView = CCTableView::create(this, m_pNodeContent->getContentSize());
        if (m_pTableView) {
            m_pTableView->setDirection(kCCScrollViewDirectionVertical);
            m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
            m_pTableView->setDelegate(this);
            m_pNodeContent->addChild(m_pTableView);
        }
    }

    return true;
}

void CAwardCenterView::onEnter() {
    CCLayerColor::onEnter();
    refreshView();
    CAwardCenterMgr::sharedManager()->requestAwardList(this, callfuncO_selector(CAwardCenterView::onAwardListLoaded));
}

void CAwardCenterView::onExit() {
    CCLayerColor::onExit();
}

void CAwardCenterView::Show(CCNode* pParent, int zOrder) {
    if (!pParent) return;
    pParent->addChild(this, zOrder);
}

void CAwardCenterView::refreshView() {
    if (m_pLabelExpiry) {
        std::string exp = CAwardCenterMgr::sharedManager()->getExpiry();
        if (exp.empty()) exp = "30 ngày";
        m_pLabelExpiry->setString(CCString::createWithFormat("Hạn giữ thư: %s", exp.c_str())->getCString());
    }

    if (m_pTableView) {
        m_pTableView->reloadData();
    }
}

CCSize CAwardCenterView::cellSizeForTable(CCTableView* table) {
    return m_cellSize;
}

CCTableViewCell* CAwardCenterView::tableCellAtIndex(CCTableView* table, unsigned int idx) {
    const std::vector<SAwardMsg>& list = CAwardCenterMgr::sharedManager()->getAwardList();
    if (idx >= list.size()) return NULL;

    CCTableViewCell* pCell = table->dequeueCell();
    CAwardCenterCell* pAwardCell = dynamic_cast<CAwardCenterCell*>(pCell);

    if (!pAwardCell) {
        pAwardCell = CAwardCenterCell::create(list[idx], this);
    } else {
        pAwardCell->setMsgData(list[idx]);
    }

    return pAwardCell;
}

unsigned int CAwardCenterView::numberOfCellsInTableView(CCTableView* table) {
    return (unsigned int)CAwardCenterMgr::sharedManager()->getAwardCount();
}

void CAwardCenterView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
}

SEL_MenuHandler CAwardCenterView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "dialogClose") == 0) {
        return menu_selector(CAwardCenterView::dialogClose);
    }
    return NULL;
}

SEL_CCControlHandler CAwardCenterView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "btn_close") == 0 || strcmp(pSelectorName, "onBtnClose") == 0) {
        return cccontrol_selector(CAwardCenterView::onBtnClose);
    }
    if (strcmp(pSelectorName, "btn_getall") == 0 || strcmp(pSelectorName, "onBtnGetAll") == 0) {
        return cccontrol_selector(CAwardCenterView::onBtnGetAll);
    }
    return NULL;
}

bool CAwardCenterView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cell", CCNode*, this->m_pNodeCell);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_title", CCLabelTTF*, this->m_pLabelTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_expiry", CCLabelTTF*, this->m_pLabelExpiry);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_close", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_getall", CCControlButton*, this->m_pBtnGetAll);

    return false;
}

void CAwardCenterView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CAwardCenterView::dialogClose(CCObject* pSender) {
    this->removeFromParentAndCleanup(true);
}

void CAwardCenterView::onBtnGetAll(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CAwardCenterView] Nguoi choi bam Nhan Tat Ca qua thuong");
    CAwardCenterMgr::sharedManager()->requestClaimAll(this, callfuncO_selector(CAwardCenterView::onClaimSuccess));
}

void CAwardCenterView::onAwardListLoaded(CCObject* pData) {
    refreshView();
}

void CAwardCenterView::onClaimSuccess(CCObject* pData) {
    refreshView();
}

void CAwardCenterView::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool CAwardCenterView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; // Nuốt chạm nền modal
}
