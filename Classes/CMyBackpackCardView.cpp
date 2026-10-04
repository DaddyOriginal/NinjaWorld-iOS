#include "CMyBackpackCardView.h"
#include "CCBManager.h"
#include "CPlayerDataMgr.h"
#include "CItemTableMgr.h"
#include <sstream>

CMyBackpackCardView::CMyBackpackCardView()
    : m_currentMode(MODE_ITEMS)
    , m_currentSubTab(TAB_WEAPON)
    , m_selectedItemIndex(0)
    , m_pNodeBackpack(NULL)
    , m_pNodePieceBackpack(NULL)
    , m_pNodeTableContent(NULL)
    , m_pNodeCardContent(NULL)
    , m_pBtnBackPack(NULL)
    , m_pBtnPieceBackPack(NULL)
    , m_pBtnNinja(NULL)
    , m_pBtnWeapon(NULL)
    , m_pBtnArmors(NULL)
    , m_pBtnAccessori(NULL)
    , m_pBtnMarks(NULL)
    , m_pBtnNinjutsu(NULL)
    , m_pBtnNinjaPiece(NULL)
    , m_pBtnEquipPiece(NULL)
    , m_pBtnNinjutsuPiece(NULL)
    , m_pBtnPetPiece(NULL)
    , m_pScrollView(NULL)
    , m_pGridContainer(NULL)
    , m_pLabelPreviewName(NULL)
    , m_pLabelPreviewLevel(NULL)
    , m_pLabelPreviewStats(NULL)
    , m_pLabelPreviewDesc(NULL)
    , m_pSpritePreviewIcon(NULL)
    , m_pBtnAction(NULL)
{
}

CMyBackpackCardView::~CMyBackpackCardView() {
    CC_SAFE_RELEASE_NULL(m_pNodeBackpack);
    CC_SAFE_RELEASE_NULL(m_pNodePieceBackpack);
    CC_SAFE_RELEASE_NULL(m_pNodeTableContent);
    CC_SAFE_RELEASE_NULL(m_pNodeCardContent);

    CC_SAFE_RELEASE_NULL(m_pBtnBackPack);
    CC_SAFE_RELEASE_NULL(m_pBtnPieceBackPack);

    CC_SAFE_RELEASE_NULL(m_pBtnNinja);
    CC_SAFE_RELEASE_NULL(m_pBtnWeapon);
    CC_SAFE_RELEASE_NULL(m_pBtnArmors);
    CC_SAFE_RELEASE_NULL(m_pBtnAccessori);
    CC_SAFE_RELEASE_NULL(m_pBtnMarks);
    CC_SAFE_RELEASE_NULL(m_pBtnNinjutsu);

    CC_SAFE_RELEASE_NULL(m_pBtnNinjaPiece);
    CC_SAFE_RELEASE_NULL(m_pBtnEquipPiece);
    CC_SAFE_RELEASE_NULL(m_pBtnNinjutsuPiece);
    CC_SAFE_RELEASE_NULL(m_pBtnPetPiece);

    CC_SAFE_RELEASE_NULL(m_pLabelPreviewName);
    CC_SAFE_RELEASE_NULL(m_pLabelPreviewLevel);
    CC_SAFE_RELEASE_NULL(m_pLabelPreviewStats);
    CC_SAFE_RELEASE_NULL(m_pLabelPreviewDesc);
    CC_SAFE_RELEASE_NULL(m_pSpritePreviewIcon);
    CC_SAFE_RELEASE_NULL(m_pBtnAction);
}

CCScene* CMyBackpackCardView::scene() {
    CCScene* pScene = CCScene::create();
    CMyBackpackCardView* pLayer = CMyBackpackCardView::create();
    if (pLayer) {
        pScene->addChild(pLayer);
    }
    return pScene;
}

