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

#include "HTTPStatistics.h"

/**
 *   @brief  Increment stat count
 *   @param[in]  download time
 *   @param[in]  HTTP/CURL response code
 *   @param[in] bool - connection status flag
 *   @return None
 */
void CHTTPStatistics::IncrementCount(long downloadTimeMs, int responseCode, bool connectivity)
{
	if(responseCode != 200 && responseCode != 204 && responseCode != 206)
	{
		GetSessionSummary()->UpdateSummary(responseCode, connectivity);
	}
	else
	{
		GetLatencyReport()->RecordLatency(downloadTimeMs);
		GetSessionSummary()->UpdateSummary(200, true);
	}
}


/**
 *   @brief  Converts class object data to Json object
 *
 *   @param[in]  NONE
 *
 *   @return cJSON pointer
 */
cJSON * CHTTPStatistics::ToJson() const
{

	//CID:100288 - Removed bDataAdded variable which has been initialized but not used
	cJSON *monitor = cJSON_CreateObject();
	cJSON * jsonObj =  NULL;
	if(monitor)
	{

		if(mLatencyReport)
		{
			jsonObj = mLatencyReport->ToJson();
			if(jsonObj)
			{
				cJSON_AddItemToObject(monitor, "l", jsonObj);
			}
		}

		if(mSessionSummary)
		{
			jsonObj = mSessionSummary->ToJson();
			if(jsonObj)
			{
				cJSON_AddItemToObject(monitor, "S", jsonObj);
			}
		}
	}

	if(jsonObj == NULL)
	{
		//None of the data got added so delete
		cJSON_Delete(monitor);
		monitor = NULL;
	}

	return monitor;
}



