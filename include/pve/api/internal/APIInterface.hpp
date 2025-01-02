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

// Forward Declarations
namespace pve
{
class PVESession;
}

namespace pve::internal
{

/**
 * 
 * `APIInterface` is a pure virtual class/interface used
 * as base class for API resources which can be fetched, updated or created
 * on a Proxmox instance.
 * Example of resources: Users, Groups, LXCs, etc... 
 * This interface defines a set of methods that should be overridden by the derived
 * class in order to perform specific actions on the derived resource.
 * 
 **/
class APIInterface
{
public:
    /**
     * 
     * Default constructor.
     * 
     **/
    APIInterface() = default;

protected:
    /**
     * 
     * Pure Virtual Method `DoGet` should be used to send `GET` request
     * to the Proxmox server.
     * 
     **/
    virtual void DoGet(pve::PVESession&) = 0;

    /**
     * 
     * Pure virtual method `DoPost` should be used to send `POST` request
     * to the Proxmox server.
     * 
     **/
    virtual void DoPost(pve::PVESession&) = 0;

    /**
     * 
     * Pure virtual method `DoPut` should be used to send `PUT` request
     * to the proxmox server.
     * 
     **/
    virtual void DoPut(pve::PVESession&) = 0;

    /**
     * 
     * Pure virtual method `DoDelete` should be used to send `DELETE` request
     * to the proxmox server.
     * 
     **/
    virtual void DoDelete(pve::PVESession&) = 0;
};

} // pve::internal