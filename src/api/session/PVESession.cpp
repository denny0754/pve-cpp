/* Project Headers */
#include <pve/api/session/PVESession.hpp>
#include <pve/api/internal/InternalUtility.hpp>

/* External Headers */
#include <curl/curl.h>
#include <fmt/format.h>

/* Standard Headers */
#include <iostream>

namespace pve
{

PVESession::PVESession(const std::string& hostname,
            uint16_t port,
            const std::string& username,
            const std::string& password,
            const std::string& realm,
            bool verify_ssl,
            PVESessionProtocol proto)
{
    m_pveHostname = hostname;
    m_pvePort = port;
    m_pveUsername = username;
    m_pvePassword = password;
    m_pveRealm = realm;
    m_verifySsl = verify_ssl;
    m_pveProtocol = proto;
    m_connected = false;
    Connect();
}

PVESession::~PVESession()
{
    if(m_nativeCurlHandle)
    {
        curl_easy_cleanup((CURL*)m_nativeCurlHandle);
        m_nativeCurlHandle = nullptr;
        m_connected = false;
    }
}

void PVESession::Connect()
{
    // Initialize the connection only if the connection hasnt't been initialized yet.
    if(!IsConnectionOk())
    {
        m_nativeCurlHandle = curl_easy_init();

        // If the native CURL handle couldn't be initialized, we declare the session
        // as already disconnected.
        if(!m_nativeCurlHandle)
        {
            m_connected = false;
            return;
        }

        std::string protocol = "https";
        if(m_pveProtocol == PVESessionProtocol::PROTO_HTTP)
        {
            protocol = "http";
        }

        m_apiUrl = fmt::format("{0}://{1}:{2}", protocol, m_pveHostname, m_pvePort);

        m_connected = true;
    }
}

void PVESession::Disconnect()
{
    if(m_nativeCurlHandle)
    {
        curl_easy_cleanup((CURL*)m_nativeCurlHandle);
        m_nativeCurlHandle = nullptr;
        m_connected = false;
    }
}

nlohmann::json PVESession::DoGet(const std::string& api_rel_path,
                       const nlohmann::json& req_body,
                       const nlohmann::json& req_header,
                       const nlohmann::json& req_cookie)
{
    return DoRequest(
        "GET",
        api_rel_path,
        req_body,
        req_header,
        req_cookie
    );
}

nlohmann::json PVESession::DoPost(const std::string& api_rel_path,
                        const nlohmann::json& req_body,
                        const nlohmann::json& req_header,
                        const nlohmann::json& req_cookie)
{
    return DoRequest(
        "POST",
        api_rel_path,
        req_body,
        req_header,
        req_cookie
    );
}

nlohmann::json PVESession::DoRequest(const std::string& http_method,
                           const std::string& api_rel_path,
                           const nlohmann::json& req_body,
                           const nlohmann::json& req_header,
                           const nlohmann::json& req_cookie)
{

    // Lock guard for multi-threaded scenario
    std::lock_guard<std::mutex> mt_lock(m_mtMutex);

    // Initializing response data
    std::string raw_response = std::string();
    nlohmann::json json_response = nlohmann::json();
    CURLcode execution_code;

    // If the connection has not been enstablished correctly, return an error.
    if(!IsConnectionOk() || !m_nativeCurlHandle)
    {
        json_response["data"] = nlohmann::json::parse("{}");
        json_response["error"] = true;
        json_response["errorMsg"] = "An internal error has occured. The underlaying handle has not been initialized correctly.";
        json_response["statusCode"] = 400;
        return json_response;
    }

    // curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLOPT_VERBOSE, 1L);

    // Setting the HTTP method
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_CUSTOMREQUEST, http_method.c_str());

    // Setting HTTP headers.
    struct curl_slist* http_header_data = NULL;
    pve::internal::CURLHELPER_ConvertJsonHeader(req_header, http_header_data);
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_HTTPHEADER, http_header_data);

    // Setting HTTP body
    nlohmann::json req_body_chg = req_body;
    req_body_chg["username"] = fmt::format("{0}@{1}", m_pveUsername, m_pveRealm);
    req_body_chg["password"] = m_pvePassword;
    std::string req_body_str = req_body_chg.dump();
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_POSTFIELDS, req_body_str.c_str());

    // Setting the function and response variable references to store the response data itself
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_WRITEFUNCTION, pve::internal::CURLHELPER_WriteDataFunction);
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_WRITEDATA, &raw_response);

    // Setting the URL of the request
    std::string req_url = fmt::format("{0}{1}", m_apiUrl, api_rel_path);
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_URL, req_url.c_str());

    // Setting the cookie
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_COOKIE, req_cookie.dump().c_str());

    // Setting SSL Verification flags
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_SSL_VERIFYHOST, m_verifySsl);
    curl_easy_setopt((CURL*)m_nativeCurlHandle, CURLoption::CURLOPT_SSL_VERIFYPEER, m_verifySsl);

    // Exeucting the request
    execution_code = curl_easy_perform((CURL*)m_nativeCurlHandle);

    // Getting the HTTP response status code.
    long status_code = 0;
    curl_easy_getinfo(m_nativeCurlHandle, CURLINFO::CURLINFO_RESPONSE_CODE, &status_code);

    // If the exeuction of the request is not succesful.
    if(execution_code != CURLcode::CURLE_OK)
    {        
        json_response["data"] = nlohmann::json::parse("{}");
        json_response["error"] = true;
        json_response["errorMsg"] = curl_easy_strerror(execution_code);
        json_response["statusCode"] = status_code;
    }
    // If the execution of the request is OK
    else
    {
        json_response["data"] = nlohmann::json::parse(raw_response);
        json_response["error"] = false;
        json_response["errorMsg"] = "";
        json_response["statuscode"] = status_code;
    }

    // Resetting and cleaning up the current request.
    curl_easy_reset(m_nativeCurlHandle);
    curl_slist_free_all(http_header_data);
    http_header_data = nullptr;

    return json_response;
}

} // ns pve