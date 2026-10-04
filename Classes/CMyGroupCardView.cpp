#include "CMyGroupCardView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include <sstream>

CMyGroupCardView::CMyGroupCardView()
    : m_curSlotIndex(0)
    , m_pDetailDialog(NULL)
    , m_pLayerNinja(NULL)
    , m_pSpriteNinjaIcon(NULL)
    , m_pSpriteNoNinja(NULL)
    , m_pSpriteNinjaCamp(NULL)
    , m_pLabelNinjaName(NULL)
    , m_pLabelLevel(NULL)
    , m_pLabelNinjaGroup(NULL)
    , m_pLabelAttack(NULL)
    , m_pLabelDefense(NULL)
    , m_pLabelChakra(NULL)
    , m_pSpriteStrengthIcon(NULL)
    , m_pSpriteStrengthFrame(NULL)
    , m_pLayerMoreCom(NULL)
    , m_pLabelMoreCom(NULL)
    , m_pLayerSuit(NULL)
{
    for (int i = 0; i < 6; ++i) m_pNodeIcons[i] = NULL;
    for (int i = 0; i < 5; ++i) m_pSpriteStars[i] = NULL;
    for (int i = 0; i < 4; ++i) {
        m_pSpriteEquipIcons[i] = NULL;
        m_pSpriteSkills[i] = NULL;
    }
    for (int i = 0; i < 8; ++i) m_pLabelEquips[i] = NULL;
}

CMyGroupCardView::~CMyGroupCardView() {
    for (int i = 0; i < 6; ++i) CC_SAFE_RELEASE_NULL(m_pNodeIcons[i]);
    CC_SAFE_RELEASE_NULL(m_pLayerNinja);
    CC_SAFE_RELEASE_NULL(m_pSpriteNinjaIcon);
    CC_SAFE_RELEASE_NULL(m_pSpriteNoNinja);
    CC_SAFE_RELEASE_NULL(m_pSpriteNinjaCamp);
    CC_SAFE_RELEASE_NULL(m_pLabelNinjaName);
    CC_SAFE_RELEASE_NULL(m_pLabelLevel);
    CC_SAFE_RELEASE_NULL(m_pLabelNinjaGroup);
    CC_SAFE_RELEASE_NULL(m_pLabelAttack);
    CC_SAFE_RELEASE_NULL(m_pLabelDefense);
    CC_SAFE_RELEASE_NULL(m_pLabelChakra);

    for (int i = 0; i < 5; ++i) CC_SAFE_RELEASE_NULL(m_pSpriteStars[i]);
    CC_SAFE_RELEASE_NULL(m_pSpriteStrengthIcon);
    CC_SAFE_RELEASE_NULL(m_pSpriteStrengthFrame);

    for (int i = 0; i < 4; ++i) {
        CC_SAFE_RELEASE_NULL(m_pSpriteEquipIcons[i]);
        CC_SAFE_RELEASE_NULL(m_pSpriteSkills[i]);
    }
    for (int i = 0; i < 8; ++i) CC_SAFE_RELEASE_NULL(m_pLabelEquips[i]);

    CC_SAFE_RELEASE_NULL(m_pLayerMoreCom);
    CC_SAFE_RELEASE_NULL(m_pLabelMoreCom);
    CC_SAFE_RELEASE_NULL(m_pLayerSuit);
}

CCScene* CMyGroupCardView::scene() {
    CCScene* pScene = CCScene::create();
    CMyGroupCardView* pLayer = CMyGroupCardView::create();
    if (pLayer) {
        pScene->addChild(pLayer);
    }
    return pScene;
}

