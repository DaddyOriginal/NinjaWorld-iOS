#include "CRouletteTurnDialogView.h"
#include "CCBManager.h"
#include <sstream>

CRouletteTurnDialogView::CRouletteTurnDialogView()
    : m_pBtnClose(NULL)
    , m_pLabelTitle(NULL)
{
    for (int i = 0; i < 10; ++i) {
        m_pSprIcon[i] = NULL;
        m_pLabelName[i] = NULL;
        m_pLabelCount[i] = NULL;
        m_pBtnCard[i] = NULL;
    }
}

CRouletteTurnDialogView::~CRouletteTurnDialogView() {
    CC_SAFE_RELEASE_NULL(m_pBtnClose);
    CC_SAFE_RELEASE_NULL(m_pLabelTitle);

    for (int i = 0; i < 10; ++i) {
        CC_SAFE_RELEASE_NULL(m_pSprIcon[i]);
        CC_SAFE_RELEASE_NULL(m_pLabelName[i]);
        CC_SAFE_RELEASE_NULL(m_pLabelCount[i]);
        CC_SAFE_RELEASE_NULL(m_pBtnCard[i]);
    }
}

CRouletteTurnDialogView* CRouletteTurnDialogView::createWithItems(const std::vector<RouletteSpinItem>& items) {
    CRouletteTurnDialogView* pView = new CRouletteTurnDialogView();
    if (pView && pView->initWithItems(items)) {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}

bool CRouletteTurnDialogView::initWithItems(const std::vector<RouletteSpinItem>& items) {
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 180))) {
        return false;
    }

    m_items = items;

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("RouletteTurnDialogView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/RouletteTurnDialogView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
    }

    updateUI();
    return true;
}

void CRouletteTurnDialogView::onEnter() {
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

void CRouletteTurnDialogView::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

bool CRouletteTurnDialogView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    return true;
}

void CRouletteTurnDialogView::Show(CCNode* pParent, int zOrder) {
    if (pParent) {
        pParent->addChild(this, zOrder);
    }
}

void CRouletteTurnDialogView::updateUI() {
    for (size_t i = 0; i < 10; ++i) {
        if (i < m_items.size()) {
            const RouletteSpinItem& item = m_items[i];
            if (m_pLabelName[i]) {
                m_pLabelName[i]->setString(item.name.empty() ? "Vật Phẩm" : item.name.c_str());
            }
            if (m_pLabelCount[i]) {
                std::stringstream ss;
                ss << "*" << item.count;
                m_pLabelCount[i]->setString(ss.str().c_str());
            }
        }
    }
}

// -------------------------------------------------------------
// CCB RESOLVERS
// -------------------------------------------------------------
SEL_MenuHandler CRouletteTurnDialogView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CRouletteTurnDialogView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "dialogClose") == 0 || strcmp(pSelectorName, "closeButton") == 0) {
        return cccontrol_selector(CRouletteTurnDialogView::onBtnClose);
    }
    return NULL;
}

bool CRouletteTurnDialogView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "closeButton", CCControlButton*, this->m_pBtnClose);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "labelTitle", CCLabelTTF*, this->m_pLabelTitle);

    for (int i = 0; i < 10; ++i) {
        char buf[64];
        snprintf(buf, sizeof(buf), "sprite_itemicon%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCSprite*, this->m_pSprIcon[i]);

        snprintf(buf, sizeof(buf), "label_cardname%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCLabelTTF*, this->m_pLabelName[i]);

        snprintf(buf, sizeof(buf), "label_count%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCLabelTTF*, this->m_pLabelCount[i]);

        snprintf(buf, sizeof(buf), "btn_card%d", i + 1);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, buf, CCControlButton*, this->m_pBtnCard[i]);
    }

    return false;
}

void CRouletteTurnDialogView::onBtnClose(CCObject* pSender, CCControlEvent pEvent) {
    this->removeFromParentAndCleanup(true);
}
