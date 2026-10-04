#include "UpdateScene.h"
#include "CCBManager.h"
#include "CLoginScene.h"
#include "CDNDownloaderBridge.h"
#include "support/zip_support/ZipUtils.h"
#include "support/zip_support/unzip.h"
#include <sys/stat.h>

USING_NS_CC;

UpdateScene* UpdateScene::s_instance = NULL;

// C-Bridge Callbacks
static void onNativeProgress(float percent, long long dl, long long total) {
    if (UpdateScene::s_instance) {
        UpdateScene::s_instance->onDownloadProgress(percent, dl, total);
    }
}

static void onNativeSuccess() {
    if (UpdateScene::s_instance) {
        UpdateScene::s_instance->onDownloadFinished();
    }
}

static void onNativeError(const char* err) {
    if (UpdateScene::s_instance) {
        UpdateScene::s_instance->onDownloadFailed(err ? err : "Lỗi kết nối");
    }
}

// Directory creation helper
static void createDirectoryRecursively(const std::string& path) {
    std::string current = "";
    for (size_t i = 0; i < path.length(); ++i) {
        current += path[i];
        if (path[i] == '/' || path[i] == '\\' || i == path.length() - 1) {
            mkdir(current.c_str(), 0755);
        }
    }
}

// Extract OBB Archive (standard ZIP format)
static bool extractObbArchive(const std::string& obbPath, const std::string& outDir) {
    unzFile uf = unzOpen(obbPath.c_str());
    if (!uf) {
        CCLog("[OBB] Error: Cannot open OBB file at %s", obbPath.c_str());
        return false;
    }

    unz_global_info globalInfo;
    if (unzGetGlobalInfo(uf, &globalInfo) != UNZ_OK) {
        unzClose(uf);
        return false;
    }

    createDirectoryRecursively(outDir);
    char buffer[32768];

    for (uLong i = 0; i < globalInfo.number_entry; ++i) {
        unz_file_info fileInfo;
        char filename[512];
        if (unzGetCurrentFileInfo(uf, &fileInfo, filename, sizeof(filename), NULL, 0, NULL, 0) != UNZ_OK) {
            break;
        }

        std::string fullPath = outDir + "/" + filename;
        size_t len = strlen(filename);
        if (filename[len - 1] == '/' || filename[len - 1] == '\\') {
            createDirectoryRecursively(fullPath);
        } else {
            size_t lastSlash = fullPath.find_last_of("/\\");
            if (lastSlash != std::string::npos) {
                createDirectoryRecursively(fullPath.substr(0, lastSlash));
            }

            if (unzOpenCurrentFile(uf) == UNZ_OK) {
                FILE* fOut = fopen(fullPath.c_str(), "wb");
                if (fOut) {
                    int readBytes = 0;
                    while ((readBytes = unzReadCurrentFile(uf, buffer, sizeof(buffer))) > 0) {
                        fwrite(buffer, 1, readBytes, fOut);
                    }
                    fclose(fOut);
                }
                unzCloseCurrentFile(uf);
            }
        }

        if (i + 1 < globalInfo.number_entry) {
            if (unzGoToNextFile(uf) != UNZ_OK) {
                break;
            }
        }
    }

    unzClose(uf);
    return true;
}

// UpdateScene Implementation
CCScene* UpdateScene::scene() {
    CCScene* pScene = CCScene::create();
    UpdateScene* pLayer = UpdateScene::create();
    pScene->addChild(pLayer);
    return pScene;
}

