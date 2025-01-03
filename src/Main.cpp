#include <iostream>
#include <pve/api/session/PVESession.hpp>
#include <pve/api/access/PVETicket.hpp>
#include <pve/api/access/PVEUser.hpp>

int main()
{
    pve::PVESession _session = pve::PVESession("172.20.10.13", 8006, "root", "test_proxmox_api", "pam", false);

    if(!_session.IsConnectionOk())
    {
        std::cerr << "Error while initializing session..." << std::endl;
    }

    // pve::PVETicket _ticket = pve::PVETicket();
    // _ticket.GenerateTicket(_session);

    // std::cout << "CSRF Prevention Token: " << _ticket.GetCSRFPreventionToken() << std::endl;
    // std::cout << "Ticket: " << _ticket.GetTicket() << std::endl;

    pve::access::PVEUser root_user = pve::access::PVEUser("root@pam");

    root_user.GetUser(_session);

    _session.Disconnect();
}

/*
void main()
{
    pve::Session session = pve::Session("node.proxmox.local", 8006, "admin", "p@ssw0rd", "localhost");

    pve::Node node = pve::Node(session, "node0");

    std::vector<pve::LXC> all_containers = node.GetLxc();
    std::vector<pve::LXC> ranged_containers = node.GetLxc(101, 102, 103, 104);
    pve::LXC single_container = node.GetLxc(101);

    pve::LXC container = pve::LXC(session, "node0", 101);

    pve::NetworkInterface intf = pve::NetworkInterface(session, node, "envp20");
    // pve::NetworkInterface intf = pve::NetworkInterface(session, "node0", "envp20");
    intf.SetGateway("192.168.0.1");

    // Applies the settings to the server.
    intf.Apply();

    // Restores changes made so far by refetching the data from the session.
    intf.Restore();

    pve::NetworkInterface intf_to_del = pve::NetworkInterface(session, "node0", "envpb30");
    intf_to_del.Shutdown();
    intf_to_del.Delete();

    //////

    // Setting Lazy Load to false so that all resources are fetched in a unique method call
    // Note: Lazy Load does not fetch the data in one API call!!!
    //       Lazy Loading can be used in order to avoid unnecessary API call to resources that are not actually needed.
    //       `User` for example has a sub-call to make in order to fetch the `Tokens` assigned to it.
    //       If lazy load is active, those tokens(if any), are not fetched unless the developer calls the `pve::User::GetToken()` method.
    //       If lazy load is inactive, the tokens are fetched when the User is fetched. `pve::User::GetToken()` won't make another call, but instead will return the tokens fetched previously.
    pve::User root = pve::User(false);

    root.Fetch(session, "root");

    if(!root.Exist())
    {
        root.Create(session);
    }



    //////////////////////////
    pve::PVESession _session = pve::PVESession(
        "1.2.3.4",      // Hostname/IP
        8006,           // Port
        "pve",          // Realm
        "api_user",     // Username
        "password",     // Password
        true            // Verify SSL
    );

    pve::PVEUser tmp_user = pve::PVEUser("tmp_user");
    tmp_user.get(_session);
    if(!tmp_user.Exist())
    {
        if(!tmp_user.Create())
        {
            std::cout << "Failed to create `tmp_user`..." << std::endl;
        }
    }

    auto root_user = _session.get<pve::PVEUser>("root");

    ///  CURL C++ Library Wrapper
    curl::CurlSession _curl = curl::CurlSession();

    curl::CurlRequest _request = curl::CurlRequest();
    _request.SetMethod(curl::CurlOption::HTTP_POST);
    _request.SetOption(curl::CurlOption::URL, "https://localhost:8006/api2/json/users");
    _request.SetOption(curl::CurlOption::SSL_VERIFYHOST, true);
    _request.SetOption(curl::CurlOption::SSL_VERIFYPEER, true);
    curl::CurlResponse _response = _curl.SendRequest(_request);
}


*/