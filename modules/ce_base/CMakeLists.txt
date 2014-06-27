set(VERSION_MAJOR 0)
set(VERSION_MINOR 03)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/App.h
	${INC_DIR}/Audio.h
	${INC_DIR}/Base.h
	${INC_DIR}/Color.h
	${INC_DIR}/ConfigBase.h
	${INC_DIR}/Font.h
	${INC_DIR}/Group.h
	${INC_DIR}/Image.h
	${INC_DIR}/Matrix4x4.h
	${INC_DIR}/Mutex.h
	${INC_DIR}/Plugin.h
	${INC_DIR}/Quaternion.h
	${INC_DIR}/Socket.h
	${INC_DIR}/Thread.h
	${INC_DIR}/Vector2.h
	${INC_DIR}/Vector3.h
)

set(SRC
	${SRC_DIR}/App.cpp
	${SRC_DIR}/Audio.cpp
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/Font.cpp
	${SRC_DIR}/Group.cpp
	${SRC_DIR}/Image.cpp
	${SRC_DIR}/Mutex.cpp
	${SRC_DIR}/Plugin.cpp
	${SRC_DIR}/Socket.cpp
	${SRC_DIR}/Thread.cpp
)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPOSIXTHREAD ${CE_BASE_USEPOSIXTHREAD})
	set(BASE_USEPOSIXSOCKET ${CE_BASE_USEPOSIXSOCKET})
	if(${BASE_USEPOSIXTHREAD})
		set(LIB ${LIB} pthread)
	endif(${BASE_USEPOSIXTHREAD})
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPOSIXTHREAD 0)
	set(BASE_USEPOSIXSOCKET 0)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(INCLUDE ${INCLUDE} ${PROJECT_SOURCE_DIR}/external/OpenGL/include)
	set(BASE_USEWINTHREAD ${CE_BASE_USEWINTHREAD})
	set(BASE_USEWINSOCKET ${CE_BASE_USEWINSOCKET})
	if(${BASE_USEWINSOCKET})
		set(LIB ${LIB} ws2_32)
	endif(${BASE_USEWINSOCKET})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(BASE_USEWINTHREAD "0")
	set(BASE_USEWINSOCKET "0")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# DevIL
find_package(DevIL)
if(NOT IL_FOUND)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
		message("Unable to find DevIL with default paths.")
		set(T_IL_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/DevIL-SDK-x86-1.7.8.zip")

		if(NOT EXISTS ${T_IL_DOWNLOAD_FILE})
			message("Downloading DevIL SDK...")
			set(T_IL_URL "http://downloads.sourceforge.net/project/openil/DevIL%20Windows%20SDK/1.7.8/DevIL-SDK-x86-1.7.8.zip?r=&ts=1403819082&use_mirror=colocrossing")
			file(DOWNLOAD "${T_IL_URL}" "${T_IL_DOWNLOAD_FILE}")
		endif(NOT EXISTS ${T_IL_DOWNLOAD_FILE})

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/DevIL/README)
			message("Unzipping DevIL SDK...")
			set(T_IL_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/DevIL")
			execute_process(COMMAND ${PROJECT_SOURCE_DIR}/external/unzip.bat ${T_IL_DOWNLOAD_FILE} ${T_IL_EXTRACT_PATH})

			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/DevIL/lib/DevIL.dll ${PROJECT_SOURCE_DIR}/samples/bin/DevIL.dll)
			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/DevIL/lib/ILU.dll ${PROJECT_SOURCE_DIR}/samples/bin/ILU.dll)
			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/DevIL/lib/ILUT.dll ${PROJECT_SOURCE_DIR}/samples/bin/ILUT.dll)
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/DevIL/README)

		message("Adding DevIL external prefix.")
		set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/DevIL)
		find_package(DevIL REQUIRED)
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif(NOT IL_FOUND)
set(LIB ${LIB} ${IL_LIBRARIES} ${ILU_LIBRARIES})
set(INCLUDE ${INCLUDE} ${IL_INCLUDE_DIR}/..)

# FreeType
find_package(FreeType)
if(NOT FREETYPE_FOUND)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
		message("Unable to find FreeType with default paths.")
		set(T_FT_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/freetype-2.5.3.tar.bz2")

		if(NOT EXISTS ${T_FT_DOWNLOAD_FILE})
			message("Downloading FreeType source...")
			set(T_FT_URL "http://downloads.sourceforge.net/project/freetype/freetype2/2.5.3/freetype-2.5.3.tar.bz2?ts=1403827588&use_mirror=softlayer-dal")
			file(DOWNLOAD "${T_FT_URL}" "${T_FT_DOWNLOAD_FILE}")
		endif(NOT EXISTS ${T_FT_DOWNLOAD_FILE})

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/README)
			message("Unzipping FreeType source...")
			set(T_FT_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/freetype")
			execute_process(COMMAND cmake -E tar jxf ${T_FT_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
			execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/freetype-2.5.3 ${T_FT_EXTRACT_PATH})
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/README)

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/freetype.sln)
			message("Generating FreeType CMake files...")
			execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/freetype.sln)

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype.lib)
			message("Building FreeType...")
			execute_process(COMMAND cmake --build . --config Release WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/freetype/Release/freetype.lib ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype.lib)
			execute_process(COMMAND cmake --build . --config Debug WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/freetype/Debug/freetype.lib ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype_D.lib)
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype.lib)

		message("Adding FreeType external prefix.")
		set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/freetype)
		find_package(FreeType REQUIRED)
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif(NOT FREETYPE_FOUND)
set(LIB ${LIB} ${FREETYPE_LIBRARIES})
set(INCLUDE ${INCLUDE} ${FREETYPE_INCLUDE_DIRS})

find_package(OpenAL REQUIRED)
set(LIB ${LIB} ${OPENAL_LIBRARY})
set(INCLUDE ${INCLUDE} ${OPENAL_INCLUDE_DIR})

configure_file(${INC_DIR}/ConfigBase.h.in ${INC_DIR}/ConfigBase.h)

include_directories(${INCLUDE})
set(NAME "ce_base")
add_library(${NAME} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(${NAME} ${LIB})
