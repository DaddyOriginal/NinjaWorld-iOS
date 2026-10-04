#include "CGrowthFundView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"

// =============================================================
// CGrowthFundCell
// =============================================================
CGrowthFundCell::CGrowthFundCell()
    : m_pParentView(NULL)
    , m_pNodePropsIcon(NULL)
    , m_pLabelPropsCount(NULL)
    , m_pSpriteHasGet(NULL)
    , m_pSpriteBtnGetFund(NULL)
    , m_pLabelFundTitle(NULL)
    , m_pLabelPropsDesc(NULL)
    , m_pLabelGrowthFundCell0(NULL)
{
}

CGrowthFundCell::~CGrowthFundCell() {
    CC_SAFE_RELEASE_NULL(m_pNodePropsIcon);
    CC_SAFE_RELEASE_NULL(m_pLabelPropsCount);
    CC_SAFE_RELEASE_NULL(m_pSpriteHasGet);
    CC_SAFE_RELEASE_NULL(m_pSpriteBtnGetFund);
    CC_SAFE_RELEASE_NULL(m_pLabelFundTitle);
    CC_SAFE_RELEASE_NULL(m_pLabelPropsDesc);
    CC_SAFE_RELEASE_NULL(m_pLabelGrowthFundCell0);
}

CGrowthFundCell* CGrowthFundCell::create(const SGrowthFundItem& item, CGrowthFundView* pParentView) {
    CGrowthFundCell* pCell = new CGrowthFundCell();
    if (pCell && pCell->init(item, pParentView)) {
        pCell->autorelease();
        return pCell;
    }
    CC_SAFE_DELETE(pCell);
    return NULL;
}

bool CGrowthFundCell::init(const SGrowthFundItem& item, CGrowthFundView* pParentView) {
    m_pParentView = pParentView;
    m_item = item;

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("growth_fund_cell.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("activity/growth_fund_cell.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        this->setContentSize(pNode->getContentSize());
    }

    setItemData(item);
    return true;
}

void CGrowthFundCell::setItemData(const SGrowthFundItem& item) {
    m_item = item;

    if (m_pLabelFundTitle) {
        m_pLabelFundTitle->setString(m_item.title.c_str());
    }

    if (m_pLabelPropsDesc) {
        m_pLabelPropsDesc->setString(m_item.desc.c_str());
    }

    if (m_pLabelPropsCount) {
        m_pLabelPropsCount->setString(CCString::createWithFormat("%d", m_item.cash)->getCString());
    }

    if (m_item.state == 2) {
        // Đã nhận
        if (m_pSpriteHasGet) m_pSpriteHasGet->setVisible(true);
        if (m_pSpriteBtnGetFund) m_pSpriteBtnGetFund->setVisible(false);
    } else if (m_item.state == 1) {
        // Có thể nhận
        if (m_pSpriteHasGet) m_pSpriteHasGet->setVisible(false);
        if (m_pSpriteBtnGetFund) {
            m_pSpriteBtnGetFund->setVisible(true);
            m_pSpriteBtnGetFund->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
        }
    } else {
        // Chưa đạt điều kiện
        if (m_pSpriteHasGet) m_pSpriteHasGet->setVisible(false);
        if (m_pSpriteBtnGetFund) {
            m_pSpriteBtnGetFund->setVisible(true);
        }
    }
}

SEL_MenuHandler CGrowthFundCell::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CGrowthFundCell::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "btn_get_fund") == 0 || strcmp(pSelectorName, "onBtnGetFund") == 0) {
        return cccontrol_selector(CGrowthFundCell::onBtnGetFund);
    }
    return NULL;
}

bool CGrowthFundCell::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_props_icon", CCNode*, this->m_pNodePropsIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_props_count", CCLabelBMFont*, this->m_pLabelPropsCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_has_get", CCSprite*, this->m_pSpriteHasGet);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_btn_get_fund", CCNode*, this->m_pSpriteBtnGetFund);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_fund_title", CCLabelTTF*, this->m_pLabelFundTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_props_desc", CCLabelTTF*, this->m_pLabelPropsDesc);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_growth_fund_cell0", CCLabelTTF*, this->m_pLabelGrowthFundCell0);

    return false;
}

void CGrowthFundCell::onBtnGetFund(CCObject* pSender, CCControlEvent pEvent) {
    onClickCell();
}

