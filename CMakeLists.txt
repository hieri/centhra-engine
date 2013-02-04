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

project(ce)
add_subdirectory(modules)
add_subdirectory(samples)
