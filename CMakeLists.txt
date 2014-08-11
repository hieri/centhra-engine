cmake_minimum_required(VERSION 2.8)

# Linux
set(CE_BASE_USEPOSIXTHREAD 1)
set(CE_BASE_USEPOSIXSOCKET 1)
set(CE_FRONTEND_USEXLIB 1)
set(CE_FRONTEND_USEXCB 0)

# Windows
set(CE_BASE_USEWINTHREAD 1)
set(CE_BASE_USEWINSOCKET 1)
set(CE_FRONTEND_USEWIN 1)

project(centhra-engine)

if(WIN32)
   set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a .dll.a .dll)
else()
   set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(modules)
add_subdirectory(samples)
add_subdirectory(plugins)
