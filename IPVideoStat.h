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
 * @file VideoStat.h
 * @brief
 */

#ifndef __VIDEO_STAT_H__
#define __VIDEO_STAT_H__

#include "IPProfileInfo.h"

// Map of profile bitrate(Bits per sec) and CProfileInfo
// 0 is reserved for Main HLS manifest or DASH manifest
typedef std::map<long, CProfileInfo> MapProfileInfo;

typedef std::map<std::string, MapProfileInfo>  MapStreamInfo; // collection of all Audip/Video/Profile info
typedef std::map<std::string, CLicenseStatistics>  MapLicenceInfo; // Licence stats for each track
typedef std::map<std::string, CFragmentStatistics> MapAdStreamInfo; //collection of ad audio/video fragments statistics

class CVideoStat
{
private:
	long long mTmeToTopProfile;
	long long mTimeAtTopProfile;
	// this is not an asset total duration, this is total duration video was played by user
	// this may be more than asset duration if user rewinds and playagain
	long long mTotalVideoDuration;
	int mNetworkDropCount;
	int mErrorDropCount;
	bool mbTsb;
	int mDisplayWidth; // TV Display Width
	int mDisplayHeight; // TV Display Height

	MapLicenceInfo mMapLicenseInfo;
	MapStreamInfo mMapStreamInfo;
	std::map<std::string ,std::string> mMapLang;
public:

	/**
	 *   @brief Default constructor
	 *
	 *   @param[in]  NONE
         *
	 *   @return None
	 */
	CVideoStat() : mTmeToTopProfile(0), mTimeAtTopProfile(0),mTotalVideoDuration(0), mNetworkDropCount(COUNT_NONE), mErrorDropCount (COUNT_NONE),
					mMapStreamInfo(),mMapLang(),mMapLicenseInfo(),mbTsb(false),mDisplayWidth(0),mDisplayHeight(0)
	{

	}

	/**
	 *   @brief Default Destructor
	 *
	 *   @param[in]  NONE
         *
	 *   @return None
	 */
	~CVideoStat()
	{

	}

	/**
	 *   @brief Sets time to top Profile Time stat
	 *
	 *   @param[in]  long long time
         *
	 *   @return None
	 */
	void SetTimeToTopProfile(long long time) { mTmeToTopProfile = time; }

	/**
	 *   @brief Sets time AT top Profile Time stat
	 *
	 *   @param[in]  long long time
         *
	 *   @return None
	 */
	void SetTimeAtTopProfile(long long time) { mTimeAtTopProfile = time; }

	/**
	 *   @brief Sets total duration of videoFragments downloaded
	 *   Note that this is not Video Duration of Asset, as fragments can be re-downloaded in case of trick play
	 *
	 *   @param[in]  long long Duration
         *
	 *   @return None
	 */
	void SetTotalDuration(long long duration) { mTotalVideoDuration =  duration; }

	/**
	 *   @brief Increment NetworkDropCount
	 *   This is count which indicates if bitrate drop happned due to Network bandwidth issue
	 *
	 *   @param[in] None
         *
	 *   @return None
	 */
	void Increment_NetworkDropCount() { mNetworkDropCount++; }

	/**
	 *   @brief Increment ErrorDropCount
	 *   This is count which indicates if bitrate drop happned due to erros in downloads
	 *
	 *   @param[in] None
         *
	 *   @return None
	 */
	void Increment_ErrorDropCount() { mErrorDropCount++; }

	/**
	 *   @brief Increment Normal Fragment stats
	 *
	 *    @param[in] std::string - Indicates track for which Increment required
	 *    @param[in]bitrate : profile bitrate
	 *    @param[in] download time - download time
	 *    @param[in] response - HTTP/CURL response
	 *    @param[in] bool - connection status flag
	 *   @return None
	 */
	void Increment_Fragment_Count(std::string eType, long bitrate, long downloadTimeMs, int response, bool connectivity);

	/**
	 *   @brief Increment Init Fragment stats ( used for dash case only )
	 *
	 *    @param[in] std::string - Indicates track for which Increment required
	 *    @param[in] bitrate : profile bitrate
	 *    @param[in] download time - download time
	 *    @param[in] response - HTTP/CURL response
	 *    @param[in] bool - connection status flag
	 *
	 *   @return None
	 */
	void Increment_Init_Fragment_Count(std::string eType, long bitrate, long downloadTimeMs, int response, bool connectivity);

