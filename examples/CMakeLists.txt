# dysrcapi
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



### Set Project
project(examples LANGUAGES CXX)

### Add dependencies
find_package(fmt CONFIG REQUIRED)

### Set Project Source
set(src
  "main.cpp"
)

### Link Project Source
add_executable(examples ${src})
target_link_libraries(examples PRIVATE
  dysrcapi::dysrcapi
  fmt::fmt
)
