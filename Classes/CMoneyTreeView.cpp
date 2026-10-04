#include "CMoneyTreeView.h"
#include "CCBManager.h"
#include "CMainMenu.h"
#include "CPlayerDataMgr.h"
#include <cmath>

CMoneyTreeView::CMoneyTreeView()
    : m_pNodeWidth(NULL)
    , m_pNodeTips(NULL)
    , m_pNodeAnim(NULL)
    , m_pNodeContent(NULL)
    , m_pNodeCell(NULL)
    , m_pLabelGoldVal(NULL)
    , m_pLabelSilverVal(NULL)
    , m_pLabelTimes(NULL)
    , m_pLabelFreeTry(NULL)
    , m_pLabelTry(NULL)
    , m_pLabelGetSilver(NULL)
    , m_pLabelCostGold(NULL)
    , m_pLabelDesc(NULL)
    , m_pLabelMoneyTreeView0(NULL)
    , m_pLabelMoneyTreeView1(NULL)
    , m_pLabelMoneyTreeView2(NULL)
    , m_pLabelMoneyTreeView3(NULL)
    , m_pLabelMoneyTreeView4(NULL)
    , m_bIsSwinging(false)
    , m_lastShakeTime(0.0)
{
}

CMoneyTreeView::~CMoneyTreeView() {
    CC_SAFE_RELEASE_NULL(m_pNodeWidth);
    CC_SAFE_RELEASE_NULL(m_pNodeTips);
    CC_SAFE_RELEASE_NULL(m_pNodeAnim);
    CC_SAFE_RELEASE_NULL(m_pNodeContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCell);
    CC_SAFE_RELEASE_NULL(m_pLabelGoldVal);
    CC_SAFE_RELEASE_NULL(m_pLabelSilverVal);
    CC_SAFE_RELEASE_NULL(m_pLabelTimes);
    CC_SAFE_RELEASE_NULL(m_pLabelFreeTry);
    CC_SAFE_RELEASE_NULL(m_pLabelTry);
    CC_SAFE_RELEASE_NULL(m_pLabelGetSilver);
    CC_SAFE_RELEASE_NULL(m_pLabelCostGold);
    CC_SAFE_RELEASE_NULL(m_pLabelDesc);
    CC_SAFE_RELEASE_NULL(m_pLabelMoneyTreeView0);
    CC_SAFE_RELEASE_NULL(m_pLabelMoneyTreeView1);
    CC_SAFE_RELEASE_NULL(m_pLabelMoneyTreeView2);
    CC_SAFE_RELEASE_NULL(m_pLabelMoneyTreeView3);
    CC_SAFE_RELEASE_NULL(m_pLabelMoneyTreeView4);
}

bool CMoneyTreeView::init() {
    if (!CCLayer::init()) {
        return false;
    }

    CCNode* pNode = CCBManager::sharedManager()->loadNodeFromCCBI("MoneyTreeView.ccbi", this);
    if (!pNode) {
        pNode = CCBManager::sharedManager()->loadNodeFromCCBI("activity/MoneyTreeView.ccbi", this);
    }

    if (pNode) {
        this->addChild(pNode);
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        pNode->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    }

    return true;
}

void CMoneyTreeView::onEnter() {
    CCLayer::onEnter();
    setAccelerometerEnabled(true);
    refreshView();
    CMoneyTreeMgr::sharedManager()->requestTreeInfo(this, callfuncO_selector(CMoneyTreeView::onTreeDataLoaded));
}

void CMoneyTreeView::onExit() {
    setAccelerometerEnabled(false);
    CCLayer::onExit();
}

void CMoneyTreeView::refreshView() {
    const SMoneyTreeConfig& cf = CMoneyTreeMgr::sharedManager()->getConfig();
    const SMoneyTreeUser& user = CMoneyTreeMgr::sharedManager()->getUser();

    if (m_pLabelGoldVal && CPlayerDataMgr::sharedManager()) {
        m_pLabelGoldVal->setString(CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getGold())->getCString());
    }
    if (m_pLabelSilverVal && CPlayerDataMgr::sharedManager()) {
        m_pLabelSilverVal->setString(CCString::createWithFormat("%d", CPlayerDataMgr::sharedManager()->getSilver())->getCString());
    }

    int remain = cf.timesPerDay - user.usedTimes;
    if (remain < 0) remain = 0;
    if (m_pLabelTimes) {
        m_pLabelTimes->setString(CCString::createWithFormat("%d/%d", remain, cf.timesPerDay)->getCString());
    }

    int freeRem = cf.freeShake - user.freeTimes;
    if (freeRem < 0) freeRem = 0;
    if (m_pLabelFreeTry) {
        m_pLabelFreeTry->setString(CCString::createWithFormat("%d", freeRem)->getCString());
    }

    if (m_pLabelCostGold) {
        if (freeRem > 0) {
            m_pLabelCostGold->setString("Miễn phí");
        } else {
            m_pLabelCostGold->setString(CCString::createWithFormat("%d", cf.shakeCost)->getCString());
        }
    }

    if (m_pLabelGetSilver) {
        m_pLabelGetSilver->setString(CCString::createWithFormat("+%d", cf.startSilver)->getCString());
    }

    if (m_pLabelDesc) {
        m_pLabelDesc->setString("Lắc thiết bị hoặc bấm nút để rung cây nhận Bạc!");
    }
}

