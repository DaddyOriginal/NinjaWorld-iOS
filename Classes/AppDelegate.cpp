#include "AppDelegate.h"
#include "CCBManager.h"
#include "UpdateScene.h"
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

    // Set Design Resolution 768x960 (Chuẩn tỷ lệ gốc của Ninja World)
    pEGLView->setDesignResolutionSize(768, 960, kResolutionExactFit);

    // Turn on display FPS
    pDirector->setDisplayStats(false);

    // Set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // Khởi chạy UpdateScene: Bản Chuyên Nghiệp (Mini Client + CDN OBB Downloader)
    // Tự động kiểm tra OBB, hiển thị thanh chạy % và tải main.1.com.ninja.world.obb từ máy chủ
    CCScene *pScene = UpdateScene::scene();
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
#include "UpdateScene.cpp"


