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

// Unity compilation for CCBManager and UpdateScene
#include "CCBManager.cpp"
#include "UpdateScene.cpp"

