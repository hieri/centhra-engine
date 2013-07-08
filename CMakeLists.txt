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

if(${WIN32})
	set(IL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../DevIL/include/IL)
	set(IL_LIBRARIES ${PROJECT_SOURCE_DIR}/../DevIL/lib/DevIL.lib)
	set(ILU_LIBRARIES ${PROJECT_SOURCE_DIR}/../DevIL/lib/ILU.lib)
	set(ILUT_LIBRARIES ${PROJECT_SOURCE_DIR}/../DevIL/lib/ILUT.lib)
	set(FREETYPE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../freetype2/include)
	set(FREETYPE_LIBRARY ${PROJECT_SOURCE_DIR}/../freetype2/lib/freetype.lib)
endif(${WIN32})

add_subdirectory(modules)
add_subdirectory(samples)
add_subdirectory(plugins)