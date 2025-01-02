/* Project Headers */
#include <pve/api/access/PVETicket.hpp>
#include <pve/api/session/PVESession.hpp>

/* Standard Headers */
#include <iostream>

namespace pve
{

PVETicket::PVETicket()
{
    m_csrfPreventionToken = std::string();
    m_ticket = std::string();
}

void PVETicket::GenerateTicket(pve::PVESession& session)
{
    DoPost(session);
}

void PVETicket::DoGet(pve::PVESession& session)
{
    // Dummy. Useful for formatters which want to provide a login page.
    // How do we implement this call?
}

void PVETicket::DoPost(pve::PVESession& session)
{
    // Create or verify authentication ticket.
    nlohmann::json req_body = nlohmann::json();
    nlohmann::json req_header = nlohmann::json();
    req_header["Content-Type"] = "application/json";
    req_header["charsets"] = "utf-8";
    nlohmann::json req_cookie = nlohmann::json();

    nlohmann::json response_data = session.DoPost("/api2/json/access/ticket", req_body, req_header, req_cookie);

    if(!response_data["error"].get<bool>())
    {
        m_csrfPreventionToken = response_data["data"]["data"]["CSRFPreventionToken"].get<std::string>();
        m_ticket = response_data["data"]["data"]["ticket"].get<std::string>();
    }
}

void PVETicket::DoPut(pve::PVESession& session)
{
    // NOT USED!
}

void PVETicket::DoDelete(pve::PVESession& session)
{
    // NOT USED!
}

} // ns pve