CMyGroupCardView* CMyGroupCardView::create() {
    CMyGroupCardView* pRet = new CMyGroupCardView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CMyGroupCardView::init() {
    if (!CCLayer::init()) {
        return false;
    }

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Nạp từ TeamNinjaView.ccbi
    CCNode* pCcbNode = CCBManager::sharedManager()->loadNodeFromCCBI("sub_ui/TeamNinjaView.ccbi", this);
    if (!pCcbNode) {
        pCcbNode = CCBManager::sharedManager()->loadNodeFromCCBI("TeamNinjaView.ccbi", this);
    }

    if (pCcbNode) {
        pCcbNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
        this->addChild(pCcbNode);
    } else {
        // Fallback Native UI chuẩn 768x960
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg, 0);

        // Header Tiêu đề
        CCLabelTTF* pTitle = CCLabelTTF::create("ĐỘI HÌNH NHẪN GIẢ", "Helvetica-Bold", 32.0f);
        pTitle->setPosition(ccp(winSize.width / 2.0f, winSize.height - 110.0f));
        pTitle->setColor(ccc3(250, 204, 21));
        this->addChild(pTitle, 2);

        // Container các ô đội hình 6 slot (ngang trên cùng)
        float slotStartX = 100.0f;
        float slotGapX = 110.0f;
        float slotY = winSize.height - 180.0f;

        for (int i = 0; i < 6; ++i) {
            CCNode* slotNode = CCNode::create();
            slotNode->setPosition(ccp(slotStartX + i * slotGapX, slotY));
            this->addChild(slotNode, 5);
            m_pNodeIcons[i] = slotNode;
            m_pNodeIcons[i]->retain();

            // Khung slot
            CCLayerColor* pSlotBg = CCLayerColor::create(ccc4(30, 41, 59, 200), 80, 80);
            pSlotBg->ignoreAnchorPointForPosition(false);
            pSlotBg->setAnchorPoint(ccp(0.5f, 0.5f));
            slotNode->addChild(pSlotBg);

            std::stringstream ssIdx;
            ssIdx << (i + 1);
            CCLabelTTF* pNum = CCLabelTTF::create(ssIdx.str().c_str(), "Helvetica-Bold", 24.0f);
            pNum->setPosition(ccp(0.0f, 0.0f));
            pNum->setColor(ccc3(148, 163, 184));
            slotNode->addChild(pNum, 2);
        }

        // Khung hiển thị Tướng chính giữa
        m_pLayerNinja = CCNode::create();
        m_pLayerNinja->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f + 20.0f));
        m_pLayerNinja->retain();
        this->addChild(m_pLayerNinja, 5);

        // Ảnh thẻ tướng
        m_pSpriteNinjaIcon = CCSprite::create("0V.png");
        m_pSpriteNinjaIcon->setPosition(ccp(0.0f, 60.0f));
        m_pSpriteNinjaIcon->setScale(0.85f);
        m_pSpriteNinjaIcon->retain();
        m_pLayerNinja->addChild(m_pSpriteNinjaIcon);

        m_pSpriteNoNinja = CCSprite::create();
        m_pSpriteNoNinja->retain();
        m_pLayerNinja->addChild(m_pSpriteNoNinja);

        // Tên Tướng
        m_pLabelNinjaName = CCLabelTTF::create("Chưa Xuất Trận", "Helvetica-Bold", 28.0f);
        m_pLabelNinjaName->setPosition(ccp(0.0f, -130.0f));
        m_pLabelNinjaName->setColor(ccc3(250, 204, 21));
        m_pLabelNinjaName->retain();
        m_pLayerNinja->addChild(m_pLabelNinjaName);

        // Cấp & Vị trí
        m_pLabelLevel = CCLabelTTF::create("Lv.1", "Helvetica-Bold", 22.0f);
        m_pLabelLevel->setPosition(ccp(-120.0f, -170.0f));
        m_pLabelLevel->setColor(ccc3(255, 255, 255));
        m_pLabelLevel->retain();
        m_pLayerNinja->addChild(m_pLabelLevel);

        m_pLabelNinjaGroup = CCLabelTTF::create("Vị Trí 1 (Tiên Phong)", "Helvetica", 20.0f);
        m_pLabelNinjaGroup->setPosition(ccp(100.0f, -170.0f));
        m_pLabelNinjaGroup->setColor(ccc3(148, 163, 184));
        m_pLabelNinjaGroup->retain();
        m_pLayerNinja->addChild(m_pLabelNinjaGroup);

        // 3 Chỉ số chính (Công, Thủ, Chakra)
        m_pLabelAttack = CCLabelTTF::create("Công: 0 - 0", "Helvetica-Bold", 20.0f);
        m_pLabelAttack->setPosition(ccp(-160.0f, -210.0f));
        m_pLabelAttack->setColor(ccc3(239, 68, 68));
        m_pLabelAttack->retain();
        m_pLayerNinja->addChild(m_pLabelAttack);

        m_pLabelDefense = CCLabelTTF::create("Thủ: 0 - 0", "Helvetica-Bold", 20.0f);
        m_pLabelDefense->setPosition(ccp(0.0f, -210.0f));
        m_pLabelDefense->setColor(ccc3(59, 130, 246));
        m_pLabelDefense->retain();
        m_pLayerNinja->addChild(m_pLabelDefense);

        m_pLabelChakra = CCLabelTTF::create("Chakra: 0 - 0", "Helvetica-Bold", 20.0f);
        m_pLabelChakra->setPosition(ccp(160.0f, -210.0f));
        m_pLabelChakra->setColor(ccc3(168, 85, 247));
        m_pLabelChakra->retain();
        m_pLayerNinja->addChild(m_pLabelChakra);

        // Duyên phận (More Com)
        m_pLayerMoreCom = CCNode::create();
        m_pLayerMoreCom->setPosition(ccp(0.0f, -250.0f));
        m_pLayerMoreCom->retain();
        m_pLayerNinja->addChild(m_pLayerMoreCom);

        m_pLabelMoreCom = CCLabelTTF::create("Duyên Phận: 0/6 kích hoạt", "Helvetica", 18.0f);
        m_pLabelMoreCom->setPosition(ccp(0.0f, 0.0f));
        m_pLabelMoreCom->setColor(ccc3(52, 211, 153));
        m_pLabelMoreCom->retain();
        m_pLayerMoreCom->addChild(m_pLabelMoreCom);
    }

    return true;
}

