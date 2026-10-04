#include "CRoundTeamFightView.h"
#include "CRoundResultView.h"
#include "CSubChapterView.h"
#include "CActiveTeamMgr.h"
#include "CNinjaTableMgr.h"
#include <sstream>

CRoundTeamFightView::CRoundTeamFightView()
    : m_chapterId(1)
    , m_stageId(1)
    , m_roundId(1)
    , m_isBoss(false)
    , m_pLabelEnemyName(NULL)
    , m_pLabelEnemyTeamCount(NULL)
    , m_pLabelMyTeamCount(NULL)
    , m_pNodeMyCardList(NULL)
    , m_pNodeEnemyCardList(NULL)
    , m_pBtnPass(NULL)
{
}

CRoundTeamFightView::~CRoundTeamFightView() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationBattleCompleted");
    CC_SAFE_RELEASE_NULL(m_pLabelEnemyName);
    CC_SAFE_RELEASE_NULL(m_pLabelEnemyTeamCount);
    CC_SAFE_RELEASE_NULL(m_pLabelMyTeamCount);
    CC_SAFE_RELEASE_NULL(m_pNodeMyCardList);
    CC_SAFE_RELEASE_NULL(m_pNodeEnemyCardList);
    CC_SAFE_RELEASE_NULL(m_pBtnPass);
}

CRoundTeamFightView* CRoundTeamFightView::createWithRound(int chapterId, int stageId, int roundId) {
    CRoundTeamFightView* pRet = new CRoundTeamFightView();
    if (pRet && pRet->initWithRound(chapterId, stageId, roundId)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CRoundTeamFightView::initWithRound(int chapterId, int stageId, int roundId) {
    if (!CCLayer::init()) return false;

    m_chapterId = chapterId;
    m_stageId = stageId;
    m_roundId = roundId;

    // Nạp giao diện RoundTeamView.ccbi
    CCNode* pRoot = CCBManager::sharedManager()->loadNodeFromCCBI("RoundTeamView.ccbi", this, this);
    if (pRoot) {
        this->addChild(pRoot);
    } else {
        CCLog("[CRoundTeamFightView] Canh bao: Khong the tai RoundTeamView.ccbi");
    }

    const RoundTableEntry* pRound = CStageTableMgr::sharedManager()->getRoundEntry(m_roundId);
    if (pRound) {
        m_isBoss = (pRound->isBoss == 1);
        if (m_pLabelEnemyName) {
            std::string enemyTitle = pRound->name;
            if (m_isBoss) enemyTitle += " [BOSS]";
            m_pLabelEnemyName->setString(enemyTitle.c_str());
        }
        setupEnemyTeamDisplay(*pRound);
    }

    setupMyTeamDisplay();
    return true;
}

void CRoundTeamFightView::onEnter() {
    CCLayer::onEnter();
}

void CRoundTeamFightView::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationBattleCompleted");
    CCLayer::onExit();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO RoundTeamView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CRoundTeamFightView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CRoundTeamFightView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnPass", CRoundTeamFightView::onFightClicked);
    return NULL;
}

bool CRoundTeamFightView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_enemyname", CCLabelTTF*, this->m_pLabelEnemyName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_enemyteamcount", CCLabelTTF*, this->m_pLabelEnemyTeamCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_myteamcount", CCLabelTTF*, this->m_pLabelMyTeamCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_mycardlist", CCNode*, this->m_pNodeMyCardList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_enemycardlist", CCNode*, this->m_pNodeEnemyCardList);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnPass", CCControlButton*, this->m_pBtnPass);
    return false;
}