void CGrowthFundCell::onClickCell() {
    if (m_item.state == 1) {
        CCLog("[CGrowthFundCell] Nhan thuong moc Quy Truong Thanh: ID=%d, Level=%d, Cash=%d", m_item.id, m_item.level, m_item.cash);
        CGrowthFundMgr::sharedManager()->requestClaimMilestone(m_item.id, m_pParentView, callfuncO_selector(CGrowthFundView::onFundDataLoaded));
    } else if (m_item.state == 0) {
        CCLog("[CGrowthFundCell] Chua dat moc cap do %d", m_item.level);
    }
}

// =============================================================
// CGrowthFundView
// =============================================================
CGrowthFundView::CGrowthFundView()
    : m_pNodeGiftCellNode(NULL)
    , m_pNodeTableContent(NULL)
    , m_pBtnCharge(NULL)
    , m_pBtnBuyFund(NULL)
    , m_pLabelFundDesc1(NULL)
    , m_pLabelFundDesc2(NULL)
    , m_pLabelFundDesc3(NULL)
    , m_pLabelFundDesc4(NULL)
    , m_pLabelFundDesc5(NULL)
    , m_pLabelVal1(NULL)
    , m_pLabelVal2(NULL)
    , m_pLabelVal3(NULL)
    , m_pLabelGrowthFund0(NULL)
    , m_pLabelGrowthFund1(NULL)
    , m_pTableView(NULL)
    , m_cellSize(CCSizeMake(480, 90))
{
}

CGrowthFundView::~CGrowthFundView() {
    CC_SAFE_RELEASE_NULL(m_pNodeGiftCellNode);
    CC_SAFE_RELEASE_NULL(m_pNodeTableContent);
    CC_SAFE_RELEASE_NULL(m_pBtnCharge);
    CC_SAFE_RELEASE_NULL(m_pBtnBuyFund);
    CC_SAFE_RELEASE_NULL(m_pLabelFundDesc1);
    CC_SAFE_RELEASE_NULL(m_pLabelFundDesc2);
    CC_SAFE_RELEASE_NULL(m_pLabelFundDesc3);
    CC_SAFE_RELEASE_NULL(m_pLabelFundDesc4);
    CC_SAFE_RELEASE_NULL(m_pLabelFundDesc5);
    CC_SAFE_RELEASE_NULL(m_pLabelVal1);
    CC_SAFE_RELEASE_NULL(m_pLabelVal2);
    CC_SAFE_RELEASE_NULL(m_pLabelVal3);
    CC_SAFE_RELEASE_NULL(m_pLabelGrowthFund0);
    CC_SAFE_RELEASE_NULL(m_pLabelGrowthFund1);
}

bool CGrowthFundView::init() {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("growth_fund.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("activity/growth_fund.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    }

    if (m_pNodeGiftCellNode) {
        m_cellSize = m_pNodeGiftCellNode->getContentSize();
    }

    if (m_pNodeTableContent) {
        m_pTableView = CCTableView::create(this, m_pNodeTableContent->getContentSize());
        if (m_pTableView) {
            m_pTableView->setDirection(kCCScrollViewDirectionVertical);
            m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
            m_pTableView->setDelegate(this);
            m_pNodeTableContent->addChild(m_pTableView);
        }
    }

    return true;
}

void CGrowthFundView::onEnter() {
    CCLayerColor::onEnter();
    refreshView();
    CGrowthFundMgr::sharedManager()->requestFundInfo(this, callfuncO_selector(CGrowthFundView::onFundDataLoaded));
}

void CGrowthFundView::onExit() {
    CCLayerColor::onExit();
}

void CGrowthFundView::Show(CCNode* pParent, int zOrder) {
    if (!pParent) return;
    pParent->addChild(this, zOrder);
}

void CGrowthFundView::refreshView() {
    const SGrowthFundPreview& prev = CGrowthFundMgr::sharedManager()->getPreview();

    if (m_pLabelVal1) {
        m_pLabelVal1->setString(CCString::createWithFormat("%d", prev.buyCash)->getCString());
    }
    if (m_pLabelVal2) {
        m_pLabelVal2->setString(CCString::createWithFormat("%d", prev.multiple)->getCString());
    }
    if (m_pLabelVal3) {
        m_pLabelVal3->setString(CCString::createWithFormat("%d", prev.totalCash)->getCString());
    }

    if (m_pLabelFundDesc1) m_pLabelFundDesc1->setString("Chỉ cần");
    if (m_pLabelFundDesc2) m_pLabelFundDesc2->setString("Vàng mua quỹ");
    if (m_pLabelFundDesc3) m_pLabelFundDesc3->setString("Nhận lại gấp");
    if (m_pLabelFundDesc4) m_pLabelFundDesc4->setString("lần, tổng");
    if (m_pLabelFundDesc5) m_pLabelFundDesc5->setString("Vàng!");

    if (m_pBtnBuyFund) {
        m_pBtnBuyFund->setEnabled(prev.buyState == 0);
    }

    if (m_pTableView) {
        m_pTableView->reloadData();
    }
}