void CMyGroupCardView::onEnter() {
    CCLayer::onEnter();
    InitUI();
}

void CMyGroupCardView::onExit() {
    CCLayer::onExit();
}

void CMyGroupCardView::InitUI() {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    CActiveTeamMgr* pTeam = pData->getActiveTeam();
    if (!pTeam) return;

    int maxUnlocked = pData->firefly_GetMaxTeamMembers();

    // 1. Cập nhật 6 nút vị trí đội hình (node_icon1 -> node_icon6)
    for (int i = 0; i < 6; ++i) {
        CCNode* pSlotNode = m_pNodeIcons[i];
        if (!pSlotNode) continue;

        bool isUnlocked = (i < maxUnlocked);
        CTeamCard* pCard = pTeam->firefly_GetTeamCardByIndex(i);
        bool hasNinja = (pCard && pCard->firefly_HasNinja());

        // Đổi màu / hiệu ứng cho slot được chọn
        if (i == m_curSlotIndex) {
            pSlotNode->setScale(1.15f);
        } else {
            pSlotNode->setScale(1.0f);
        }
    }

    // 2. Cập nhật thông tin thẻ Tướng tại vị trí đang chọn (m_curSlotIndex)
    CTeamCard* pCurCard = pTeam->firefly_GetTeamCardByIndex(m_curSlotIndex);
    bool hasNinja = (pCurCard && pCurCard->firefly_HasNinja());

    if (hasNinja && pCurCard->firefly_GetNinja()) {
        CPlayerNinja* pNinja = pCurCard->firefly_GetNinja();

        if (m_pSpriteNinjaIcon) {
            m_pSpriteNinjaIcon->setVisible(true);
            std::string portrait = pNinja->getPortraitPath();
            CCTexture2D* pTex = CCTextureCache::sharedTextureCache()->addImage(portrait.c_str());
            if (pTex) {
                m_pSpriteNinjaIcon->setTexture(pTex);
                m_pSpriteNinjaIcon->setTextureRect(CCRectMake(0, 0, pTex->getContentSize().width, pTex->getContentSize().height));
            }
        }

        if (m_pSpriteNoNinja) {
            m_pSpriteNoNinja->setVisible(false);
        }

        if (m_pLabelNinjaName) {
            std::stringstream ss;
            ss << pNinja->firefly_GetName();
            if (pNinja->firefly_GetStrengthLevel() > 0) {
                ss << " [+" << pNinja->firefly_GetStrengthLevel() << "]";
            }
            m_pLabelNinjaName->setString(ss.str().c_str());
        }

        if (m_pLabelLevel) {
            std::stringstream ss;
            ss << "Lv." << pNinja->firefly_GetLevel();
            m_pLabelLevel->setString(ss.str().c_str());
        }

        if (m_pLabelNinjaGroup) {
            std::stringstream ss;
            ss << "Vị Trí " << (m_curSlotIndex + 1);
            if (m_curSlotIndex == 0) ss << " (Đội Trưởng)";
            m_pLabelNinjaGroup->setString(ss.str().c_str());
        }

        if (m_pLabelAttack) {
            std::stringstream ss;
            ss << "Công: " << pCurCard->firefly_GetAttackLow() << " - " << pCurCard->firefly_GetAttackHigh();
            m_pLabelAttack->setString(ss.str().c_str());
        }

        if (m_pLabelDefense) {
            std::stringstream ss;
            ss << "Thủ: " << pCurCard->firefly_GetDefenseLow() << " - " << pCurCard->firefly_GetDefenseHigh();
            m_pLabelDefense->setString(ss.str().c_str());
        }

        if (m_pLabelChakra) {
            std::stringstream ss;
            ss << "Chakra: " << pCurCard->firefly_GetChakraLow() << " - " << pCurCard->firefly_GetChakraHigh();
            m_pLabelChakra->setString(ss.str().c_str());
        }

        if (m_pLabelMoreCom) {
            std::stringstream ss;
            ss << "Duyên Phận: " << pCurCard->firefly_GetCombinCount() << " kích hoạt";
            m_pLabelMoreCom->setString(ss.str().c_str());
        }

        // Cập nhật số sao
        int stars = pNinja->firefly_GetQuality();
        for (int s = 0; s < 5; ++s) {
            if (m_pSpriteStars[s]) {
                m_pSpriteStars[s]->setVisible(s < stars);
            }
        }
    } else {
        // Ô vị trí trống
        if (m_pSpriteNinjaIcon) m_pSpriteNinjaIcon->setVisible(false);
        if (m_pSpriteNoNinja) m_pSpriteNoNinja->setVisible(true);

        if (m_pLabelNinjaName) m_pLabelNinjaName->setString("Chưa Xuất Trận");
        if (m_pLabelLevel) m_pLabelLevel->setString("Lv.0");

        if (m_pLabelNinjaGroup) {
            std::stringstream ss;
            ss << "Vị Trí " << (m_curSlotIndex + 1) << " (Trống)";
            m_pLabelNinjaGroup->setString(ss.str().c_str());
        }

        if (m_pLabelAttack) m_pLabelAttack->setString("Công: 0 - 0");
        if (m_pLabelDefense) m_pLabelDefense->setString("Thủ: 0 - 0");
        if (m_pLabelChakra) m_pLabelChakra->setString("Chakra: 0 - 0");
        if (m_pLabelMoreCom) m_pLabelMoreCom->setString("Chưa có Nhẫn Giả");

        for (int s = 0; s < 5; ++s) {
            if (m_pSpriteStars[s]) m_pSpriteStars[s]->setVisible(false);
        }
    }
}

