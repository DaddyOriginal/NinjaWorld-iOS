#ifndef _CRL_REQUEST_H_
#define _CRL_REQUEST_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include <map>
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

class CRLRequest;

/**
 * CRLNetDelegate: Interface nhận sự kiện phản hồi mạng
 */
class CRLNetDelegate {
public:
    virtual void onHttpSuccess(CRLRequest* pRequest, const std::string& responseData) {}
    virtual void onHttpError(CRLRequest* pRequest, int errorCode, const std::string& errorMsg) {}
};

/**
 * CRLRequest: Module mạng giao tiếp HTTP / REST / XML tương thích Firefly MMO Client
 */
class CRLRequest : public CCObject {
public:
    enum HttpMethod {
        METHOD_GET = 0,
        METHOD_POST = 1
    };

    typedef void (CCObject::*SEL_CallFuncReq)(CRLRequest*);

private:
    std::string m_url;
    std::string m_targetUrl;
    std::string m_postBody;
    std::string m_errorMsg;
    std::map<std::string, std::string> m_fields;
    CRLNetDelegate* m_pDelegate;
    CCObject* m_pTarget;
    SEL_CallFuncND m_pSelectorND;
    HttpMethod m_method;
    int m_cmdId;
    int m_responseCode;
    std::string m_responseData;

public:
    CRLRequest();
    virtual ~CRLRequest();

    static CRLRequest* create();

    void setURL(const std::string& url);
    void setURL(const char* url);
    const std::string& getURL() const { return m_url; }

    void setTargetUrl(const std::string& targetUrl);
    void setTargetUrl(const char* targetUrl);
    const std::string& getTargetUrl() const { return m_targetUrl; }

    void setMethod(HttpMethod method) { m_method = method; }
    HttpMethod getMethod() const { return m_method; }

    void setParam(const std::string& key, const std::string& value) { addData(key, value); }
    void setParam(const char* key, const char* value) { addData(key, value); }
    void setParam(const char* key, int value) { addData(key, value); }

    void addData(const std::string& key, const std::string& value);
    void addData(const char* key, const char* value);
    void addData(const char* key, int value);
    void setPostBody(const std::string& rawBody);

    void setCMD(int cmd) { m_cmdId = cmd; }
    int getCMD() const { return m_cmdId; }

    void setDelegate(CRLNetDelegate* pDelegate) { m_pDelegate = pDelegate; }
    CRLNetDelegate* getDelegate() const { return m_pDelegate; }

    void setCallback(CCObject* pTarget, SEL_CallFuncND pSelector) {
        m_pTarget = pTarget;
        m_pSelectorND = pSelector;
    }

    int getResponseCode() const { return m_responseCode; }
    const std::string& getResponseData() const { return m_responseData; }
    std::string getResponseString() const { return m_responseData; }
    std::string getErrorMessage() const { return m_errorMsg; }
    bool isSuccess() const { return m_responseCode >= 200 && m_responseCode < 300; }

    // Bắt đầu gửi yêu cầu bất đồng bộ
    void start();
    void send() { start(); }

    // Callback hoàn tất từ CCHttpClient
    void onHttpRequestCompleted(CCHttpClient* pSender, CCHttpResponse* pResponse);
};

inline std::string extractTag(const std::string& xml, const std::string& tag) {
    std::string openTag = "<" + tag + ">";
    std::string closeTag = "</" + tag + ">";
    size_t start = xml.find(openTag);
    if (start == std::string::npos) return "";
    start += openTag.length();
    size_t end = xml.find(closeTag, start);
    if (end == std::string::npos) return "";
    return xml.substr(start, end - start);
}

#endif // _CRL_REQUEST_H_
