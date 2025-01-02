/* Project Headers */
#include <pve/api/internal/InternalUtility.hpp>

/* External Headers */
#include <curl/curl.h>
#include <fmt/format.h>

namespace pve::internal
{

void CURLHELPER_ConvertJsonHeader(const nlohmann::json& header_data, curl_slist*& curl_header_data)
{
    for(auto& [header_key, header_value] : header_data.items())
    {
        std::string header_item = fmt::format("{0}: {1}", header_key, header_value.get<std::string>());
        curl_header_data = curl_slist_append(curl_header_data, header_item.c_str());
    }
}

size_t CURLHELPER_WriteDataFunction(char* curl_data, size_t size, size_t nmemb, std::string* user_data)
{
    user_data->append((char*) curl_data, size * nmemb);
    return size * nmemb;
}

} // ns pve::internal