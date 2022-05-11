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

/**
 * @file IPFragmentStatistics.cpp
 * @brief This file have the stats of IP fragments
 */

#include "IPFragmentStatistics.h"
#include "StatsDefine.h"


cJSON * CFragmentStatistics::ToJson() const
{
	cJSON *monitor = cJSON_CreateObject();
	cJSON * jsonObj = NULL;
	if(monitor)
	{

		if(!m_url.empty())
		{
			jsonObj = cJSON_CreateString(m_url.c_str());
			cJSON_AddItemToObject(monitor, TAG_URL, jsonObj);
		}

		if(pNormalFragmentStat)
		{
			jsonObj = pNormalFragmentStat->ToJson();
			if(jsonObj)
			{
				cJSON_AddItemToObject(monitor, TAG_NORMAL_FRAGMENT_STAT, jsonObj);
			}
		}

		if(pInitFragmentStat)
		{
			jsonObj = pInitFragmentStat->ToJson();

			if(jsonObj)
			{
				cJSON_AddItemToObject(monitor, TAG_NORMAL_INIT_FRAGMENT_STAT, jsonObj);
			}
		}

		if(jsonObj == NULL)
		{
			//nothing is added to monitor
			//delete monitor
			cJSON_Delete(monitor);
			monitor = NULL;
		}
	}
	return monitor;
}