CMyBackpackCardView* CMyBackpackCardView::create() {
    CMyBackpackCardView* pRet = new CMyBackpackCardView();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CMyBackpackCardView::init() {
    if (!CCLayer::init()) {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Thử nạp từ CCBI
    CCNode* pCcbNode = CCBManager::sharedManager()->loadNodeFromCCBI("backpack/MyBackpackView.ccbi", this);
    if (!pCcbNode) {
        pCcbNode = CCBManager::sharedManager()->loadNodeFromCCBI("MyBackpackView.ccbi", this);
    }

    if (pCcbNode) {
        pCcbNode->setPosition(ccp(winSize.width / 2.0f, winSize.height / 2.0f));
        this->addChild(pCcbNode);
    } else {
        // Fallback Native UI chuẩn 768x960
        CCLayerColor* pBg = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pBg, 0);

        // Header Tiêu đề
        CCLabelTTF* pTitle = CCLabelTTF::create("TÚI ĐỒ & TRANG BỊ", "Helvetica-Bold", 32.0f);
        pTitle->setPosition(ccp(winSize.width / 2.0f, winSize.height - 110.0f));
        pTitle->setColor(ccc3(250, 204, 21));
        this->addChild(pTitle, 2);

        // 2 Nút chuyển Mode chính (Túi Trang Bị & Túi Mảnh)
        m_pBtnBackPack = CCControlButton::create(
            CCLabelTTF::create("Túi Trang Bị", "Helvetica-Bold", 22.0f),
            CCScale9Sprite::create()
        );
        m_pBtnBackPack->setPreferredSize(CCSizeMake(180, 50));
        m_pBtnBackPack->setPosition(ccp(winSize.width / 2.0f - 100.0f, winSize.height - 165.0f));
        m_pBtnBackPack->addTargetWithActionForControlEvents(this, cccontrol_selector(CMyBackpackCardView::onBtnModeBackpack), CCControlEventTouchUpInside);
        m_pBtnBackPack->retain();
        this->addChild(m_pBtnBackPack, 5);

        m_pBtnPieceBackPack = CCControlButton::create(
            CCLabelTTF::create("Túi Mảnh Ghép", "Helvetica-Bold", 22.0f),
            CCScale9Sprite::create()
        );
        m_pBtnPieceBackPack->setPreferredSize(CCSizeMake(180, 50));
        m_pBtnPieceBackPack->setPosition(ccp(winSize.width / 2.0f + 100.0f, winSize.height - 165.0f));
        m_pBtnPieceBackPack->addTargetWithActionForControlEvents(this, cccontrol_selector(CMyBackpackCardView::onBtnModePieceBackpack), CCControlEventTouchUpInside);
        m_pBtnPieceBackPack->retain();
        this->addChild(m_pBtnPieceBackPack, 5);

        // Container túi trang bị (node_backpack)
        m_pNodeBackpack = CCNode::create();
        m_pNodeBackpack->setPosition(ccp(0.0f, 0.0f));
        m_pNodeBackpack->retain();
        this->addChild(m_pNodeBackpack, 5);

        // Các tab con của Túi trang bị: Tướng, Vũ khí, Giáp, Trang sức, Ấn ký
        const char* tabNames[] = { "Tướng", "Vũ Khí", "Áo Giáp", "Trang Sức", "Ấn Ký" };
        SEL_CCControlHandler tabHandlers[] = {
            cccontrol_selector(CMyBackpackCardView::onTabNinja),
            cccontrol_selector(CMyBackpackCardView::onTabWeapon),
            cccontrol_selector(CMyBackpackCardView::onTabArmors),
            cccontrol_selector(CMyBackpackCardView::onTabAccessori),
            cccontrol_selector(CMyBackpackCardView::onTabMarks)
        };

        float startTabX = 90.0f;
        float gapTabX = 145.0f;
        for (int i = 0; i < 5; ++i) {
            CCControlButton* tabBtn = CCControlButton::create(
                CCLabelTTF::create(tabNames[i], "Helvetica-Bold", 18.0f),
                CCScale9Sprite::create()
            );
            tabBtn->setPreferredSize(CCSizeMake(130, 42));
            tabBtn->setPosition(ccp(startTabX + i * gapTabX, winSize.height - 225.0f));
            tabBtn->addTargetWithActionForControlEvents(this, tabHandlers[i], CCControlEventTouchUpInside);
            m_pNodeBackpack->addChild(tabBtn);

            if (i == 0) m_pBtnNinja = tabBtn;
            else if (i == 1) m_pBtnWeapon = tabBtn;
            else if (i == 2) m_pBtnArmors = tabBtn;
            else if (i == 3) m_pBtnAccessori = tabBtn;
            else if (i == 4) m_pBtnMarks = tabBtn;
            if (tabBtn) tabBtn->retain();
        }

        // Container túi mảnh (node_piecebackpack)
        m_pNodePieceBackpack = CCNode::create();
        m_pNodePieceBackpack->setPosition(ccp(0.0f, 0.0f));
        m_pNodePieceBackpack->setVisible(false);
        m_pNodePieceBackpack->retain();
        this->addChild(m_pNodePieceBackpack, 5);

        // Các tab con của Túi mảnh: Mảnh Tướng, Mảnh Trang Bị, Mảnh Bí Kíp, Mảnh Pet
        const char* pieceTabNames[] = { "Mảnh Tướng", "Mảnh Trang Bị", "Mảnh Bí Kíp", "Mảnh Linh Thú" };
        SEL_CCControlHandler pieceTabHandlers[] = {
            cccontrol_selector(CMyBackpackCardView::onTabNinjaPiece),
            cccontrol_selector(CMyBackpackCardView::onTabEquipPiece),
            cccontrol_selector(CMyBackpackCardView::onTabNinjutsuPiece),
            cccontrol_selector(CMyBackpackCardView::onTabPetPiece)
        };

        float startPieceX = 110.0f;
        float gapPieceX = 180.0f;
        for (int i = 0; i < 4; ++i) {
            CCControlButton* pBtn = CCControlButton::create(
                CCLabelTTF::create(pieceTabNames[i], "Helvetica-Bold", 18.0f),
                CCScale9Sprite::create()
            );
            pBtn->setPreferredSize(CCSizeMake(165, 42));
            pBtn->setPosition(ccp(startPieceX + i * gapPieceX, winSize.height - 225.0f));
            pBtn->addTargetWithActionForControlEvents(this, pieceTabHandlers[i], CCControlEventTouchUpInside);
            m_pNodePieceBackpack->addChild(pBtn);

            if (i == 0) m_pBtnNinjaPiece = pBtn;
            else if (i == 1) m_pBtnEquipPiece = pBtn;
            else if (i == 2) m_pBtnNinjutsuPiece = pBtn;
            else if (i == 3) m_pBtnPetPiece = pBtn;
            if (pBtn) pBtn->retain();
        }

        // Vùng danh sách vật phẩm (node_tablecontent)
        m_pNodeTableContent = CCNode::create();
        m_pNodeTableContent->setPosition(ccp(40.0f, 120.0f));
        m_pNodeTableContent->setContentSize(CCSizeMake(420, 560));
        m_pNodeTableContent->retain();
        this->addChild(m_pNodeTableContent, 5);

        // Khung xem trước chi tiết (node_cardcontent)
        m_pNodeCardContent = CCNode::create();
        m_pNodeCardContent->setPosition(ccp(480.0f, 120.0f));
        m_pNodeCardContent->setContentSize(CCSizeMake(250, 560));
        m_pNodeCardContent->retain();
        this->addChild(m_pNodeCardContent, 5);
    }

    // Thiết lập khung Preview trong node_cardcontent
    if (m_pNodeCardContent) {
        CCLayerColor* pCardBg = CCLayerColor::create(ccc4(30, 41, 59, 220), 250, 560);
        m_pNodeCardContent->addChild(pCardBg);

        m_pSpritePreviewIcon = CCSprite::create("0V.png");
        m_pSpritePreviewIcon->setPosition(ccp(125.0f, 440.0f));
        m_pSpritePreviewIcon->setScale(0.7f);
        m_pSpritePreviewIcon->retain();
        m_pNodeCardContent->addChild(m_pSpritePreviewIcon);

        m_pLabelPreviewName = CCLabelTTF::create("Chưa Chọn", "Helvetica-Bold", 20.0f);
        m_pLabelPreviewName->setPosition(ccp(125.0f, 320.0f));
        m_pLabelPreviewName->setColor(ccc3(250, 204, 21));
        m_pLabelPreviewName->retain();
        m_pNodeCardContent->addChild(m_pLabelPreviewName);

        m_pLabelPreviewLevel = CCLabelTTF::create("Lv.1", "Helvetica", 18.0f);
        m_pLabelPreviewLevel->setPosition(ccp(125.0f, 280.0f));
        m_pLabelPreviewLevel->setColor(ccc3(255, 255, 255));
        m_pLabelPreviewLevel->retain();
        m_pNodeCardContent->addChild(m_pLabelPreviewLevel);

        m_pLabelPreviewStats = CCLabelTTF::create("Công: 0\nThủ: 0\nChakra: 0", "Helvetica", 18.0f, CCSizeMake(230, 80), kCCTextAlignmentCenter);
        m_pLabelPreviewStats->setPosition(ccp(125.0f, 210.0f));
        m_pLabelPreviewStats->setColor(ccc3(52, 211, 153));
        m_pLabelPreviewStats->retain();
        m_pNodeCardContent->addChild(m_pLabelPreviewStats);

        m_pLabelPreviewDesc = CCLabelTTF::create("", "Helvetica", 16.0f, CCSizeMake(230, 100), kCCTextAlignmentLeft);
        m_pLabelPreviewDesc->setPosition(ccp(125.0f, 110.0f));
        m_pLabelPreviewDesc->setColor(ccc3(209, 213, 219));
        m_pLabelPreviewDesc->retain();
        m_pNodeCardContent->addChild(m_pLabelPreviewDesc);

        m_pBtnAction = CCControlButton::create(
            CCLabelTTF::create("Trang Bị", "Helvetica-Bold", 20.0f),
            CCScale9Sprite::create()
        );
        m_pBtnAction->setPreferredSize(CCSizeMake(180, 48));
        m_pBtnAction->setPosition(ccp(125.0f, 40.0f));
        m_pBtnAction->addTargetWithActionForControlEvents(this, cccontrol_selector(CMyBackpackCardView::onBtnActionClick), CCControlEventTouchUpInside);
        m_pBtnAction->retain();
        m_pNodeCardContent->addChild(m_pBtnAction);
    }

    return true;
}

void CMyBackpackCardView::onEnter() {
    CCLayer::onEnter();
    refreshItemList();
}

void CMyBackpackCardView::onExit() {
    CCLayer::onExit();
}

void CMyBackpackCardView::switchMode(BACKPACK_MODE mode) {
    m_currentMode = mode;
    m_selectedItemIndex = 0;

    if (m_pNodeBackpack) m_pNodeBackpack->setVisible(mode == MODE_ITEMS);
    if (m_pNodePieceBackpack) m_pNodePieceBackpack->setVisible(mode == MODE_PIECES);

    if (mode == MODE_ITEMS) {
        m_currentSubTab = TAB_WEAPON;
    } else {
        m_currentSubTab = TAB_PIECE_NINJA;
    }

    refreshItemList();
}

void CMyBackpackCardView::switchSubTab(BACKPACK_SUBTAB tab) {
    m_currentSubTab = tab;
    m_selectedItemIndex = 0;
    refreshItemList();
}

void CMyBackpackCardView::refreshItemList() {
    if (!m_pNodeTableContent) return;

    m_pNodeTableContent->removeAllChildrenWithCleanup(true);

    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    // Danh sách các item hiển thị
    int itemCount = 0;
    std::vector<std::string> itemNames;
    std::vector<std::string> itemIcons;
    std::vector<int> itemStars;
    std::vector<std::string> itemBadges;

    if (m_currentMode == MODE_ITEMS) {
        if (m_currentSubTab == TAB_NINJA) {
            const std::vector<CPlayerNinja*>& ninjas = pData->getAllNinjas();
            itemCount = (int)ninjas.size();
            for (int i = 0; i < itemCount; ++i) {
                itemNames.push_back(ninjas[i]->firefly_GetName());
                itemIcons.push_back(ninjas[i]->getIconPath());
                itemStars.push_back(ninjas[i]->firefly_GetQuality());
                std::stringstream ss;
                ss << "Lv." << ninjas[i]->firefly_GetLevel();
                itemBadges.push_back(ss.str());
            }
        } else if (m_currentSubTab == TAB_WEAPON || m_currentSubTab == TAB_ARMOR || m_currentSubTab == TAB_ACCESSORY) {
            int slotType = (m_currentSubTab == TAB_WEAPON) ? 1 : ((m_currentSubTab == TAB_ARMOR) ? 2 : 3);
            std::vector<CGameCardEquipment*> equips = pData->getEquipmentsBySlot(slotType);
            itemCount = (int)equips.size();
            for (int i = 0; i < itemCount; ++i) {
                itemNames.push_back(equips[i]->getName());
                itemIcons.push_back(equips[i]->getIconPath());
                itemStars.push_back(equips[i]->getStar());
                itemBadges.push_back(equips[i]->isEquipped() ? "Đang Mặc" : "");
            }
        } else if (m_currentSubTab == TAB_MARK) {
            const std::vector<CGameCardMark*>& marks = pData->getAllMarks();
            itemCount = (int)marks.size();
            for (int i = 0; i < itemCount; ++i) {
                itemNames.push_back(marks[i]->getName());
                itemIcons.push_back(marks[i]->getIconPath());
                itemStars.push_back(marks[i]->getQuality());
                itemBadges.push_back(marks[i]->isEquipped() ? "Đang Đeo" : "");
            }
        }
    } else {
        // Mode Túi Mảnh
        int pType = 1;
        if (m_currentSubTab == TAB_PIECE_EQUIP) pType = 2;
        else if (m_currentSubTab == TAB_PIECE_NINJUTSU) pType = 4;
        else if (m_currentSubTab == TAB_PIECE_PET) pType = 5;

        std::vector<CPlayerNinjaPiece*> pieces = pData->getPiecesByType(pType);
        itemCount = (int)pieces.size();
        for (int i = 0; i < itemCount; ++i) {
            itemNames.push_back(pieces[i]->getName());
            itemIcons.push_back(pieces[i]->getIconPath());
            itemStars.push_back(pieces[i]->getStar());
            std::stringstream ss;
            ss << pieces[i]->getCount() << "/" << pieces[i]->getReqCount();
            if (pieces[i]->canSynthesize()) ss << " (Đủ)";
            itemBadges.push_back(ss.str());
        }
    }

    if (itemCount == 0) {
        CCLabelTTF* pEmpty = CCLabelTTF::create("Túi đồ hiện đang trống", "Helvetica", 22.0f);
        pEmpty->setPosition(ccp(210.0f, 280.0f));
        pEmpty->setColor(ccc3(148, 163, 184));
        m_pNodeTableContent->addChild(pEmpty);
        updatePreviewPanel();
        return;
    }

    // Hiển thị lưới vật phẩm (3 cột, cuộn dọc)
    int cols = 3;
    float cellW = 135.0f;
    float cellH = 135.0f;
    int rows = (itemCount + cols - 1) / cols;
    float totalH = rows * cellH;

    m_pGridContainer = CCNode::create();
    m_pGridContainer->setContentSize(CCSizeMake(420, totalH));

    for (int i = 0; i < itemCount; ++i) {
        int r = i / cols;
        int c = i % cols;
        float x = c * cellW + cellW / 2.0f;
        float y = totalH - (r * cellH + cellH / 2.0f);

        // Khung cell
        CCLayerColor* pCellBg = CCLayerColor::create((i == m_selectedItemIndex) ? ccc4(59, 130, 246, 200) : ccc4(30, 41, 59, 200), 120, 120);
        pCellBg->ignoreAnchorPointForPosition(false);
        pCellBg->setAnchorPoint(ccp(0.5f, 0.5f));
        pCellBg->setPosition(ccp(x, y));
        m_pGridContainer->addChild(pCellBg);

        // Tên vật phẩm
        CCLabelTTF* pName = CCLabelTTF::create(itemNames[i].c_str(), "Helvetica-Bold", 14.0f, CCSizeMake(115, 36), kCCTextAlignmentCenter);
        pName->setPosition(ccp(x, y - 40.0f));
        pName->setColor(ccc3(255, 255, 255));
        m_pGridContainer->addChild(pName, 2);

        // Icon ảnh
        CCSprite* pIcon = CCSprite::create(itemIcons[i].c_str());
        if (!pIcon) pIcon = CCSprite::create("0V.png");
        if (pIcon) {
            pIcon->setPosition(ccp(x, y + 10.0f));
            pIcon->setScale(0.45f);
            m_pGridContainer->addChild(pIcon, 2);
        }

        // Badge trạng thái
        if (!itemBadges[i].empty()) {
            CCLabelTTF* pBadge = CCLabelTTF::create(itemBadges[i].c_str(), "Helvetica-Bold", 13.0f);
            pBadge->setPosition(ccp(x, y + 45.0f));
            pBadge->setColor(ccc3(250, 204, 21));
            m_pGridContainer->addChild(pBadge, 3);
        }
    }

    m_pScrollView = CCScrollView::create(CCSizeMake(420, 560), m_pGridContainer);
    m_pScrollView->setDirection(kCCScrollViewDirectionVertical);
    m_pScrollView->setContentOffset(ccp(0, 560 - totalH));
    m_pNodeTableContent->addChild(m_pScrollView);

    updatePreviewPanel();
}

void CMyBackpackCardView::updatePreviewPanel() {
    if (!m_pNodeCardContent) return;

    CPlayerDataMgr* pData = CPlayerDataMgr::sharedManager();
    if (!pData) return;

    if (m_currentMode == MODE_ITEMS) {
        if (m_currentSubTab == TAB_NINJA) {
            const std::vector<CPlayerNinja*>& ninjas = pData->getAllNinjas();
            if (m_selectedItemIndex < (int)ninjas.size()) {
                CPlayerNinja* n = ninjas[m_selectedItemIndex];
                if (m_pLabelPreviewName) m_pLabelPreviewName->setString(n->firefly_GetName().c_str());
                if (m_pLabelPreviewLevel) {
                    std::stringstream ss;
                    ss << "Lv." << n->firefly_GetLevel() << " (" << n->firefly_GetQuality() << " Sao)";
                    m_pLabelPreviewLevel->setString(ss.str().c_str());
                }
                if (m_pLabelPreviewStats) {
                    std::stringstream ss;
                    ss << "Công: " << n->firefly_GetAttackMin() << " - " << n->firefly_GetAttackMax()
                       << "\nThủ: " << n->firefly_GetDefenseMin() << " - " << n->firefly_GetDefenseMax()
                       << "\nChakra: " << n->firefly_GetChakraMin() << " - " << n->firefly_GetChakraMax();
                    m_pLabelPreviewStats->setString(ss.str().c_str());
                }
                if (m_pLabelPreviewDesc) m_pLabelPreviewDesc->setString(n->firefly_GetDesc().c_str());
                if (m_pBtnAction) m_pBtnAction->setTitleForState(CCString::create("Xem Tướng"), CCControlStateNormal);
            }
        } else if (m_currentSubTab == TAB_WEAPON || m_currentSubTab == TAB_ARMOR || m_currentSubTab == TAB_ACCESSORY) {
            int slotType = (m_currentSubTab == TAB_WEAPON) ? 1 : ((m_currentSubTab == TAB_ARMOR) ? 2 : 3);
            std::vector<CGameCardEquipment*> equips = pData->getEquipmentsBySlot(slotType);
            if (m_selectedItemIndex < (int)equips.size()) {
                CGameCardEquipment* eq = equips[m_selectedItemIndex];
                if (m_pLabelPreviewName) m_pLabelPreviewName->setString(eq->getName().c_str());
                if (m_pLabelPreviewLevel) {
                    std::stringstream ss;
                    ss << "Lv." << eq->getLevel() << " (" << eq->getStar() << " Sao)";
                    m_pLabelPreviewLevel->setString(ss.str().c_str());
                }
                if (m_pLabelPreviewStats) {
                    std::stringstream ss;
                    ss << "Công: +" << eq->getAttack()
                       << "\nThủ: +" << eq->getDefense()
                       << "\nChakra: +" << eq->getChakra();
                    m_pLabelPreviewStats->setString(ss.str().c_str());
                }
                if (m_pLabelPreviewDesc) m_pLabelPreviewDesc->setString(eq->getDesc().c_str());
                if (m_pBtnAction) {
                    m_pBtnAction->setTitleForState(CCString::create(eq->isEquipped() ? "Tháo Ra" : "Trang Bị"), CCControlStateNormal);
                }
            }
        }
    } else {
        // Mode Mảnh Ghép
        int pType = 1;
        if (m_currentSubTab == TAB_PIECE_EQUIP) pType = 2;
        else if (m_currentSubTab == TAB_PIECE_NINJUTSU) pType = 4;
        else if (m_currentSubTab == TAB_PIECE_PET) pType = 5;

        std::vector<CPlayerNinjaPiece*> pieces = pData->getPiecesByType(pType);
        if (m_selectedItemIndex < (int)pieces.size()) {
            CPlayerNinjaPiece* p = pieces[m_selectedItemIndex];
            if (m_pLabelPreviewName) m_pLabelPreviewName->setString(p->getName().c_str());
            if (m_pLabelPreviewLevel) {
                std::stringstream ss;
                ss << "Số lượng: " << p->getCount() << " / " << p->getReqCount();
                m_pLabelPreviewLevel->setString(ss.str().c_str());
            }
            if (m_pLabelPreviewStats) {
                m_pLabelPreviewStats->setString(p->canSynthesize() ? "Đã đủ mảnh để Hợp Thành!" : "Cần thu thập thêm mảnh.");
            }
            if (m_pLabelPreviewDesc) m_pLabelPreviewDesc->setString("Thu thập đủ số lượng mảnh để hợp thành nhận Nhẫn Giả / Trang bị hoàn chỉnh.");
            if (m_pBtnAction) {
                m_pBtnAction->setTitleForState(CCString::create(p->canSynthesize() ? "Hợp Thành" : "Chưa Đủ"), CCControlStateNormal);
            }
        }
    }
}

// -------------------------------------------------------------
// SỰ KIỆN NÚT BẤM CCB & TAB
// -------------------------------------------------------------
void CMyBackpackCardView::onBtnModeBackpack(CCObject* pSender, CCControlEvent pEvent) {
    switchMode(MODE_ITEMS);
}

void CMyBackpackCardView::onBtnModePieceBackpack(CCObject* pSender, CCControlEvent pEvent) {
    switchMode(MODE_PIECES);
}

void CMyBackpackCardView::onTabNinja(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_NINJA);
}

