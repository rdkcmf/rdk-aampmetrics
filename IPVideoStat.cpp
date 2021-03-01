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
 * @file VideoStat.cpp
 * @brief VideoStat data packing and storage
 */

#include "IPVideoStat.h"

#include <iostream>
#include <algorithm>

#define TAG_VERSION					"vr" 	// version of video end event
//timetoTop
#define TAG_TIME_TO_TOP					"tt"  	// time to reach top profile
#define TAG_TIME_AT_TOP					"ta" 	//time for which video remain on top profile
#define TAG_TIME_PLAYBACK_DURATION			"d" 	// time for which playback was done, this is measured at the time of fragment download , hence play-back duration may be slightly less due to g-streamer and aamp buffers
#define TAG_NET_DROP				"dn"   	// Step down profile count happened due to Bad network bandwidth
#define TAG_ERR_DROP				"de"   	// Step down profile count happened due to Bad download errors/failures
#define TAG_PROFILE_CAPPING                     "pc"     // profile filter status by display resolution
#define TAG_DISPLAY_WIDTH				"w"   	// Display Width
#define TAG_DISPLAY_HEIGHT				"h"   	// Display Height

#define TAG_TSB_AVAILIBLITY				"t"		// indicates if TSB used for playback,
// TAGs for Playback types
#define TAG_MAIN					"m"	// Main manifest
#define TAG_VIDEO					"v"	// Video Profile
#define TAG_IFRAME					"i"	// Iframe Profile
#define TAG_AD_AUDIO					"aa"	// Ad audio track
#define TAG_AD_VIDEO					"av"	// Ad video track
#define TAG_AUDIO					"a"	// Audio track 1
#define TAG_UNKNOWN					"u"	// unknown track

#define TAG_SUPPORTED_LANG				"l"		// Supported language
#define TAG_PROFILES 					"p"		// Encapsulates Different Profile available in stream
#define TAG_LICENSE_STAT				"ls"	// License statistics

const static std::map<VideoStatTrackType, std::string> StatTrackTypeStr = {
	{STAT_MAIN, TAG_MAIN},
	{STAT_VIDEO, TAG_VIDEO},
	{STAT_IFRAME, TAG_IFRAME},
	{STAT_AD_AUDIO, TAG_AD_AUDIO},
	{STAT_AD_VIDEO, TAG_AD_VIDEO},
	{STAT_AUDIO, TAG_AUDIO},
	{STAT_UNKNOWN, TAG_UNKNOWN}
};
/**
 *   @brief Returns string of JSON object
 *
 *   @param[in]  None
 *
 *   @return char * - Note that caller is responsible for deleting memory allocated for string
 */
