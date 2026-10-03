#include "AppDelegate.h"
#include "CCBManager.h"
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
    pEGLView->setDesignResolutionSize(768, 960, kResolutionShowAll);

    // Turn on display FPS
    pDirector->setDisplayStats(false);

    // Set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // Add search paths for all asset subdirectories
    std::vector<std::string> searchPaths;
    searchPaths.push_back("");
    searchPaths.push_back("ccbResources");
    searchPaths.push_back("sub_ui");
    searchPaths.push_back("upgrade");
    searchPaths.push_back("dlg_ui");
    searchPaths.push_back("characters");
    searchPaths.push_back("level_bg");
    searchPaths.push_back("home");
    searchPaths.push_back("icon");
    searchPaths.push_back("com_res");
    searchPaths.push_back("script");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    // Load LoginView.ccbi as the start scene
    CCScene *pScene = CCScene::create();
    CCNode *loginNode = CCBManager::sharedManager()->loadNodeFromCCBI("LoginView.ccbi");
    if (loginNode) {
        pScene->addChild(loginNode);
    } else {
        CCLog("[ERROR] Failed to load LoginView.ccbi");
    }

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