void CMyBackpackCardView::onTabWeapon(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_WEAPON);
}

void CMyBackpackCardView::onTabArmors(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_ARMOR);
}

void CMyBackpackCardView::onTabAccessori(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_ACCESSORY);
}

void CMyBackpackCardView::onTabMarks(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_MARK);
}

void CMyBackpackCardView::onTabNinjutsu(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_NINJUTSU);
}

void CMyBackpackCardView::onTabNinjaPiece(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_PIECE_NINJA);
}

void CMyBackpackCardView::onTabEquipPiece(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_PIECE_EQUIP);
}

void CMyBackpackCardView::onTabNinjutsuPiece(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_PIECE_NINJUTSU);
}

void CMyBackpackCardView::onTabPetPiece(CCObject* pSender, CCControlEvent pEvent) {
    switchSubTab(TAB_PIECE_PET);
}

void CMyBackpackCardView::onBtnActionClick(CCObject* pSender, CCControlEvent pEvent) {
    CCLog("[CMyBackpackCardView] Nhan nut Action (Trang bi / Hop thanh) cho item %d", m_selectedItemIndex);
}

void CMyBackpackCardView::onSelectItem(int index) {
    m_selectedItemIndex = index;
    updatePreviewPanel();
}

// -------------------------------------------------------------
// CCB RESOLVERS CHO MyBackpackView.ccbi
// -------------------------------------------------------------
SEL_MenuHandler CMyBackpackCardView::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName) {
    return NULL;
}

