#ifndef _CCB_MANAGER_H_
#define _CCB_MANAGER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CCBManager {
private:
    static CCBManager* s_instance;
    CCNodeLoaderLibrary* m_pNodeLoaderLibrary;

    CCBManager();
    ~CCBManager();

public:
    static CCBManager* sharedManager();
    static void purge();

    CCNode* loadNodeFromCCBI(const char* pCCBFileName, CCObject* pOwner = NULL);
    CCScene* loadSceneFromCCBI(const char* pCCBFileName, CCObject* pOwner = NULL);
};

#endif // _CCB_MANAGER_H_
