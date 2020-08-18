# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2018 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
cmake_minimum_required (VERSION 2.8)
project (Metrics)

set(LIB_SOURCES HTTPStatistics.cpp
	    LicnsStatistics.cpp
	    FragmentStatistics.cpp
	    VideoStat.cpp
	    ProfileInfo.cpp
	    SessionSummary.cpp
	    LatencyReport.cpp)

add_library(metrics SHARED ${LIB_SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wno-multichar")
target_link_libraries(metrics "-lcjson")

set_target_properties(metrics PROPERTIES PUBLIC_HEADER "VideoStat.h")
install(TARGETS metrics DESTINATION lib PUBLIC_HEADER DESTINATION include)
install(FILES HTTPStatistics.h FragmentStatistics.h LicnsStatistics.h ProfileInfo.h SessionSummary.h LatencyReport.h DESTINATION include)
