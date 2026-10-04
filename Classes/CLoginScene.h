#ifndef _CLOGIN_SCENE_H_
#define _CLOGIN_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CRLRequest.h"
#include "CServerListMgr.h"
#include "CServerSelector.h"
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

class CLoginScene 
    : public CCLayer
    , public CCBSelectorResolver
    , public CCBMemberVariableAssigner
    , public CRLNetDelegate
    , public ServerSelectDelegate
{
private:
    // UI Elements ánh xạ từ LoginView.ccbi
    CCLabelTTF* m_pLabelServerName;
    CCLabelTTF* m_pLabelVersionInfo;
    CCLabelTTF* m_pLabelIdName;
    CCControlButton* m_pBtnLogin;
    CCControlButton* m_pBtnReg;
    CCControlButton* m_pBtnSelectServer;

    // Thông tin tài khoản
    std::string m_username;
    std::string m_password;

public:
    CLoginScene();
    virtual ~CLoginScene();

    static CCScene* scene();
    CREATE_FUNC(CLoginScene);

    virtual bool init();
    virtual void onEnter();
    virtual void onExit();

    // CCBSelectorResolver Callbacks (Nút bấm CCBI)
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);

    // CCBMemberVariableAssigner (Gắn biến CCBI)
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    // Sự kiện nút bấm
    void onBtnLogin(CCObject* pSender, CCControlEvent pCCControlEvent);
    void onBtnRegist(CCObject* pSender, CCControlEvent pCCControlEvent);
    void onBtnSelectServer(CCObject* pSender, CCControlEvent pCCControlEvent);

    // ServerSelectDelegate Callback khi người chơi đổi server
    virtual void onServerSelected(int serverId, const std::string& serverName, const std::string& hostUrl);

    // Xử lý luồng mạng CRLNetDelegate
    virtual void onHttpSuccess(CRLRequest* pRequest, const std::string& responseData);
    virtual void onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg);

    // Logic nghiệp vụ
    void doLogin(const std::string& account, const std::string& pwd);
    void doRegister(const std::string& account, const std::string& pwd);
    void requestServerList();
    void enterMainGame();
};

#endif // _CLOGIN_SCENE_H_
