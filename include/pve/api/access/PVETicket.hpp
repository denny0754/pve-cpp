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

namespace pve
{

class PVETicket : public pve::internal::APIInterface
{
public:
    PVETicket();

    void GenerateTicket(pve::PVESession& session);

    inline const std::string& GetTicket() const
    {
        return m_ticket;
    }

    inline const std::string& GetCSRFPreventionToken() const
    {
        return m_csrfPreventionToken;
    }

protected:
    void DoGet(pve::PVESession& session) override;

    void DoPost(pve::PVESession& session) override;

    void DoPut(pve::PVESession& session) override;

    void DoDelete(pve::PVESession& session) override;

private:
    std::string m_ticket;

    std::string m_csrfPreventionToken;
};

} // ns pve