void CMyGroupCardView::selectSlot(int slotIndex) {
    if (slotIndex >= 0 && slotIndex < 6) {
        m_curSlotIndex = slotIndex;
        InitUI();
    }
}

void CMyGroupCardView::showNinjaDetail() {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    CActiveTeamMgr* pTeam = pData->getActiveTeam();
    if (!pTeam) return;

    CTeamCard* pCard = pTeam->firefly_GetTeamCardByIndex(m_curSlotIndex);
    if (pCard && pCard->firefly_HasNinja()) {
        CNinjaDetailView* pDetail = CNinjaDetailView::createWithNinja(pCard->firefly_GetNinja(), this);
        if (pDetail) {
            pDetail->Show(this->getParent() ? this->getParent() : this, 50);
        }
    }
}

void CMyGroupCardView::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool CMyGroupCardView::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
    return true;
}

void CMyGroupCardView::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
    CCPoint touchLoc = pTouch->getLocation();

    // 1. Kiểm tra bấm vào 6 ô slot đội hình
    for (int i = 0; i < 6; ++i) {
        if (m_pNodeIcons[i]) {
            CCPoint nodePos = m_pNodeIcons[i]->getPosition();
            if (m_pNodeIcons[i]->getParent() && m_pNodeIcons[i]->getParent() != this) {
                nodePos = m_pNodeIcons[i]->getParent()->convertToWorldSpace(m_pNodeIcons[i]->getPosition());
            }
            CCRect rect = CCRectMake(nodePos.x - 45, nodePos.y - 45, 90, 90);
            if (rect.containsPoint(touchLoc)) {
                selectSlot(i);
                return;
            }
        }
    }

    // 2. Bấm vào khung Tướng chính giữa để xem Chi tiết Nhẫn Giả
    if (m_pLayerNinja) {
        CCPoint layerPos = m_pLayerNinja->getPosition();
        if (m_pLayerNinja->getParent() && m_pLayerNinja->getParent() != this) {
            layerPos = m_pLayerNinja->getParent()->convertToWorldSpace(m_pLayerNinja->getPosition());
        }
        CCRect cardRect = CCRectMake(layerPos.x - 180, layerPos.y - 150, 360, 300);
        if (cardRect.containsPoint(touchLoc)) {
            showNinjaDetail();
            return;
        }
    }
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO TeamNinjaView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CMyGroupCardView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CMyGroupCardView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

