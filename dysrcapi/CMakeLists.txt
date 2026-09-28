# dysrcapi
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



### Set Project
project(dysrcapi LANGUAGES CXX)



### Set Files
set(src
  dysrcapi.cpp
)



### Link Files
add_library(dysrcapi STATIC ${src})
add_library(dysrcapi::dysrcapi ALIAS dysrcapi)

target_include_directories(
  dysrcapi
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

