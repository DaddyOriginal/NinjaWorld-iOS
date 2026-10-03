#ifndef __UPDATE_SCENE_H__
#define __UPDATE_SCENE_H__

#include "cocos2d.h"
#include <string>

class UpdateScene : public cocos2d::CCLayer {
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(UpdateScene);

    // Callbacks from Downloader
    void onDownloadProgress(float percent, long long downloadedBytes, long long totalBytes);
    void onDownloadFinished(const std::string& filePath);
    void onDownloadFailed(const std::string& error);
    void onUnzipFinished();

    // UI actions
    void startDownload();
    void enterGame();
    void retryClicked(cocos2d::CCObject* pSender);

private:
    cocos2d::CCLabelTTF* m_pStatusLabel;
    cocos2d::CCLabelTTF* m_pDetailLabel;
    cocos2d::CCLayerColor* m_pFillBar;
    cocos2d::CCMenuItemFont* m_pRetryItem;
    cocos2d::CCMenu* m_pMenu;

    std::string m_obbUrl;
    std::string m_obbSavePath;
    std::string m_resDir;
};

#endif // __UPDATE_SCENE_H__
