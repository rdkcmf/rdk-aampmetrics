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




#ifndef __PROFILEINFO_H__
#define __PROFILEINFO_H__

#include "IPFragmentStatistics.h"
#include "IPLicnsStatistics.h"


/*
 *  Profile is consist of Manifest , Fragment and License stat,
 *  Here there are two types of fragments Init and Normal and last failed URL is stored
 *  for only fragments hence separate class is created for fragment CFragmentStatistics
 */
class CProfileInfo
{
private:
	CHTTPStatistics * mpManifestStat;
	CFragmentStatistics * mpFragmentStat;
	int mWidth; // Frame width
	int mHeight; // Frame height
public:

	/**
	 *   @brief Default constructor
	 *
	 *   @param[in]  NONE
         *
	 *   @return None
	 */
	CProfileInfo() : mpManifestStat(NULL),mpFragmentStat(NULL),mWidth(0),mHeight(0)
	{

	}

	CProfileInfo(const CProfileInfo & newObj) :CProfileInfo()
	{
		if(newObj.mpManifestStat)
		{
			mpManifestStat = new CHTTPStatistics(*newObj.mpManifestStat);
		}

		if(newObj.mpFragmentStat)
		{
			this->mpFragmentStat = new CFragmentStatistics(*newObj.mpFragmentStat);
		}
	}

	CProfileInfo& operator=(const CProfileInfo& newObj)
	{
		if(newObj.mpManifestStat)
		{
			mpManifestStat = GetManifestStat(); // Allocate if required

			*mpManifestStat = *newObj.mpManifestStat;
		}
		else
		{
			if(mpManifestStat)
			{
				delete mpManifestStat;
				mpManifestStat = NULL;
			}
		}

		if(newObj.mpFragmentStat)
		{
			mpFragmentStat = GetFragementStat(); // Allocate if required

			*mpFragmentStat = *newObj.mpFragmentStat;
		}
		else
		{
			if(mpFragmentStat)
			{
				delete mpFragmentStat;
				mpFragmentStat = NULL;
			}
		}

		return *this;
	}

	/**
	 *   @brief Default Destructor
	 *
	 *   @param[in]  NONE
         *
	 *   @return None
	 */
	~CProfileInfo()
	{
		if(mpManifestStat)
		{
			delete mpManifestStat;
		}
		if(mpFragmentStat)
		{
			delete mpFragmentStat;
		}
	}

	/**
	 *   @brief Returns Manifest stat and allocates if not allocated.
	 *
	 *   @param[in]  NONE
         *
	 *   @return CHTTPStatistics pointer
	 */
	CHTTPStatistics * GetManifestStat()
	{
		if(!mpManifestStat)
		{
			mpManifestStat = new CHTTPStatistics();
		}
		return mpManifestStat;
	}

	/**
	 *   @brief Returns Fragment stat and allocates if not allocated.
	 *
	 *   @param[in]  NONE
         *
	 *   @return CFragmentStatistics pointer
	 */
	CFragmentStatistics * GetFragementStat()
	{
		if(!mpFragmentStat)
		{
			mpFragmentStat = new CFragmentStatistics();
		}
		return mpFragmentStat;
	}

	/**
	 *   @brief Sets the profile frame size
	 *
	 *   @param[in]  width of profile
	 *   @param[in]  Height of profile
         *
	 *   @return None
	 */
	void SetSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	/**
	 *   @brief  Converts class object data to Json object
	 *
	 *   @param[in]  NONE
     *
	 *   @return cJSON pointer
	 */
	cJSON * ToJson() const;


};

#endif //AAMP_SRC_PROFILEINFO_H
