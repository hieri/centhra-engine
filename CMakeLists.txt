cmake_minimum_required(VERSION 2.8)

set(CE_CLIENT 1)
if(NOT CE_CLIENT)
	set(CE_NOCLIENT 1)
else()
	set(CE_NOCLIENT 0)
endif()

if(NOT DEFINED CE_BUILD_SAMPLES)
	set(CE_BUILD_SAMPLES 1)
endif()

# Linux
set(CE_BASE_USEPOSIXTHREAD 1)
set(CE_BASE_USEPOSIXSOCKET 1)
set(CE_BASE_USEXLIB 1)
set(CE_BASE_USEXCB 0)

# Windows
set(CE_BASE_USEWINTHREAD 1)
set(CE_BASE_USEWINSOCKET 1)
set(CE_BASE_USEWIN 1)

project(centhra-engine)

set(CE_VERSION_MAJOR 0)
set(CE_VERSION_MINOR 05)

if(WIN32)
	set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)
else()
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
include("${CMAKE_MODULE_PATH}/CE.cmake")

add_subdirectory(modules)
add_subdirectory(plugins)

if(CE_BUILD_SAMPLES)
	add_subdirectory(samples)
endif()