SEL_CCControlHandler CMyBackpackCardView::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnBackPack", CMyBackpackCardView::onBtnModeBackpack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnPieceBackPack", CMyBackpackCardView::onBtnModePieceBackpack);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnNinja", CMyBackpackCardView::onTabNinja);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnWeapon", CMyBackpackCardView::onTabWeapon);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnArmors", CMyBackpackCardView::onTabArmors);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnAccessori", CMyBackpackCardView::onTabAccessori);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnMarks", CMyBackpackCardView::onTabMarks);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "Btnninjutsu", CMyBackpackCardView::onTabNinjutsu);

    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnNinjaPiece", CMyBackpackCardView::onTabNinjaPiece);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnEquipPiece", CMyBackpackCardView::onTabEquipPiece);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnninjutsuPiece", CMyBackpackCardView::onTabNinjutsuPiece);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "BtnpetPiece", CMyBackpackCardView::onTabPetPiece);
    return NULL;
}

bool CMyBackpackCardView::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode) {
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_backpack", CCNode*, m_pNodeBackpack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_piecebackpack", CCNode*, m_pNodePieceBackpack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_tablecontent", CCNode*, m_pNodeTableContent);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "node_cardcontent", CCNode*, m_pNodeCardContent);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnBackPack", CCControlButton*, m_pBtnBackPack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnPieceBackPack", CCControlButton*, m_pBtnPieceBackPack);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnNinja", CCControlButton*, m_pBtnNinja);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnWeapon", CCControlButton*, m_pBtnWeapon);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnArmors", CCControlButton*, m_pBtnArmors);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnAccessori", CCControlButton*, m_pBtnAccessori);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnMarks", CCControlButton*, m_pBtnMarks);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Btnninjutsu", CCControlButton*, m_pBtnNinjutsu);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnNinjaPiece", CCControlButton*, m_pBtnNinjaPiece);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnEquipPiece", CCControlButton*, m_pBtnEquipPiece);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnninjutsuPiece", CCControlButton*, m_pBtnNinjutsuPiece);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "BtnpetPiece", CCControlButton*, m_pBtnPetPiece);

    return false;
}

void CMyBackpackCardView::onHttpRequestCompleted(CRLRequest* pRequest) {
    if (!pRequest || !pRequest->isSuccess()) return;
    refreshItemList();
}
