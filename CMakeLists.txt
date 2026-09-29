# dysrcapi
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



### Set Porject
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(DYSRCAPI_BUILD_EXAMPLES "Build Examples" ON)
option(DYSRCAPI_BUILD_TESTS "Build Tests" ON)



### VCPKG vendor location and tiplet
if(WIN32)
  set(VCPKG_TARGET_TRIPLET "x64-mingw-static" CACHE PATH "")
  set(VCPKG_INSTALLED_DIR "${CMAKE_SOURCE_DIR}/vendor" CACHE PATH "")
  list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vendor/x64-mingw-static")
endif()



### Subdirectoriesi
add_subdirectory(dysrcapi)

if(DYSRCAPI_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

#if(DYSRCAPI_BUILD_TESTS)
# add_subdirectory(tests)
#endif()



### Install
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
  TARGETS dysrcapi
  EXPORT DysrcapiTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dysrcapi/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/dysrcapi"
  FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.inl"
)

install(
  EXPORT DysrcapiTargets
  FILE DysrcapiTargets.cmake
  NAMESPACE dysrcapi::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dysrcapi"
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/dysrcapiConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/dysrcapiConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dysrcapi"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/dysrcapiConfigVersion.cmake"
  VERSION 1.0.0
  COMPATIBILITY SameMajorVersion
)

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/dysrcapiConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/dysrcapiConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dysrcapi"
)

