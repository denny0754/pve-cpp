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

/* Project Headers */
#include <pve/api/internal/APIInterface.hpp>

/* Standard Headers */
#include <string>
#include <vector>

namespace pve::access
{

class PVEUser : public pve::internal::APIInterface
{
public:
    /**
     * 
     * Default constructor.
     * 
     * Initializes the User with all blank information.
     * 
     **/
    PVEUser();

    /**
     * 
     * Initializes the user with all blank information except for the User ID.
     * 
     * @param userid The id of the user in the format `username@realm`
     * 
     * @warning The `userid` parameter _must_ be in the format `username@realm`. Any other
     * format will not be accepted by the API!
     * 
     **/
    PVEUser(const std::string& userid);

    /**
     * 
     * Returns the User ID of the current User.
     * 
     * @return The User ID of the current User.
     * 
     **/
    inline const std::string& GetUserID() const
    {
        return m_userId;
    }

    /**
     * 
     * Returns the Comment field of the current User.
     * 
     * @return The comment field of the current User.
     * 
     **/
    inline const std::string& GetComment() const
    {
        return m_comment;
    }

    /**
     * 
     * Returns the EMail of the current User.
     * 
     * @return The EMail of the current User.
     * 
     **/
    inline const std::string& GetEmail() const
    {
        return m_email;
    }

    /**
     * 
     * Returns the active flag of the current User.
     * 
     * @return True if the User is active. False otherwise.
     * 
     **/
    inline bool IsActive() const
    {
        return m_isActive;
    }

    /**
     * 
     * Returns the expiration date of the current User.
     * 
     * @return The expiration date of the current user(seconds since epoch). Returns 0 if there is no expiration date.
     * 
     **/
    inline time_t GetExpirationDate() const
    {
        return m_expirationDate;
    }

    /**
     * 
     * Returns the first name of the current User.
     * 
     * @return The first name of the current User.
     * 
     **/
    inline const std::string& GetFirstName() const
    {
        return m_firstName;
    }

    /**
     * 
     * Returns the last name of the current User.
     * 
     * @return The last name of the current User.
     * 
     **/
    inline const std::string& GetLastName() const
    {
        return m_lastName;
    }

    /**
     * 
     * Returns the groups to which the current user is assigned.
     * 
     * @return The groups to which the current user is assigned.
     * 
     **/
    inline const std::vector<std::string>& GetGroups() const
    {
        return m_groups;
    }

    void SetUserID(const std::string& userid);

    void SetComment(const std::string& comment);

    void SetEmail(const std::string& email);

    void SetExpirationDate(time_t date);

    void SetFirstName(const std::string& firstname);

    void SetLastName(const std::string& lastname);

    void SetGroups(const std::vector<std::string>& groups);

    void Activate();

    void Disable();

    void GetUser(pve::PVESession& session);

    /**
     * 
     * Sends a request to the PVE instance for the user to be updated with the information stored
     * in the current instance of object `User`.
     * 
     * @warning Use this method carefully. All fields of the user are always sent to be updated.
     * 
     * @param session Reference to the PVE session
     * 
     **/
    void ApplyChanges(pve::PVESession& session);

    /**
     * 
     * Sends a request to the PVE instance for the user's password to be updated with a new one.
     * 
     * @param session Reference to the PVE session
     * 
     * @param old_password The old password of the User. This is needed for confirmation.
     * 
     * @param new_password The new password of the User.
     * 
     **/
    void UpdatePassword(pve::PVESession& session, const std::string& old_password, const std::string& new_password);

    /**
     * 
     * Sends a request to the PVE instance for the user to be created.
     * 
     * @param session Reference to the PVE session
     * 
     **/
    void Create(pve::PVESession& session);

    /**
     * 
     * Sends a request to the PVE instance for the user to be deleted.
     * 
     * @param session Reference to the PVE session
     * 
     **/
    void Delete(pve::PVESession& session);

protected:
    void DoGet(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie) override;

    void DoPost(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie) override;

    void DoPut(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie) override;

    void DoDelete(pve::PVESession& session, nlohmann::json& req_body, nlohmann::json& req_header, nlohmann::json& req_cookie) override;

private:
    /**
     * 
     * The unique ID of the User.
     * 
     * @note The format of the user id must be the following: username@realm
     * 
     * 
     **/
    std::string m_userId;

    /**
     * 
     * Eventual comments of the User.
     * 
     **/
    std::string m_comment;

    /**
     * 
     * The email of the User.
     * 
     **/
    std::string m_email;

    /**
     * 
     * Active flag. If `false` the user is blocked/not usable.
     * 
     **/
    bool m_isActive;

    /**
     * 
     * Account expiration date (seconds since epoch).
     * '0' means no expiration date.
     * 
     **/
    time_t m_expirationDate;

    /**
     * 
     * First name of the User.
     * 
     **/
    std::string m_firstName;

    /**
     * 
     * Last name of the User.
     * 
     **/
    std::string m_lastName;

    std::string m_keys;

    std::vector<std::string> m_groups;
};

} // ns pve::access