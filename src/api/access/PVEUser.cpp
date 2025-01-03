/* Project Headers */
#include <pve/api/access/PVEUser.hpp>
#include <pve/api/session/PVESession.hpp>

/* External Headers */
#include <fmt/format.h>

/* Standard Headers */
#include <iostream>

namespace pve::access
{

PVEUser::PVEUser()
{
    m_userId = std::string();
    m_comment = std::string();
    m_email = std::string();
    m_isActive = false;
    m_expirationDate = 0;
    m_firstName = std::string();
    m_lastName = std::string();
    m_keys = std::string();
    m_groups = {};
}

PVEUser::PVEUser(const std::string& userid)
    : m_userId(userid)
{
    m_comment = std::string();
    m_email = std::string();
    m_isActive = false;
    m_expirationDate = 0;
    m_firstName = std::string();
    m_lastName = std::string();
    m_keys = std::string();
    m_groups = {};
}

void PVEUser::SetUserID(const std::string& userid)
{
    m_userId = userid;
}

void PVEUser::SetComment(const std::string& comment)
{
    m_comment = comment;
}

void PVEUser::SetEmail(const std::string& email)
{
    m_email = email;   
}

void PVEUser::Activate()
{
    m_isActive = true;
}

void PVEUser::Disable()
{
    m_isActive = false;
}

void PVEUser::SetExpirationDate(time_t date)
{
    m_expirationDate = date;
}

void PVEUser::SetFirstName(const std::string& firstname)
{
    m_firstName = firstname;
}

void PVEUser::SetLastName(const std::string& lastname)
{
    m_lastName = lastname;
}

void PVEUser::SetGroups(const std::vector<std::string>& groups)
{
    m_groups = groups;
}

void PVEUser::GetUser(pve::PVESession& session)
{
    // API CALL
    // GET /api2/json/access/users/{m_userId}
    nlohmann::json req_body = nlohmann::json::parse("{}");
    nlohmann::json req_header = nlohmann::json::parse("{}");
    nlohmann::json req_cookie = nlohmann::json::parse("{}");

    req_header["Content-Type"] = "application/json";
    req_header["charsets"] = "utf-8";

    nlohmann::json response_data = session.DoGet(
        fmt::format("/api2/json/access/users/{0}", m_userId),
        req_body,
        req_header,
        req_cookie
    );

    std::cout << response_data.dump(4) << std::endl;

    if(!response_data["error"].get<bool>())
    {
        auto user_data = response_data["data"];
        
        if(user_data.find("firstname") != user_data.end())
        {
            m_firstName = user_data["firstname"];
        }

        if(user_data.find("lastname") != user_data.end())
        {
            m_lastName = user_data["lastname"];
        }

        if(user_data.find("comment") != user_data.end())
        {
            m_comment = user_data["comment"];
        }

        if(user_data.find("email") != user_data.end())
        {
            m_email = user_data["email"].get<std::string>();
        }

        if(user_data.find("enable") != user_data.end())
        {
            m_isActive = user_data["enable"].get<bool>();
        }

        if(user_data.find("expire") != user_data.end())
        {
            m_expirationDate = user_data["expire"].get<time_t>();
        }
    }
}

void PVEUser::ApplyChanges(pve::PVESession& session)
{
    // API CALL:
    // PUT /api2/json/access/users/{m_userId}
}

void PVEUser::UpdatePassword(pve::PVESession& session, const std::string& old_password, const std::string& new_password)
{
    // API CALL:
    // PUT /api2/json/access/password
}

void PVEUser::Create(pve::PVESession& session)
{
    // API CALL:
    // POST /api2/json/access/users
}

void PVEUser::Delete(pve::PVESession& session)
{
    // API CALL:
    // DELETE /api2/json/access/users/{m_userId}
}

void PVEUser::DoGet(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie)
{

}

void PVEUser::DoPost(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie)
{

}

void PVEUser::DoPut(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie)
{

}

void PVEUser::DoDelete(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie)
{

}


} // ns pve::access