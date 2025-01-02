# pve-cpp
Proxmox Virtual Environment C++ API Bindings

### Brief Description

PVE-CPP is an API binding for Proxmox Virtual Environment.

### Examples Usages

```c++
#include <pve/PVEAPI.hpp>

int main()
{
    pve::PVESession session = pve::PVESession("1.2.3.4", 8006, "api_user", "api_password", "pam", false);

    // Generating a PVE ticket using the class object `PVETicket`.
    pve::PVETicket ticket = pve::PVETicket();
    ticket.GenerateTicket(session);

    std::cout << "CSRF Prevention Token: " << ticket.GetCSRFPreventionToken() << std::endl;
    std::cout << "Ticket: " << ticket.GetTicket() << std::endl;

    auto another_ticket = session.Get<pve::PVETicket>();

    // Creating an object for user `api_user`
    pve::PVEUser api_user = pve::PVEUser("api_user");

    // Changing the `comment` field of the User.
    api_user.SetComment("This user should only be used to make API calls!");

    // Applying the change of the `comment`.
    api_user.ApplyChanges(session);

    // Updating the password of the user.
    api_user.UpdatePassword(session, "api_password", "new_password");

    // Creating an object of type `pve::LXC` to start a new LXC instance.
    pve::LXC debian_101 = pve::LXC();
    // Setting the Node on which the LXC is going to be created.
    debian_101.SetNode("node0");
    // Setting the VM ID of the container.
    debian_101.SetVMID("101");
    // Setting the OS Template.
    debian_101.SetTemplate("debian-10-standard_10.7-1_amd64.tar.gz");
    // Setting the maximum number of cores the container can use.
    debian_101.SetCores(2);
    // Setting the maximum memory(RAM) the container cna use.
    debian_101.SetMemory(2048);
    // Setting the initial password of the `root` user.
    debian_101.SetPassword("Change_Me!!!");
    // Creating the new container. 
    if(!debian_101.Create(session))
    {
        std::cout << "A problem occured while creating Linux Container `debian_101`." << std::endl;
        return -1;
    }

    // If the container is not running, start it.
    if(debian_101.IsStopped(session))
    {
        debian_101.Start(session);
    }

    // Call is not mandatory! This method is also called on the destructor to cleanup the session.
    // session.Disconnect();
}
```