char * CVideoStat::ToJsonString(const char* additionalData) const
{
	char * strRet = NULL;
	cJSON *monitor = cJSON_CreateObject();
	if(monitor)
	{

		cJSON * jsonObj = NULL;

		if(mTmeToTopProfile > 0 )
		{
			jsonObj =  cJSON_CreateNumber(mTmeToTopProfile);
			cJSON_AddItemToObject(monitor, TAG_TIME_TO_TOP, jsonObj);
		}

		if(mTimeAtTopProfile > 0)
		{
			jsonObj =  cJSON_CreateNumber(mTimeAtTopProfile);
			cJSON_AddItemToObject(monitor, TAG_TIME_AT_TOP, jsonObj);
		}

		if(mTotalVideoDuration > 0 )
		{
			jsonObj =  cJSON_CreateNumber(mTotalVideoDuration);
			cJSON_AddItemToObject(monitor, TAG_TIME_PLAYBACK_DURATION, jsonObj);
		}

		if(mTotalVideoDuration >0 )
		{
			jsonObj =  cJSON_CreateNumber(mNetworkDropCount);
			cJSON_AddItemToObject(monitor, TAG_NET_DROP, jsonObj);
		}

		if(mErrorDropCount > 0)
		{
			jsonObj =  cJSON_CreateNumber(mErrorDropCount);
			cJSON_AddItemToObject(monitor, TAG_ERR_DROP, jsonObj);
		}

		if(mDisplayWidth != 0)
		{
			jsonObj =  cJSON_CreateNumber(mDisplayWidth);
			cJSON_AddItemToObject(monitor, TAG_DISPLAY_WIDTH, jsonObj);
		}
		if(mDisplayHeight != 0)
		{
			jsonObj =  cJSON_CreateNumber(mDisplayHeight);
			cJSON_AddItemToObject(monitor, TAG_DISPLAY_HEIGHT, jsonObj);
		}
		bool isDataAdded = false;

		cJSON *langList = cJSON_CreateObject();
		for (auto const& langItem : mMapLang)
		{

			std::string value = langItem.first;
			if(!value.empty() && !langItem.second.empty())
			{
				isDataAdded = true;
				cJSON *strObj = cJSON_CreateString(langItem.second.c_str());
				cJSON_AddItemToObject(langList, value.c_str(), strObj);
			}
		}

		if(isDataAdded)
		{
			cJSON_AddItemToObject(monitor, TAG_SUPPORTED_LANG, langList);
		}
		else
		{
			cJSON_Delete(langList);
		}

		cJSON *monitor_json = cJSON_Parse(additionalData);

		for (auto const& mapProfileInfo : mMapStreamInfo)
		{
			cJSON *profiles = cJSON_CreateObject();
			jsonObj = NULL;

			isDataAdded = false;

			if(!additionalData)
			{
				for (auto const& profileInfo : mapProfileInfo.second)
				{

					jsonObj = profileInfo.second.ToJson();
					if(jsonObj)
					{
						std::string profileIndiex = std::to_string(profileInfo.first);

						cJSON_AddItemToObject(profiles, profileIndiex.c_str(), jsonObj);
						isDataAdded = true;
					}

				}
			}
			else
			{
				if (monitor_json)
				{
					cJSON *child = monitor_json->child;
					// iterate on stream info
					while(child)
					{
						if(!strcmp(child->string, mapProfileInfo.first.c_str()))
						{
							// Found appropriate stream info
							cJSON *childProfile = child->child;
							// iterate on child profiles and add it to profile stat
							if(childProfile)
							{
								cJSON* childBitrates = childProfile->child;
								while(childBitrates)
								{
									// data collection
									// Add fog data one by one to aamp profiles.
									cJSON_AddItemReferenceToObject(profiles, childBitrates->string, childBitrates);
									isDataAdded = true;
									childBitrates = childBitrates->next;
								}
							}
						}
						child = child->next;
					}
				}
			}

			if(isDataAdded) // at least one profile added to profiles
			{

				cJSON * trackJson  = cJSON_CreateObject();

				cJSON_AddItemToObject(trackJson, TAG_PROFILES, profiles);

				cJSON * licenceJson = NULL;

				auto  it = mMapLicenseInfo.find(mapProfileInfo.first);
				if (it != mMapLicenseInfo.end())
				{
					licenceJson = (*it).second.ToJson();
				}

				if(licenceJson)
				{
					cJSON_AddItemToObject(trackJson, TAG_LICENSE_STAT, licenceJson);
				}

				cJSON_AddItemToObject(monitor, mapProfileInfo.first.c_str(), trackJson);
			}
			else
			{
				cJSON_Delete(profiles);
			}
		}

		// code for ad values to JSON

		// monitor->child will be NULL if none of above data is added to monitor
		if(monitor->child)
		{
			jsonObj = cJSON_CreateString(VIDEO_END_DATA_VERSION);
			cJSON_AddItemToObject(monitor, TAG_VERSION, jsonObj);

			if(mbTsb)
			{
				jsonObj =  cJSON_CreateNumber(1);
				cJSON_AddItemToObject(monitor, TAG_TSB_AVAILIBLITY, jsonObj);
			}

			strRet = cJSON_PrintUnformatted(monitor);
		}

		cJSON_Delete(monitor);
		if (monitor_json)
		{
			cJSON_Delete(monitor_json);
		}
	}
	return strRet;
}


/**
 *   @brief Increment Normal Fragment stats
 *
 *   @param[in] string - Indicates track for which Increment required
 *   @param[in] bitrate : profile bitrate
 *   @param[in] download time - download time
 *   @param[in] response - HTTP/CURL response
 *	 @param[in] bool - connection status flag
 *   @return None
 */
void CVideoStat::Increment_Fragment_Count(std::string eType, long bitrate, long downloadTimeMs, int response, bool connectivity)
{
	if(eType != StatTrackTypeStr.at(STAT_MAIN)) // fragment stats are not applicable for main hls or dash manifest
	{
		//MapProfileInfo mapProfileInfo = mMapStreamInfo[eType];
		CProfileInfo * pinfo = &(mMapStreamInfo[eType][bitrate]);
		pinfo->GetFragementStat()->GetNormalFragmentStat()->IncrementCount(downloadTimeMs, response, connectivity);
	}
}

/**
 *   @brief Increment Init Fragment stats ( used for dash case only )
 *
 *   @param[in] string - Indicates track for which Increment required
 *   @param[in] bitrate : profile bitrate
 *   @param[in] download time - download time
 *   @param[in] response - HTTP/CURL response
 *	 @param[in] bool - connection status flag
 *   @return None
 */
void CVideoStat::Increment_Init_Fragment_Count(std::string eType, long bitrate, long downloadTimeMs, int response, bool connectivity)
{
	if(eType != StatTrackTypeStr.at(STAT_MAIN)) // fragment stats are not applicable for main hls or dash manifest
	{
		CProfileInfo * pinfo = &(mMapStreamInfo[eType][bitrate]);
		pinfo->GetFragementStat()->GetInitFragmentStat()->IncrementCount(downloadTimeMs, response, connectivity);
	}
}

/**
 *   @brief Increment Manifest stats
 *
 *   @param[in] string - Indicates track for which Increment required
 *   @param[in] bitrate : profile bitrate
 *   @param[in] bitrate : profile bitrate ( 0 means Main HLS Mainifest or DASH manifest )
 *   @param[in] download time - download time
 *   @param[in] response - HTTP/CURL response
 *   @param[in] bool - connection status flag
 *
 *   @return None
 */
