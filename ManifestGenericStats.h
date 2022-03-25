/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#ifndef __MANIFEST_GEN_STATS_H__
#define __MANIFEST_GEN_STATS_H__

#include <stdio.h>
#include <map>
#include <string>
#include <cjson/cJSON.h>

//Map to fill the manifest info json 
typedef std::map<std::string,double>  MapManifestData;

//struct to keep track of manifest details
struct ManifestData
{
	long mDownloadTimeMs;
	long mParseTimeMs;
	size_t mSize;
	size_t mPeriodCount;
	
	ManifestData(long downloadTimeMs, size_t size, long parseTimeMs = -1,  size_t periodCount = 0):
	mDownloadTimeMs(downloadTimeMs)
	,mParseTimeMs(parseTimeMs)
	,mSize(size)
	,mPeriodCount(periodCount)
	{
	}
};


class ManifestGenericStats
{
	bool isInitialized;
	MapManifestData mManifestData;
public:

	static size_t totalGaps;

	/**
	 *   @brief  constructor
	 *   @param[in]  NONE
	 *   @return  None
	 */
	ManifestGenericStats() : isInitialized(false), mManifestData()
	{
	}

	/**
	 *   @brief  updates manifest details
	 *   @param[in]  NONE
	 *   @return  None
	 */
	void UpdateManifestData(ManifestData *manifestData);
	
	/**
	 *   @brief  Converts class object data to Json object
	 *   @param[in]  NONE
	 *   @return cJSON pointer
	 */
	cJSON * ToJson() const;
};
#endif /* ManifestGenericStats.h */
