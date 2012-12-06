cmake_minimum_required(VERSION 2.8)

# Linux
set(CE_BASE_USEPTHREAD 1)
set(CE_FRONTEND_USEXLIB 1)
set(CE_FRONTEND_USEXCB 1)

# Windows
set(CE_BASE_USEWINTHREAD 1)
set(CE_FRONTEND_USEWIN 1)

project(ce)
add_subdirectory(ce_base)
add_subdirectory(ce_frontend)
add_subdirectory(samples)
