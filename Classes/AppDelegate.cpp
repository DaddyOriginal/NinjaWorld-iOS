#include "AppDelegate.h"
#include "CCBManager.h"
#include "CLoginScene.h"
#include "SimpleAudioEngine.h"
#include "support/zip_support/ZipUtils.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Kích hoạt chìa khóa giải mã 1.697 file texture .pvr.ccz gốc của Ninja World
    ZipUtils::ccSetPvrEncryptionKey(0xf013c6ef, 0x5ca560ce, 0x01471215, 0xca9bada1);

    // Chuẩn tỷ lệ màn hình gốc Ninja World từ decompiled AppDelegate
    CCSize frameSize = pEGLView->getFrameSize();
    float ratio = frameSize.height / frameSize.width;
    if (ratio > 1.5f) {
        // Tỷ lệ màn hình điện thoại (16:9, 19.5:9 viền mỏng như iPhone X/11/12/13/14/15/16)
        // Chiều rộng chuẩn 686.0f, chiều cao tự động theo tỷ lệ để màn hình rộng trọn vẹn, không bị ép hẹp
        float designWidth = 686.0f;
        float designHeight = (frameSize.height * designWidth) / frameSize.width;
        pEGLView->setDesignResolutionSize(designWidth, designHeight, kResolutionNoBorder);
    } else {
        // Máy tính bảng iPad (tỷ lệ 4:3)
        pEGLView->setDesignResolutionSize(768.0f, 1024.0f, kResolutionNoBorder);
    }

    // Thiết lập đường dẫn tìm kiếm tài nguyên chuẩn của client gốc
    std::vector<std::string> searchPaths;
    searchPaths.push_back("ccbi");
    searchPaths.push_back("data");
    searchPaths.push_back("ccbResources");
    searchPaths.push_back("characters");
    searchPaths.push_back("animations");
    searchPaths.push_back("com_res");
    searchPaths.push_back("home");
    searchPaths.push_back("icon");
    searchPaths.push_back("npc");
    searchPaths.push_back("sound");
    searchPaths.push_back("backpack");
    searchPaths.push_back("equip");
    searchPaths.push_back("");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    // Turn off display FPS
    pDirector->setDisplayStats(false);

    // Set FPS 60
    pDirector->setAnimationInterval(1.0 / 60);

    // Khởi chạy trực tiếp màn hình Đăng Nhập GỐC của Ninja World (LoginView.ccbi)
    CCScene *pScene = CLoginScene::scene();
    pDirector->runWithScene(pScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

// Unity compilation for Classes
#include "CCBManager.cpp"
#include "CRLRequest.cpp"
#include "CNinjaTableMgr.cpp"
#include "CItemTableMgr.cpp"
#include "CPlayerNinja.cpp"
#include "CGameCardEquipment.cpp"
#include "CPlayerNinjaPiece.cpp"
#include "CGameCardMark.cpp"
#include "CTeamCard.cpp"
#include "CActiveTeamMgr.cpp"
#include "CPlayerDataMgr.cpp"
#include "CServerListMgr.cpp"
#include "CServerSelector.cpp"
#include "CLoginScene.cpp"
#include "CNinjaDetailView.cpp"
#include "CMyGroupCardView.cpp"
#include "CMyBackpackCardView.cpp"
#include "CStageTableMgr.cpp"
#include "CChapterMgr.cpp"
#include "CChapterView.cpp"
#include "CSubChapterView.cpp"
#include "CRoundTeamFightView.cpp"
#include "CRoundResultView.cpp"
#include "CPlayerNinjaRecruitView.cpp"
#include "CPlayerArenaView.cpp"
#include "CTowerTableMgr.cpp"
#include "CTowerMgr.cpp"
#include "CTowerBossView.cpp"
#include "CTowerLevelView.cpp"
#include "CTowerView.cpp"
#include "CDailyTaskTableMgr.cpp"
#include "CDailyTaskMgr.cpp"
#include "CDailyRewardView.cpp"
#include "CDailyTaskView.cpp"
#include "CEightGateMgr.cpp"
#include "CLimitTrainSoulView.cpp"
#include "CEightGateView.cpp"
#include "CMailMgr.cpp"
#include "CMailView.cpp"
#include "CFriendMgr.cpp"
#include "CFriendView.cpp"
#include "CRouletteMgr.cpp"
#include "CRouletteTurnDialogView.cpp"
#include "CRouletteView.cpp"
#include "CSaveTimeMgr.cpp"
#include "CSaveTimeView.cpp"
#include "CAwardCenterMgr.cpp"
#include "CAwardCenterView.cpp"
#include "CGrowthFundMgr.cpp"
#include "CGrowthFundView.cpp"
#include "CMoneyTreeMgr.cpp"
#include "CMoneyTreeView.cpp"
#include "CDefaultMainMenu.cpp"
#include "CMainMenu.cpp"


