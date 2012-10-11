cmake_minimum_required(VERSION 2.8)
project(CE)

# Version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)

# Configure Engine Header File
configure_file(include/CE/Config.h.in include/CE/Config.h)

# Set Module Path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# Directories
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Project Config
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Project Creation
add_subdirectory(src)

# Examples
add_subdirectory(example)
