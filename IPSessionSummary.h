/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdio.h>
#include <map>
#include <string>
#include <cjson/cJSON.h>
#include <string.h>

#ifndef __SESSION_SUMMARY_H__
#define __SESSION_SUMMARY_H__

typedef std::map<std::string, unsigned int>  MapSessionSummary; // Session summary stats for each profile

class CSessionSummary
{
private:
	bool isInitialized;
	MapSessionSummary mSessionSummaryMap;
public:
	CSessionSummary() : isInitialized(false), mSessionSummaryMap()
	{
	}
	//Keeps track of all the error counts across profiles
	static size_t totalErrorCount;
	/**
	 *   @brief  Increments summary stat count
	 *   @param[in]  HTTP/CURL response
	 *   @return None
	 */
	void IncrementCount(std::string response);

	/**
	 *   @brief  Converts class object data to Json object
	 *   @param[in]  NONE
	 *   @return cJSON pointer
	 */
	cJSON * ToJson() const;

	/**
	 *   @brief Update session summary report
	 *   @param[in]  int http/curl response
	 *	 @param[in] bool - connection status flag
	 *   @return NONE.
	 */
	void UpdateSummary(int response, bool connectivity);
};




#endif /* __SESSION_SUMMARY_H__ */
