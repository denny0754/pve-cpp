/*
	`pve-cpp` is the C++ utility library to make API calls to a Proxmox server.
	Copyright (C) 2024  Diego Vaccher

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

/* External Headers */
#include <nlohmann/json.hpp>

/* Standard Headers */
#include <string>
#include <mutex>

namespace pve
{

enum class PVESessionProtocol
{
    PROTO_HTTP,
    PROTO_HTTPS
};

class PVESession
{
public:
    /**
     * 
     * Default constructor is deleted. The full constructor must be used
     * in order to enstablish a connection to the Proxmox instance.
     * 
     **/
    PVESession() = delete;

    /**
     * 
     * The following constructor will initialize all parameters needed to connect to the Proxmox instance.
     * 
     * @param hostname The hostname at which the proxmox instance is reached. It must not include the protocol(i.e. `https` or `http`).
     * 
     * @param port The port at which the proxmox instance is reached. By default, proxmox instances are listening on port `8006`.
     * 
     * @param username The user used to connect and make requests to the proxmox instance.
     * 
     * @param password The password of the user.
     * 
     * @param realm The realm on which the user was created. By default, new users on Promox are created on the `pve` realm.
     * 
     * @param verify_ssl Defaults to `true`. If true, any request sent to the proxmox instance is ssl verified.
     * 
     * @param proto Defaults to `HTTPS`. The protocol that should be used when making request to the proxmox instance.
     * 
     **/
    PVESession(const std::string& hostname,
               uint16_t port,
               const std::string& username,
               const std::string& password,
               const std::string& realm,
               bool verify_ssl = true,
               PVESessionProtocol proto = PVESessionProtocol::PROTO_HTTPS
    );

    /**
     * 
     * The following method will initialize the session to the Proxmox instance.
     * This method can be called once, unless the `Disconnect` method has been called, at which point the `Connect` method can be called again.
     * 
     **/
    void Connect();

    /**
     * 
     * The following method cleans up the session to the Proxmox instance.
     * The call of this method will enable the `Connect` method to be called again.
     * 
     **/
    void Disconnect();

    /**
     * 
     * The following method will return the success status of the connection.
     * to the backend.
     * 
     * @return `true` if the backend connection has been initialized correctly. `false` otherwise.
     * 
     **/
    inline bool IsConnectionOk() const
    {
        return m_connected;
    }

    /**
     * 
     * The following destructor cleans up the session to the Proxmox instance.
     * 
     **/
    ~PVESession();

protected:
    /**
     * 
     * The following method will perform a `GET` request to the requested
     * API path defined in `api_rel_path`.
     * The path of the api is relative and it'll be concatenated to the full proxmox instance url.
     * This method is just a helper method. The actual request execution is done in the `DoRequest` method.
     * 
     * @param api_rel_path The relative path to the requested API resource.
     * 
     * @param req_body The body of the request
     * 
     * @param req_header The header of the request
     * 
     * @param req_cookie The cookies of the request.
     * 
     * @return A JSON formatted response in the following format:
     *  {
     *      "data": {...}
     *      "error": [true|false],
     *      "errorMsg": "...",
     *      "statusCode": [200|400|500|...]
     *  }
     * 
     **/
    nlohmann::json DoGet(const std::string& api_rel_path,
               const nlohmann::json& req_body,
               const nlohmann::json& req_header,
               const nlohmann::json& req_cookie
    );

private:
    /**
     * 
     * The following method will perform a `GET` request to the requested
     * API path defined in `api_rel_path`.
     * The path of the api is relative and it'll be concatenated to the full proxmox instance url.
     * This method is just a helper method. The actual request execution is done in the `DoRequest` method.
     * 
     * @param http_method The HTTP method of the request: ["GET"|"POST"|"DELETE"]
     * 
     * @param api_rel_path The relative path to the requested API resource.
     * 
     * @param req_body The body of the request
     * 
     * @param req_header The header of the request
     * 
     * @param req_cookie The cookies of the request.
     * 
     * @return A JSON formatted response in the following format:
     *  {
     *      "data": {...}
     *      "error": [true|false],
     *      "errorMsg": "...",
     *      "statusCode": [200|400|500|...]
     *  }
     * 
     **/
    nlohmann::json DoRequest(
        const std::string& http_method,
        const std::string& api_rel_path,
        const nlohmann::json& req_body,
        const nlohmann::json& req_header,
        const nlohmann::json& req_cookie
    );

private:
    /**
     * 
     * The PVE Hostname at which the proxmox instance is reached.
     * The Hostname should not contain the protocol or trailing slashes.
     * Valid format:
     *  - Domain Format: `my.domain.local`
     *  - IPv4 Format: `1.2.3.4`
     * 
     **/
    std::string m_pveHostname;

    /**
     * 
     * The PVE Port at which the proxmox instance is reached.
     * By default, proxmox is installed to listen on port `8006`.
     * If calls are made behind a proxy, this should be set accordingly.
     * 
     **/
    uint16_t m_pvePort = 8006;

    /**
     * 
     * The Proxmox user that will be used to make requests to the
     * proxmox instance.
     * 
     **/
    std::string m_pveUsername;

    /**
     * 
     * The password of the Proxmox user used to make requests
     * to the proxmox instance.
     * 
     **/
    std::string m_pvePassword;

    /**
     * 
     * The realm on which the user was created.
     * By default, new user are created on the `pve` or `pam` proxmox
     * realms.
     * 
     **/
    std::string m_pveRealm;

    /**
     * 
     * Flag used to enable the verification of SSL Host and Peer.
     * CURL Options: SSL_VERIFY_HOST and SSL_VERIFY_PEER.
     * 
     **/
    bool m_verifySsl = true;

    /**
     * 
     * The protocol at which the proxmox instance is reached with.
     * New installations of proxmox usually work on the `HTTPS` protocol.
     * 
     **/
    PVESessionProtocol m_pveProtocol = PVESessionProtocol::PROTO_HTTPS;

    /**
     * 
     * The actual native CURL handle used to make requests to the
     * proxmox instance.
     * 
     **/
    void* m_nativeCurlHandle = nullptr;

    /**
     * 
     * Flag used to check whether the session has been initialized correctly
     * or not.
     * 
     **/
    bool m_connected = false;

    /**
     * 
     * Mutex used to lock resources so that multi-threaded scenario are possible.
     * 
     **/
    std::mutex m_mtMutex;

    /**
     * 
     * The full URL used to make requests to the proxmox server.
     * THis is the concatenation of the following parameters:
     *  - Protocol
     *  - Hostname
     *  - Port
     * 
     **/
    std::string m_apiUrl;
};

} // ns pve