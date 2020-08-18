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

#include "LatencyReport.h"

#define TAG_LATENCY_REPORT		"l"  	// Latency report tag
#define TAG_TIME_WINDOW_PREFIX		"T"	// Time window prefix
#define DEFAULT_START_VALUE 1
#define LATENCY_WINDOW_BUCKET_DURATION 250


/**
 *   @brief  Converts class object data to Json object
 *
 *   @param[in]  NONE
 *
 *   @return cJSON pointer
 */
cJSON * CLatencyReport::ToJson() const
{
	cJSON *monitor = NULL;

	if(isInitialized)
	{
		monitor = cJSON_CreateObject();
		if(monitor)
		{
			cJSON * jsonObj = NULL;
			for(auto it : mLatencyReportMap)
			{
				jsonObj =  cJSON_CreateNumber(it.second);
				cJSON_AddItemToObject(monitor, it.first.c_str(), jsonObj);
			}

			if(jsonObj == NULL)
			{
				//nothing is added to monitor
				//delete monitor
				cJSON_Delete(monitor);
				monitor = NULL;
			}
		}
	}
	return monitor;
}


/**
 *   @brief  Increments Latency report count
 *   @param[in]  time window
 *
 *   @return None
 */
void CLatencyReport::IncrementCount(std::string window)
{
    if(isInitialized)
    {
		mLatencyReportMap.at(window)++;
    }
}


/**
 *   @brief Calculates time window for latency report
 *   @param[in]  long time in milli seconds
 *   @return string time window tag.
 */
std::string CLatencyReport::GetTimeWindow(long timeMs)
{
	std::string window;
	if(isInitialized)
	{
		unsigned int windowNumber = (timeMs / LATENCY_WINDOW_BUCKET_DURATION);
		window = TAG_TIME_WINDOW_PREFIX + std::to_string(windowNumber);
	}
	return window;
}


/**
 *   @brief Record Latency Report
 *   @param[in]  long time in milli seconds
 *
 *   @return NONE.
 */
void CLatencyReport::RecordLatency(long timeMs)
{
	// Initialize data
	if(!isInitialized)
	{
		isInitialized = true;
	}

	std::string window = GetTimeWindow(timeMs);
	if(!window.empty())
	{
		// Check whether window is already recorded
		if(mLatencyReportMap.find(window) == mLatencyReportMap.end())
		{
			mLatencyReportMap.insert(std::pair<std::string, unsigned int>(window, DEFAULT_START_VALUE));
		}
		else
		{
			IncrementCount(window);
		}
	}
}
