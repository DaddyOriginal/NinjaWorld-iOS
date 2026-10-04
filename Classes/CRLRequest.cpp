#include "CRLRequest.h"
#include "CServerListMgr.h"
#include <sstream>

CRLRequest::CRLRequest()
    : m_pDelegate(NULL)
    , m_pTarget(NULL)
    , m_pSelectorND(NULL)
    , m_method(METHOD_POST)
    , m_cmdId(0)
    , m_responseCode(0)
{
}

CRLRequest::~CRLRequest() {
    m_pDelegate = NULL;
    m_pTarget = NULL;
    m_pSelectorND = NULL;
}

CRLRequest* CRLRequest::create() {
    CRLRequest* pRequest = new CRLRequest();
    pRequest->autorelease();
    return pRequest;
}

void CRLRequest::setURL(const std::string& url) {
    m_url = url;
}

void CRLRequest::setURL(const char* url) {
    if (url) {
        m_url = url;
    }
}

void CRLRequest::setTargetUrl(const std::string& targetUrl) {
    m_targetUrl = targetUrl;
}

void CRLRequest::setTargetUrl(const char* targetUrl) {
    if (targetUrl) {
        m_targetUrl = targetUrl;
    }
}

void CRLRequest::addData(const std::string& key, const std::string& value) {
    m_fields[key] = value;
}

void CRLRequest::addData(const char* key, const char* value) {
    if (key && value) {
        m_fields[key] = value;
    }
}

void CRLRequest::addData(const char* key, int value) {
    if (key) {
        std::stringstream ss;
        ss << value;
        m_fields[key] = ss.str();
    }
}

void CRLRequest::setPostBody(const std::string& rawBody) {
    m_postBody = rawBody;
}

void CRLRequest::start() {
    if (m_url.empty() && !m_targetUrl.empty()) {
        std::string baseDomain = "http://160.22.123.62:8088";
        if (CServerListMgr::sharedManager() && !CServerListMgr::sharedManager()->getSelectConfig().domain.empty()) {
            baseDomain = CServerListMgr::sharedManager()->getSelectConfig().domain;
        }
        while (!baseDomain.empty() && baseDomain.back() == '/') {
            baseDomain.pop_back();
        }
        if (!m_targetUrl.empty() && m_targetUrl[0] == '/') {
            m_url = baseDomain + m_targetUrl;
        } else {
            m_url = baseDomain + "/" + m_targetUrl;
        }
    }

    if (m_url.empty() || m_url.find("http") == std::string::npos) {
        CCLog("[CRLRequest] URL rong hoac sai dinh dang (%s) -> Dung dia chi may chu mac dinh", m_url.c_str());
        m_url = "http://160.22.123.62:8088/xk_r_dir";
    }

    CCHttpRequest* pHttpRequest = new CCHttpRequest();
    pHttpRequest->setUrl(m_url.c_str());
    pHttpRequest->setRequestType(m_method == METHOD_GET ? CCHttpRequest::kHttpGet : CCHttpRequest::kHttpPost);
    pHttpRequest->setResponseCallback(this, httpresponse_selector(CRLRequest::onHttpRequestCompleted));

    std::string requestBody = m_postBody;

    // Nếu không có raw body, tự động đóng gói form urlencoded
    if (requestBody.empty() && !m_fields.empty()) {
        std::stringstream ss;
        bool first = true;
        for (std::map<std::string, std::string>::iterator it = m_fields.begin(); it != m_fields.end(); ++it) {
            if (!first) {
                ss << "&";
            }
            ss << it->first << "=" << it->second;
            first = false;
        }
        requestBody = ss.str();
    }

    if (!requestBody.empty()) {
        pHttpRequest->setRequestData(requestBody.c_str(), requestBody.length());
        
        // Thêm headers phù hợp
        std::vector<std::string> headers;
        if (requestBody.find("<?xml") != std::string::npos || requestBody.find("<") == 0) {
            headers.push_back("Content-Type: text/xml; charset=utf-8");
        } else {
            headers.push_back("Content-Type: application/x-www-form-urlencoded; charset=utf-8");
        }
        pHttpRequest->setHeaders(headers);
    }

    CCLog("[CRLRequest] Gui request: %s (CMD: %d, DataLen: %lu)", m_url.c_str(), m_cmdId, (unsigned long)requestBody.length());
    this->retain(); // Giữ an toàn đối tượng CRLRequest qua luồng mạng bất đồng bộ
    CCHttpClient::getInstance()->send(pHttpRequest);
    pHttpRequest->release();
}

void CRLRequest::onHttpRequestCompleted(CCHttpClient* pSender, CCHttpResponse* pResponse) {
    if (!pResponse) {
        m_responseCode = -1;
        m_errorMsg = "Response is NULL";
        CCLog("[CRLRequest] Error: pResponse is NULL");
        if (m_pDelegate) {
            m_pDelegate->onHttpError(this, -1, m_errorMsg);
        }
        if (m_pTarget && m_pSelectorND) {
            SEL_CallFuncReq sel = (SEL_CallFuncReq)m_pSelectorND;
            (m_pTarget->*sel)(this);
        }
        this->release(); // Khớp với retain() trong start()
        return;
    }

    m_responseCode = pResponse->getResponseCode();
    bool isSucceed = pResponse->isSucceed();

    if (!isSucceed) {
        std::string err = pResponse->getErrorBuffer();
        m_errorMsg = err;
        CCLog("[CRLRequest] Error (%d): %s", m_responseCode, err.c_str());
        if (m_pDelegate) {
            m_pDelegate->onHttpError(this, m_responseCode, err);
        }
        if (m_pTarget && m_pSelectorND) {
            SEL_CallFuncReq sel = (SEL_CallFuncReq)m_pSelectorND;
            (m_pTarget->*sel)(this);
        }
        this->release(); // Khớp với retain() trong start()
        return;
    }

    std::vector<char>* pBuffer = pResponse->getResponseData();
    if (pBuffer && !pBuffer->empty()) {
        m_responseData.assign(pBuffer->begin(), pBuffer->end());
    } else {
        m_responseData.clear();
    }

    CCLog("[CRLRequest] Success (%d) Len=%lu: %s", m_responseCode, (unsigned long)m_responseData.length(), 
          m_responseData.substr(0, 100).c_str());

    if (m_pDelegate) {
        m_pDelegate->onHttpSuccess(this, m_responseData);
    }

    if (m_pTarget && m_pSelectorND) {
        SEL_CallFuncReq sel = (SEL_CallFuncReq)m_pSelectorND;
        (m_pTarget->*sel)(this);
    }
    this->release(); // Khớp với retain() trong start()
}

