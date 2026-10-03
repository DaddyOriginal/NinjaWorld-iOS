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

CCNode* CCBManager::loadNodeFromCCBI(const char* pCCBFileName, CCObject* pOwner) {
    if (!pCCBFileName) return NULL;

    CCBReader* ccbReader = new CCBReader(m_pNodeLoaderLibrary);
    ccbReader->autorelease();

    CCNode* node = ccbReader->readNodeGraphFromFile(pCCBFileName, pOwner);
    return node;
}

CCScene* CCBManager::loadSceneFromCCBI(const char* pCCBFileName, CCObject* pOwner) {
    CCScene* scene = CCScene::create();
    CCNode* node = loadNodeFromCCBI(pCCBFileName, pOwner);
    if (node) {
        scene->addChild(node);
    }
    return scene;
}