void CMoneyTreeView::RunSwingTreeAnim() {
    if (!m_pNodeAnim) return;

    CCActionInterval* rotLeft = CCRotateBy::create(0.06f, -6.0f);
    CCActionInterval* rotRight = CCRotateBy::create(0.12f, 12.0f);
    CCActionInterval* rotBack = CCRotateBy::create(0.06f, -6.0f);
    CCSequence* shakeSeq = CCSequence::create(rotLeft, rotRight, rotBack, NULL);
    CCRepeat* repeatShake = CCRepeat::create(shakeSeq, 3);

    m_pNodeAnim->runAction(repeatShake);
}

void CMoneyTreeView::onBtnBack(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CMoneyTreeView] Quay ve Trang Chu");
    if (CMainMenu::sharedMainMenu()) {
        CMainMenu::sharedMainMenu()->changeToSub(SUBMENU_HOME);
    }
}

void CMoneyTreeView::onBtnSwing(CCObject* pSender, CCControlEvent pEvent) {
    if (m_bIsSwinging) return;

    int remain = CMoneyTreeMgr::sharedManager()->getRemainTimes();
    if (remain <= 0) {
        CCLog("[CMoneyTreeView] Da het luot rung hom nay!");
        return;
    }

    m_bIsSwinging = true;
    RunSwingTreeAnim();
    CMoneyTreeMgr::sharedManager()->requestSwingTree(this, callfuncO_selector(CMoneyTreeView::onSwingSuccess));
}

void CMoneyTreeView::onTreeDataLoaded(CCObject* pData) {
    refreshView();
}

void CMoneyTreeView::onSwingSuccess(CCObject* pData) {
    m_bIsSwinging = false;
    refreshView();

    const SMoneyTreeUser& user = CMoneyTreeMgr::sharedManager()->getUser();
    if (user.latestSilver > 0) {
        // Hiển thị hiệu ứng chữ bay cộng Bạc
        CCLabelTTF* pFloatText = CCLabelTTF::create(CCString::createWithFormat("+%d Bạc!", user.latestSilver)->getCString(), "Arial-BoldMT", 28);
        if (pFloatText) {
            pFloatText->setColor(ccc3(255, 230, 0));
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            pFloatText->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 50));
            this->addChild(pFloatText, 100);

            CCActionInterval* moveUp = CCMoveBy::create(1.0f, ccp(0, 80));
            CCActionInterval* fadeOut = CCFadeOut::create(1.0f);
            CCSpawn* spawn = CCSpawn::create(moveUp, fadeOut, NULL);
            CCCallFunc* remove = CCCallFunc::create(pFloatText, callfunc_selector(CCNode::removeFromParent));
            pFloatText->runAction(CCSequence::create(spawn, remove, NULL));
        }
    }
}

void CMoneyTreeView::didAccelerate(CCAcceleration* pAccelerationValue) {
    if (m_bIsSwinging) return;

    // Tính độ rung lắc thiết bị
    double force = sqrt(pAccelerationValue->x * pAccelerationValue->x +
                        pAccelerationValue->y * pAccelerationValue->y +
                        pAccelerationValue->z * pAccelerationValue->z);

    if (force > 1.8) {
        struct timeval now;
        gettimeofday(&now, NULL);
        double curTime = (double)now.tv_sec + (double)now.tv_usec / 1000000.0;

        if (curTime - m_lastShakeTime > 1.5) {
            m_lastShakeTime = curTime;
            CCLog("[CMoneyTreeView] Phat hien rung lac vat ly! Tu dong Rung Cay Tien!");
            onBtnSwing(NULL, CCControlEventTouchUpInside);
        }
    }
}

SEL_MenuHandler CMoneyTreeView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CMoneyTreeView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    if (strcmp(pSelectorName, "onBtnBack") == 0) {
        return cccontrol_selector(CMoneyTreeView::onBtnBack);
    }
    if (strcmp(pSelectorName, "onBtnSwing") == 0) {
        return cccontrol_selector(CMoneyTreeView::onBtnSwing);
    }
    return NULL;
}

bool CMoneyTreeView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_width", CCNode*, this->m_pNodeWidth);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_tips", CCNode*, this->m_pNodeTips);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_anim", CCNode*, this->m_pNodeAnim);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_content", CCNode*, this->m_pNodeContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cell", CCNode*, this->m_pNodeCell);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_goldval", CCLabelTTF*, this->m_pLabelGoldVal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_silverval", CCLabelTTF*, this->m_pLabelSilverVal);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_times", CCLabelTTF*, this->m_pLabelTimes);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_freetry", CCLabelTTF*, this->m_pLabelFreeTry);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_try", CCLabelTTF*, this->m_pLabelTry);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_getsilver", CCLabelTTF*, this->m_pLabelGetSilver);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_costgold", CCLabelTTF*, this->m_pLabelCostGold);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_desc", CCLabelTTF*, this->m_pLabelDesc);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_MoneyTreeView0", CCLabelTTF*, this->m_pLabelMoneyTreeView0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_MoneyTreeView1", CCLabelTTF*, this->m_pLabelMoneyTreeView1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_MoneyTreeView2", CCLabelTTF*, this->m_pLabelMoneyTreeView2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_MoneyTreeView3", CCLabelTTF*, this->m_pLabelMoneyTreeView3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_MoneyTreeView4", CCLabelTTF*, this->m_pLabelMoneyTreeView4);

    return false;
}
