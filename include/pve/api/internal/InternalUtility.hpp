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

struct curl_slist;

namespace pve::internal
{

/**
 * 
 * The following utility function extracts header fields/parameters from a JSON and convert
 * those into a simple string which is then appended to the `curlslist` data read by CURL library.
 * 
 * @param header_data The JSON formatted header data.
 * 
 * @param curl_header_data The list of HTTP Header fields/parameters used by CURL.
 * 
 **/
void CURLHELPER_ConvertJsonHeader(const nlohmann::json& header_data, curl_slist*& curl_header_data);

/**
 * 
 * The following utility function converts a json formatted string into the following cookie format:
 * KEY=VALUE;KEY2=VALUE2
 * 
 * @note For some reason, when setting auth cookie and passing a JSON formatted string, the API returns a 401(no ticket).
 * 
 * @param cookie_data The json formatted cookie list
 * 
 * @param curl_cookie_data The output on which the new cookie formatted list is stored.
 * 
 **/
void CURLHELPER_ConvertJsonCookie(const nlohmann::json& cookie_data, std::string& curl_cookie_data);

/**
 * 
 * The following function is used as callback to write the response of a CURL request into an `std::string`.
 * 
 * @param curl_data The raw response data returned by the CURL perform function.
 * 
 * @param size The number of chunks of data. Alwasy 1.
 * 
 * @param nmemb The size of the actual data.
 * 
 * @param user_data The pointer to the object in which the data is going to be written.
 * 
 * @return The size of data read and stored. If this number differs from the actual size of the data, an error is thrown.
 * 
 **/
size_t CURLHELPER_WriteDataFunction(char* curl_data, size_t size, size_t nmemb, std::string* user_data);

}