CCSize CGrowthFundView::cellSizeForTable(CCTableView* table) {
    return m_cellSize;
}

CCTableViewCell* CGrowthFundView::tableCellAtIndex(CCTableView* table, unsigned int idx) {
    const std::vector<SGrowthFundItem>& milestones = CGrowthFundMgr::sharedManager()->getMilestones();
    if (idx >= milestones.size()) return NULL;

    CCTableViewCell* pCell = table->dequeueCell();
    CGrowthFundCell* pFundCell = dynamic_cast<CGrowthFundCell*>(pCell);

    if (!pFundCell) {
        pFundCell = CGrowthFundCell::create(milestones[idx], this);
    } else {
        pFundCell->setItemData(milestones[idx]);
    }

    return pFundCell;
}

unsigned int CGrowthFundView::numberOfCellsInTableView(CCTableView* table) {
    return (unsigned int)CGrowthFundMgr::sharedManager()->getMilestoneCount();
}

void CGrowthFundView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    CGrowthFundCell* pFundCell = dynamic_cast<CGrowthFundCell*>(cell);
    if (pFundCell) {
        pFundCell->onClickCell();
    }
}

SEL_MenuHandler CGrowthFundView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "dialogClose") == 0) {
        return menu_selector(CGrowthFundView::dialogClose);
    }
    return NULL;
}

SEL_CCControlHandler CGrowthFundView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "btn_buy_fund") == 0 || strcmp(pSelectorName, "onBtnBuyFund") == 0) {
        return cccontrol_selector(CGrowthFundView::onBtnBuyFund);
    }
    if (strcmp(pSelectorName, "btn_charge") == 0 || strcmp(pSelectorName, "onBtnCharge") == 0) {
        return cccontrol_selector(CGrowthFundView::onBtnCharge);
    }
    if (strcmp(pSelectorName, "btn_close") == 0 || strcmp(pSelectorName, "onBtnClose") == 0) {
        return cccontrol_selector(CGrowthFundView::onBtnClose);
    }
    return NULL;
}

bool CGrowthFundView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_gift_cell_node", CCNode*, this->m_pNodeGiftCellNode);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_table_content", CCNode*, this->m_pNodeTableContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_charge", CCControlButton*, this->m_pBtnCharge);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_buy_fund", CCControlButton*, this->m_pBtnBuyFund);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_fund_desc1", CCLabelTTF*, this->m_pLabelFundDesc1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_fund_desc2", CCLabelTTF*, this->m_pLabelFundDesc2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_fund_desc3", CCLabelTTF*, this->m_pLabelFundDesc3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_fund_desc4", CCLabelTTF*, this->m_pLabelFundDesc4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_fund_desc5", CCLabelTTF*, this->m_pLabelFundDesc5);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_val_1", CCLabelBMFont*, this->m_pLabelVal1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_val_2", CCLabelBMFont*, this->m_pLabelVal2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_val_3", CCLabelBMFont*, this->m_pLabelVal3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_growth_fund0", CCLabelTTF*, this->m_pLabelGrowthFund0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_growth_fund1", CCLabelTTF*, this->m_pLabelGrowthFund1);

    return false;
}

void CGrowthFundView::onBtnBuyFund(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CGrowthFundView] Mua Quy Truong Thanh");
    CGrowthFundMgr::sharedManager()->requestBuyFund(this, callfuncO_selector(CGrowthFundView::onFundDataLoaded));
}

void CGrowthFundView::onBtnCharge(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CGrowthFundView] Chuyen den man hinh nap tien");
}

void CGrowthFundView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}

void CGrowthFundView::dialogClose(CCObject* pSender) {
    this->removeFromParentAndCleanup(true);
}

void CGrowthFundView::onFundDataLoaded(CCObject* pData) {
    refreshView();
}

void CGrowthFundView::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool CGrowthFundView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true; // Nuốt chạm nền
}