void CRoundTeamFightView::setupMyTeamDisplay() {
    if (!m_pNodeMyCardList) return;
    m_pNodeMyCardList->removeAllChildrenWithCleanup(true);

    CActiveTeamMgr* pTeamMgr = CActiveTeamMgr::sharedManager();
    int activeCount = 0;

    for (int slot = 1; slot <= 6; ++slot) {
        CTeamCard* card = pTeamMgr ? pTeamMgr->getTeamCard(slot) : NULL;
        if (card && card->getCardType() > 0) {
            activeCount++;

            // Thẻ avatar ninja của đội người chơi
            const NinjaTableEntry* pNinja = CNinjaTableMgr::sharedManager()->getNinjaEntry(card->getCardId());
            std::string ninjaName = pNinja ? pNinja->name : "Ninja";

            CCNode* cardNode = CCNode::create();
            cardNode->setContentSize(CCSizeMake(80.0f, 100.0f));
            cardNode->setPosition(ccp((slot - 1) * 85.0f, 0.0f));

            CCLabelTTF* pLbl = CCLabelTTF::create(ninjaName.c_str(), "Helvetica-Bold", 12.0f);
            pLbl->setPosition(ccp(40.0f, 15.0f));
            pLbl->setColor(ccc3(255, 235, 120));
            cardNode->addChild(pLbl);

            m_pNodeMyCardList->addChild(cardNode);
        }
    }

    if (m_pLabelMyTeamCount) {
        std::stringstream ss;
        ss << activeCount << "/6";
        m_pLabelMyTeamCount->setString(ss.str().c_str());
    }
}

void CRoundTeamFightView::setupEnemyTeamDisplay(const RoundTableEntry& round) {
    if (!m_pNodeEnemyCardList) return;
    m_pNodeEnemyCardList->removeAllChildrenWithCleanup(true);

    // Hiển thị đội hình quân địch
    int enemyCount = round.isBoss ? 1 : 3;
    for (int i = 0; i < enemyCount; ++i) {
        CCNode* enemyNode = CCNode::create();
        enemyNode->setContentSize(CCSizeMake(80.0f, 100.0f));
        enemyNode->setPosition(ccp(i * 85.0f, 0.0f));

        std::string eName = round.isBoss ? "Boss Ninja" : "Lính Ninja";
        CCLabelTTF* pLbl = CCLabelTTF::create(eName.c_str(), "Helvetica-Bold", 12.0f);
        pLbl->setPosition(ccp(40.0f, 15.0f));
        pLbl->setColor(ccc3(239, 68, 68)); // Đỏ
        enemyNode->addChild(pLbl);

        m_pNodeEnemyCardList->addChild(enemyNode);
    }

    if (m_pLabelEnemyTeamCount) {
        std::stringstream ss;
        ss << enemyCount << "/6";
        m_pLabelEnemyTeamCount->setString(ss.str().c_str());
    }
}

void CRoundTeamFightView::onFightClicked(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CRoundTeamFightView] Bat dau tran chien PVE: Chap=%d, Stage=%d, Round=%d",
          m_chapterId, m_stageId, m_roundId);

    // Đăng ký nhận thông báo trận đấu hoàn tất từ CChapterMgr
    CCNotificationCenter::sharedNotificationCenter()->addObserver(
        this,
        callfuncO_selector(CRoundTeamFightView::onBattleNotificationReceived),
        "kNotificationBattleCompleted",
        NULL
    );

    // Gửi lệnh vượt ải lên Server
    CChapterMgr::sharedManager()->requestBattle(m_chapterId, m_stageId, m_roundId, m_isBoss);
}

void CRoundTeamFightView::onBattleNotificationReceived(CCObject* pObj) {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "kNotificationBattleCompleted");

    PVEFightResult result = CChapterMgr::sharedManager()->getLastFightResult();
    const RoundTableEntry* pRound = CStageTableMgr::sharedManager()->getRoundEntry(m_roundId);
    if (pRound) {
        result.enemyName = pRound->name;
    }

    // Chuyển sang màn hình hiển thị kết quả chiến thắng / thất bại
    CRoundResultView* pResultView = CRoundResultView::createWithResult(result, m_chapterId, m_stageId);
    if (pResultView && this->getParent()) {
        CCNode* pParent = this->getParent();
        pParent->addChild(pResultView, this->getZOrder() + 1);
        this->removeFromParentAndCleanup(true);
    }
}

void CRoundTeamFightView::onBackClicked(CCObject* pSender) {
    CCNode* pParent = this->getParent();
    if (pParent) {
        CSubChapterView* pSub = CSubChapterView::createWithChapter(m_chapterId);
        if (pSub) {
            pParent->addChild(pSub, this->getZOrder());
        }
        this->removeFromParentAndCleanup(true);
    }
}
