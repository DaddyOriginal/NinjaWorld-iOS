#include "CCBManager.h"

CCBManager* CCBManager::s_instance = NULL;

CCBManager::CCBManager() {
    m_pNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    m_pNodeLoaderLibrary->retain();
}

CCBManager::~CCBManager() {
    CC_SAFE_RELEASE_NULL(m_pNodeLoaderLibrary);
}

CCBManager* CCBManager::sharedManager() {
    if (!s_instance) {
        s_instance = new CCBManager();
    }
    return s_instance;
}

void CCBManager::purge() {
    CC_SAFE_DELETE(s_instance);
}

CCNode* CCBManager::loadNodeFromCCBI(const char* pCCBFileName, CCObject* pOwner, CCObject* pExtra) {
    if (!pCCBFileName) return NULL;

    CCBReader* ccbReader = new CCBReader(m_pNodeLoaderLibrary);
    ccbReader->autorelease();

    std::string path = pCCBFileName;
    if (CCFileUtils::sharedFileUtils()->isFileExist(path)) {
        return ccbReader->readNodeGraphFromFile(path.c_str(), pOwner);
    }

    std::string ccbiPath = "ccbi/" + path;
    if (CCFileUtils::sharedFileUtils()->isFileExist(ccbiPath)) {
        return ccbReader->readNodeGraphFromFile(ccbiPath.c_str(), pOwner);
    }

    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        std::string baseName = path.substr(lastSlash + 1);
        if (CCFileUtils::sharedFileUtils()->isFileExist(baseName)) {
            return ccbReader->readNodeGraphFromFile(baseName.c_str(), pOwner);
        }
        std::string baseCcbiPath = "ccbi/" + baseName;
        if (CCFileUtils::sharedFileUtils()->isFileExist(baseCcbiPath)) {
            return ccbReader->readNodeGraphFromFile(baseCcbiPath.c_str(), pOwner);
        }
    }

    return ccbReader->readNodeGraphFromFile(pCCBFileName, pOwner);
}

CCScene* CCBManager::loadSceneFromCCBI(const char* pCCBFileName, CCObject* pOwner, CCObject* pExtra) {
    CCScene* scene = CCScene::create();
    CCNode* node = loadNodeFromCCBI(pCCBFileName, pOwner, pExtra);
    if (node) {
        scene->addChild(node);
    }
    return scene;
}