void CVideoStat::Increment_Manifest_Count(std::string eType, long bitrate, long downloadTimeMs, int response, bool connectivity)
{
	CProfileInfo * pinfo = &(mMapStreamInfo[eType][bitrate]);
	pinfo->GetManifestStat()->IncrementCount(downloadTimeMs, response, connectivity);
}


/**
 *   @brief   Records License stat based on isEncypted
 *
 *   @param[in] VideoStatTrackType - Indicates track
 *   @param[in] isEncypted - Indicates clear(false) or encrypted ( true)
 *   @param[in] isKeyChanged - indicates if key is changed for encrypted fragment
 *   @return None
 */
void CVideoStat::Record_License_EncryptionStat(VideoStatTrackType eType, bool isEncypted, bool isKeyChanged, int audioIndex)
{
	std::string strType = GetTrackTypeStr(eType, audioIndex);
	mMapLicenseInfo[strType].Record_License_EncryptionStat(isEncypted,isKeyChanged);
}

/**
 *   @brief Sets URL for failed download fragments
 *
 *   @param[in]  long long time
 *
 *   @return None
 */
void CVideoStat::SetFailedFragmentUrl(VideoStatTrackType eType, long bitrate, std::string url, int audioIndex)
{
	std::string strType = GetTrackTypeStr(eType, audioIndex);
	if(eType != STAT_MAIN) // fragment stats are not applicable for main hls or dash manifest
	{
		CProfileInfo * pinfo = &(mMapStreamInfo[strType][bitrate]);
		pinfo->GetFragementStat()->SetUrl(url);
	}
}

/**
 *   @brief sets Lang associated with Audio Tracks
 *
 *   @param[in]  VideoStatTrackType - Audio Track
 *   @param[in]  std::string lang string
 *   @param[in]  int audio track index
 *
 *   @return None
 */
void CVideoStat::Setlanguage(VideoStatTrackType eType, std::string strLang, int audioIndex)
{
	std::string strType = GetTrackTypeStr(eType, audioIndex);
	if(eType == VideoStatTrackType::STAT_AUDIO)
	{
		this->mMapLang[strType] = strLang;
	}
}


/**
 *   @brief Increment stats ,
 *
 *   @param[in] VideoStatDataType - indicates type of Data ( e.g manifest/fragment/license etc )
 *   @param[in] VideoStatTrackType - Indicates track for which Increment required
 *   @param[in] bitrate : profile bitrate
 *   @param[in] download time - download time
 *   @param[in] response - HTTP/CURL response
 *   @param[in] bool - connection status flag
 *   @param[in] audioIndex - Audio track index
 *
 *   @return None
 */
void CVideoStat::Increment_Data(VideoStatDataType dataType,VideoStatTrackType eType, long bitrate , long downloadTimeMs, int response, bool connectivity, int audioIndex)
{
	std::string strType = GetTrackTypeStr(eType, audioIndex);
	switch(dataType)
	{
		case VE_DATA_MANIFEST:
			{
				Increment_Manifest_Count(strType,bitrate,downloadTimeMs,response,connectivity);
			}
			break;
		case VE_DATA_FRAGMENT:
			{
				Increment_Fragment_Count(strType,bitrate,downloadTimeMs,response,connectivity);
			}
			break;
		case VE_DATA_INIT_FRAGMENT:
			{
				Increment_Init_Fragment_Count(strType,bitrate,downloadTimeMs,response,connectivity);
			}
			break;
		default:
			break;
	}
}

/**
 *   @brief Sets profile frame size
 *   @param[in]  Profile or track type
 *   @param[in]  int width
 *   @param[in]  int height
 *   @param[in]  int audio index
 *   @param[in]  bool capped profile status
 *   @return None
 */
void CVideoStat::SetProfileResolution(VideoStatTrackType eType, long bitrate, int width, int height, int audioIndex, bool cappedProfile)
{
	std::string strType = GetTrackTypeStr(eType, audioIndex);

	if(eType != STAT_MAIN) // fragment stats are not applicable for main hls or dash manifest
	{
		CProfileInfo * pinfo = &(mMapStreamInfo[strType][bitrate]);
		pinfo->SetSize(width,height,cappedProfile);
	}
}

/**
 *   @brief Sets Display frame size
 *
 *   @param[in]  int width
 *   @param[in]  int height
 *   @return None
 */
void CVideoStat::SetDisplayResolution(int width, int height)
{
	mDisplayWidth = width;
	mDisplayHeight = height;
}

/**
*   @brief Converts Tracktype enum to string
*
*   @param[in]  VideoStatTrackType - track type
*   @param[in]  int - Audio track index
*
*   @return std::string - track type in string format
*/
std::string CVideoStat::GetTrackTypeStr(VideoStatTrackType type, int audioIndex)
{
	if(type == STAT_AUDIO)
	{
		return (StatTrackTypeStr.at(type) + std::to_string(audioIndex));
	}
	else
	{
		return StatTrackTypeStr.at(type);
	}
}