bool UpdateScene::init() {
    if (!CCLayer::init()) {
        return false;
    }

    s_instance = this;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 1. Background (0V.png splash art)
    CCSprite* pBg = CCSprite::create("0V.png");
    if (pBg) {
        pBg->setPosition(ccp(winSize.width * 0.5f, winSize.height * 0.5f));
        float scaleX = winSize.width / pBg->getContentSize().width;
        float scaleY = winSize.height / pBg->getContentSize().height;
        float scale = (scaleX > scaleY) ? scaleX : scaleY;
        pBg->setScale(scale);
        this->addChild(pBg, 0);
    } else {
        CCLayerColor* pDark = CCLayerColor::create(ccc4(15, 23, 42, 255), winSize.width, winSize.height);
        this->addChild(pDark, 0);
    }

    // 2. Bottom HUD Container
    const float hudWidth = 668.0f;
    const float hudHeight = 150.0f;
    const float hudX = (winSize.width - hudWidth) * 0.5f;
    const float hudY = 60.0f;

    CCLayerColor* pHudBg = CCLayerColor::create(ccc4(10, 15, 26, 220), hudWidth, hudHeight);
    pHudBg->setPosition(ccp(hudX, hudY));
    this->addChild(pHudBg, 1);

    // 3. Status Label
    m_pStatusLabel = CCLabelTTF::create("Đang kiểm tra tài nguyên...", "Helvetica-Bold", 24.0f);
    m_pStatusLabel->setPosition(ccp(winSize.width * 0.5f, hudY + 115.0f));
    m_pStatusLabel->setColor(ccc3(255, 255, 255));
    this->addChild(m_pStatusLabel, 2);

    // 4. Progress Bar Track & Fill
    const float barWidth = 628.0f;
    const float barHeight = 26.0f;
    const float barX = (winSize.width - barWidth) * 0.5f;
    const float barY = hudY + 65.0f;

    CCLayerColor* pBarTrack = CCLayerColor::create(ccc4(30, 41, 59, 255), barWidth, barHeight);
    pBarTrack->setPosition(ccp(barX, barY));
    this->addChild(pBarTrack, 2);

    // Gold fill bar (#F59E0B)
    m_pFillBar = CCLayerColor::create(ccc4(245, 158, 11, 255), 0.0f, barHeight);
    m_pFillBar->setPosition(ccp(barX, barY));
    this->addChild(m_pFillBar, 3);

    // 5. Detail Label
    m_pDetailLabel = CCLabelTTF::create("", "Helvetica", 20.0f);
    m_pDetailLabel->setPosition(ccp(winSize.width * 0.5f, hudY + 30.0f));
    m_pDetailLabel->setColor(ccc3(203, 213, 225));
    this->addChild(m_pDetailLabel, 2);

    // 6. Retry Button
    m_pRetryItem = CCMenuItemFont::create("Thử lại tải OBB", this, menu_selector(UpdateScene::retryClicked));
    m_pRetryItem->setFontSize(22);
    m_pRetryItem->setColor(ccc3(245, 158, 11));
    m_pRetryItem->setPosition(ccp(winSize.width * 0.5f, hudY + 30.0f));
    m_pRetryItem->setVisible(false);

    m_pMenu = CCMenu::create(m_pRetryItem, NULL);
    m_pMenu->setPosition(CCPointZero);
    this->addChild(m_pMenu, 4);

    // Setup Paths
    std::string writable = CCFileUtils::sharedFileUtils()->getWritablePath();
    m_obbSavePath = writable + "main.1.com.ninja.world.obb";
    m_resDir = writable + "res";
    std::string flagFile = m_resDir + "/obb_extracted.flag";

    // Default OBB URL from public VPS
    m_obbUrl = "http://160.22.123.62:8088/download/main.1.com.ninja.world.obb";

    // Read custom cdn_config.json if present
    unsigned long size = 0;
    unsigned char* pData = CCFileUtils::sharedFileUtils()->getFileData("cdn_config.json", "r", &size);
    if (pData && size > 0) {
        std::string jsonStr((char*)pData, size);
        delete[] pData;
        size_t pos = jsonStr.find("\"obb_url\"");
        if (pos != std::string::npos) {
            size_t start = jsonStr.find("\"", pos + 9);
            if (start != std::string::npos) {
                size_t end = jsonStr.find("\"", start + 1);
                if (end != std::string::npos) {
                    m_obbUrl = jsonStr.substr(start + 1, end - start - 1);
                }
            }
        }
    }

    // Check if resources already exist
    if (CCFileUtils::sharedFileUtils()->isFileExist(flagFile)) {
        m_pStatusLabel->setString("Tài nguyên đã sẵn sàng! Đang vào game...");
        m_pFillBar->setContentSize(CCSizeMake(barWidth, barHeight));
        m_pDetailLabel->setString("100% Hoàn tất");
        scheduleOnce(schedule_selector(UpdateScene::enterGame), 0.3f);
    } else {
        scheduleOnce(schedule_selector(UpdateScene::startDownload), 0.3f);
    }

    return true;
}

void UpdateScene::startDownload() {
    m_pRetryItem->setVisible(false);
    m_pDetailLabel->setVisible(true);
    m_pStatusLabel->setString("Đang kết nối tải main.1.com.ninja.world.obb...");
    m_pDetailLabel->setString("Bắt đầu kết nối...");
    m_pFillBar->setContentSize(CCSizeMake(0, 26));

    startNativeDownload(m_obbUrl.c_str(), m_obbSavePath.c_str(), onNativeProgress, onNativeSuccess, onNativeError);
}

