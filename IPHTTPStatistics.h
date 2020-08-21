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

#ifndef __HTTP_STATISTICS_H__
#define __HTTP_STATISTICS_H__

#include "IPLatencyReport.h"
#include "IPSessionSummary.h"

#define COUNT_NONE	0
#define VIDEO_END_DATA_VERSION		"2.0"

/*
 * Defines Video Stat count type
 */
typedef enum E_VideoStatCountType {
	COUNT_UNKNOWN,
	COUNT_LIC_TOTAL,
	COUNT_LIC_ENC_TO_CLR,
	COUNT_LIC_CLR_TO_ENC,
} VideoStatCountType;


/*
 *  Class to store all Video stats common to all download types
 */
class CHTTPStatistics
{
protected:
	CSessionSummary* mSessionSummary;
	CLatencyReport* mLatencyReport;
public:
	CHTTPStatistics() : mSessionSummary(NULL), mLatencyReport(NULL)
	{

	}

	~CHTTPStatistics()
	{
		if(mSessionSummary)
		{
			delete mSessionSummary;
			mSessionSummary = NULL;
		}
		if(mLatencyReport)
		{
			delete mLatencyReport;
			mLatencyReport = NULL;
		}
	}

	CHTTPStatistics(const CHTTPStatistics& newObj): CHTTPStatistics()
	{
		if(newObj.mSessionSummary)
		{
			mSessionSummary = new CSessionSummary(*newObj.mSessionSummary);
		}
		if(newObj.mLatencyReport)
		{
			mLatencyReport = new CLatencyReport(*newObj.mLatencyReport);
		}
	}

	CHTTPStatistics& operator=(const CHTTPStatistics& newObj)
	{
		if(newObj.mSessionSummary)
		{
			mSessionSummary = new CSessionSummary(*newObj.mSessionSummary);
		}
		else
		{
			if(mSessionSummary)
			{
				delete mSessionSummary;
				mSessionSummary = NULL;
			}
		}
		if(newObj.mLatencyReport)
		{
			mLatencyReport = new CLatencyReport(*newObj.mLatencyReport);
		}
		else
		{
			if(mLatencyReport)
			{
				delete mLatencyReport;
				mLatencyReport = NULL;
			}
		}
	}

	/**
	 *   @brief Returns Latency report and allocates if not allocated.
	 *   @param[in]  NONE
	 *   @return CLatencyReport pointer
	 */
	CLatencyReport * GetLatencyReport()
	{
		if(!mLatencyReport)
		{
			mLatencyReport = new CLatencyReport();
		}
		return mLatencyReport;
	}

	/**
	 *   @brief Returns session summary and allocates if not allocated.
	 *   @param[in]  NONE
	 *   @return CSessionSummary pointer
	 */
	CSessionSummary * GetSessionSummary()
	{
		if(!mSessionSummary)
		{
			mSessionSummary = new CSessionSummary();
		}
		return mSessionSummary;
	}

	/**
	 *   @brief  Increment stat count
	 *
	 *   @param[in]  download time
	 *   @param[in]  HTTP/CURL response code
	 * 	 @param[in] bool - connection status flag
	 *
	 *   @return None
	 */
	void IncrementCount(long downloadTimeMs, int responseCode, bool connectivity);

	/**
	 *   @brief  Converts class object data to Json object
	 *
	 *   @param[in]  NONE
	 *
	 *   @return cJSON pointer
	 */
	cJSON * ToJson() const;
};



#endif /* __HTTP_STATISTICS_H__ */