	/**
	 *   @brief Increment Manifest stats
	 *
	 *    @param[in] std::string - Indicates track for which Increment required
	 *    @param[in] bitrate : profile bitrate ( 0 means Main HLS Mainifest or DASH manifest )
	 *    @param[in] download time - download time
	 *    @param[in] response - HTTP/CURL response
	 *    @param[in] bool - connection status flag
	 *
	 *   @return None
	 */
	void Increment_Manifest_Count(std::string eType, long bitrate, long downloadTimeMs, int response, bool connectivity);

	/**
	 *   @brief   Records License stat based on isEncypted
	 *
	 *   @param[in] VideoStatTrackType - Indicates track
	 *   @param[in] isEncypted - Indicates clear(false) or encrypted ( true)
	 *   @param[in] isKeyChanged - indicates if key is changed for encrypted fragment
	 *   @return None
	 */
	void Record_License_EncryptionStat(VideoStatTrackType eType, bool isEncypted, bool isKeyChanged, int audioIndex = 1);

	/**
	 *   @brief Increment stats ,
	 *
	 *   @param[in] VideoStatDataType - indicates type of Data ( e.g manifest/fragment/license etc )
	 *   @param[in] VideoStatTrackType - Indicates track for which Increment required
	 *   @param[in] bitrate : profile bitrate
	 *   @param[in] download time - download time
	 *   @param[in] response - HTTP/CURL response
	 *   @param[in] bool - connection status flag
	 *   @param[in] audioIndex- Audio track index
	 *
	 *   @return None
	 */
	void Increment_Data(VideoStatDataType dataType,VideoStatTrackType eType, long bitrate, long downloadTimeMs, int response, bool connectivity, int audioIndex = 1);

	/**
	 *   @brief Sets URL for failed download fragments
	 *
	 *   @param[in] VideoStatTrackType - Indicates track for which Increment required
	 *   @param[in] bitrate : profile bitrate
	 *   @param[in] url : failed content URL
	 *   @param[in] audioIndex- Audio track index
	 *   @return None
	 */
	void SetFailedFragmentUrl(VideoStatTrackType eType, long bitrate, std::string url, int audioIndex = 1);

	/**
	 *   @brief Sets profile frame size
	 *   @param[in]  Profile or track type
	 *   @param[in]  int width
	 *   @param[in]  int height
	 *   @param[in] audioIndex- Audio track index
	 *   @param[in] cappedProfile
	 *   @return None
	 */
	void SetProfileResolution(VideoStatTrackType eType, long bitrate, int width, int height, int audioIndex = 1, bool cappedProfile = false);

	/**
	 *   @brief Sets Display frame size,
	 *   @param[in]  int width
	 *   @param[in]  int height
	 *   @return None
	 */
	void SetDisplayResolution(int width, int height);

	/**
	 *   @brief sets Lang associated with Audio Tracks
	 *
	 *   @param[in]  VideoStatTrackType - Audio Track
	 *   @param[in]  std::string lang string
	 *   @param[in]  Audio Track index
	 *   @return None
	 */
	void Setlanguage(VideoStatTrackType eType, std::string strLang, int audioIndex);

	/**
	 *   @brief sets time shift buffer status
	 *
	 *   @param[in]  bEnable = true means Tsb used.
	 *   @return None
	 */
	void SetTsbStatus(bool bEnable) { mbTsb = bEnable;}


	/**
	 *   @brief Returns string of JSON object
	 *
	 *   @param[in]  None
	 *
	 *   @return char * - Note that caller is responsible for deleting memory allocated for string
	 */
	char * ToJsonString(const char* additionalData = nullptr) const;


	/**
	 *   @brief Converts Tracktype enum to string
	 *
	 *   @param[in]  VideoStatTrackType - track type
	 *   @param[in]  int - Audio track index
	 *
	 *   @return std::string - track type in string format
	 */
	std::string GetTrackTypeStr(VideoStatTrackType type, int audioIndex);
};
#endif /* __VIDEO_STAT_H__ */