void UpdateScene::onDownloadProgress(float percent, long long downloadedBytes, long long totalBytes) {
    if (m_pFillBar) {
        float width = 628.0f * (percent > 1.0f ? 1.0f : percent);
        m_pFillBar->setContentSize(CCSizeMake(width, 26));
    }

    char buf[128];
    float dlMB = (float)downloadedBytes / (1024.0f * 1024.0f);
    float totalMB = (float)totalBytes / (1024.0f * 1024.0f);
    snprintf(buf, sizeof(buf), "Đang tải: %.1f MB / %.1f MB (%.0f%%)", dlMB, totalMB, percent * 100.0f);
    m_pStatusLabel->setString("Đang tải gói OBB Ninja World...");
    m_pDetailLabel->setString(buf);
}

void UpdateScene::onDownloadFinished() {
    m_pFillBar->setContentSize(CCSizeMake(628, 26));
    m_pStatusLabel->setString("Tải xong! Đang giải nén tài nguyên OBB...");
    m_pDetailLabel->setString("Quá trình giải nén diễn ra trong vài giây...");

    // Extract archive
    bool ok = extractObbArchive(m_obbSavePath, m_resDir);
    if (ok) {
        // Ghi flag file hoàn tất
        FILE* f = fopen((m_resDir + "/obb_extracted.flag").c_str(), "w");
        if (f) {
            fputs("ok\n", f);
            fclose(f);
        }
        // Xóa file OBB nén tạm thời để tiết kiệm dung lượng bộ nhớ máy
        remove(m_obbSavePath.c_str());
        this->onUnzipFinished();
    } else {
        this->onDownloadFailed("Giải nén OBB thất bại!");
    }
}

void UpdateScene::onDownloadFailed(const std::string& error) {
    m_pStatusLabel->setString("Tải OBB thất bại! Kiểm tra IP Server.");
    m_pDetailLabel->setString(error.c_str());
    m_pRetryItem->setVisible(true);
}

void UpdateScene::onUnzipFinished() {
    m_pStatusLabel->setString("Hoàn tất! Đang khởi động Ninja World...");
    m_pDetailLabel->setString("100% Sẵn sàng");
    scheduleOnce(schedule_selector(UpdateScene::enterGame), 0.5f);
}

void UpdateScene::retryClicked(CCObject* pSender) {
    startDownload();
}

void UpdateScene::enterGame() {
    ZipUtils::ccSetPvrEncryptionKey(0xf013c6ef, 0x5ca560ce, 0x01471215, 0xca9bada1);

    std::vector<std::string> searchPaths;
    searchPaths.push_back(m_resDir);
    searchPaths.push_back(m_resDir + "/activity");
    searchPaths.push_back(m_resDir + "/animations");
    searchPaths.push_back(m_resDir + "/backpack");
    searchPaths.push_back(m_resDir + "/ccbResources");
    searchPaths.push_back(m_resDir + "/characters");
    searchPaths.push_back(m_resDir + "/com_res");
    searchPaths.push_back(m_resDir + "/config");
    searchPaths.push_back(m_resDir + "/core_combat");
    searchPaths.push_back(m_resDir + "/countrywar");
    searchPaths.push_back(m_resDir + "/dlg_ui");
    searchPaths.push_back(m_resDir + "/equip");
    searchPaths.push_back(m_resDir + "/gameobjlist");
    searchPaths.push_back(m_resDir + "/home");
    searchPaths.push_back(m_resDir + "/icon");
    searchPaths.push_back(m_resDir + "/level_bg");
    searchPaths.push_back(m_resDir + "/mark");
    searchPaths.push_back(m_resDir + "/multiserverbattle");
    searchPaths.push_back(m_resDir + "/notice");
    searchPaths.push_back(m_resDir + "/npc");
    searchPaths.push_back(m_resDir + "/props");
    searchPaths.push_back(m_resDir + "/script");
    searchPaths.push_back(m_resDir + "/secretshop");
    searchPaths.push_back(m_resDir + "/serverinfo");
    searchPaths.push_back(m_resDir + "/skill");
    searchPaths.push_back(m_resDir + "/skill_res");
    searchPaths.push_back(m_resDir + "/social");
    searchPaths.push_back(m_resDir + "/sound");
    searchPaths.push_back(m_resDir + "/store");
    searchPaths.push_back(m_resDir + "/sub_ui");
    searchPaths.push_back(m_resDir + "/upgrade");
    // Bundle fallback paths
    searchPaths.push_back("ccbi");
    searchPaths.push_back("data");
    searchPaths.push_back("ccbResources");
    searchPaths.push_back("ccbResources/candidate");
    searchPaths.push_back("com_res");
    searchPaths.push_back("characters");
    searchPaths.push_back("home");
    searchPaths.push_back("");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    CCScene *pScene = CLoginScene::scene();
    if (!pScene) {
        CCLog("[ERROR] Failed to create CLoginScene!");
        pScene = CCScene::create();
    }

    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, pScene));
}