bool CMyGroupCardView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon1", CCNode*, m_pNodeIcons[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon2", CCNode*, m_pNodeIcons[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon3", CCNode*, m_pNodeIcons[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon4", CCNode*, m_pNodeIcons[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon5", CCNode*, m_pNodeIcons[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_icon6", CCNode*, m_pNodeIcons[5]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_ninja", CCNode*, m_pLayerNinja);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_ninjaicon", CCSprite*, m_pSpriteNinjaIcon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_noninja", CCSprite*, m_pSpriteNoNinja);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_ninjacamp", CCSprite*, m_pSpriteNinjaCamp);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_ninjaname", CCLabelTTF*, m_pLabelNinjaName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_level", CCLabelTTF*, m_pLabelLevel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_ninjagroup", CCLabelTTF*, m_pLabelNinjaGroup);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_ninjaattack", CCLabelTTF*, m_pLabelAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_ninjadefense", CCLabelTTF*, m_pLabelDefense);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_ninjacharkra", CCLabelTTF*, m_pLabelChakra);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_star01", CCSprite*, m_pSpriteStars[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_star02", CCSprite*, m_pSpriteStars[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_star03", CCSprite*, m_pSpriteStars[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_star04", CCSprite*, m_pSpriteStars[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "sprite_star05", CCSprite*, m_pSpriteStars[4]);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_morecom", CCNode*, m_pLayerMoreCom);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "label_morecom", CCLabelTTF*, m_pLabelMoreCom);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "layer_suit", CCNode*, m_pLayerSuit);

    return false;
}

// -------------------------------------------------------------
// DELEGATE CALLBACKS
// -------------------------------------------------------------
void CMyGroupCardView::onNinjaDetailClose(CNinjaDetailView* pView) {
    InitUI();
}

void CMyGroupCardView::onNinjaDetailChange(CNinjaDetailView* pView) {
    CCLog("[CMyGroupCardView] Chuyen sang che do Chon Tuong Thay The cho O %d", m_curSlotIndex + 1);
}

void CMyGroupCardView::onNinjaDetailUpgrade(CNinjaDetailView* pView) {
    CCLog("[CMyGroupCardView] Chuyen sang giao dien Cuong Hoa Tuong");
}

void CMyGroupCardView::requestChangeFormation(int slotSeq, int newNinjaSeq, int oldNinjaSeq) {
    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    CRLRequest* pReq = CRLRequest::create();
    pReq->setDelegate(this);
    pReq->setCmd(2300);
    pReq->addParam("seq", slotSeq);
    pReq->addParam("newid", newNinjaSeq);
    pReq->addParam("oldid", oldNinjaSeq);
    pReq->addParam("teamid", pData->getActiveTeam()->getActiveTeamIndex());
    pReq->addParam("ServerID", pData->getServerId());
    pReq->sendPost("/rl_w_ninjalist");
}

void CMyGroupCardView::onHttpRequestCompleted(CRLRequest* pRequest) {
    if (!pRequest || pRequest->getState() != CRLRequest::REQ_SUCCESS) return;

    std::string resp = pRequest->getResponseData();
    CCLog("[CMyGroupCardView] Server tra ve doi hinh moi: %s", resp.substr(0, 150).c_str());

    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (pData) {
        pData->parseLoginXml(resp);
        InitUI();
    }
}
