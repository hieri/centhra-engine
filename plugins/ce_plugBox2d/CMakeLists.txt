set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/PhysicsHandler.cpp
)
set(INC
	${INC_DIR}/CE/Plugin/Box2D/Base.h
	${INC_DIR}/CE/Plugin/Box2D/PhysicsHandler.h
)

find_package(OpenGL REQUIRED)
if(NOT BOX2D_FOUND)
	find_package(Box2D)
	if(NOT BOX2D_FOUND)
		if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
			message("Unable to find Box2D with default paths.")
			set(T_BX_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1.zip")

			if(NOT EXISTS ${T_BX_DOWNLOAD_FILE})
				message("Downloading Box2D source...")
				set(T_BX_URL "https://box2d.googlecode.com/files/Box2D_v2.2.1.zip")
				file(DOWNLOAD "${T_BX_URL}" "${T_BX_DOWNLOAD_FILE}")
			endif()

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Readme.txt)
				message("Unzipping Box2D source...")
				set(T_BX_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/box2d")
				execute_process(COMMAND ${PROJECT_SOURCE_DIR}/external/unzip.bat ${T_BX_DOWNLOAD_FILE} ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1 ${T_BX_EXTRACT_PATH})
			endif()

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Box2D.sln)
				message("Generating Box2D CMake files...")
				execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
			endif()

			if(MINGW)
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Box2D/libBox2D.a)
					message("Building Box2D...")
					execute_process(COMMAND cmake --build . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
				endif()
			else()
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)
					message("Building Box2D...")
					execute_process(COMMAND cmake --build . --config Release WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
					execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/box2d/Box2D/Release/Box2D.lib ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)
					execute_process(COMMAND cmake --build . --config Debug WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
					execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/box2d/Box2D/Debug/Box2D.lib ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D-d.lib)
				endif()
			endif()

			message("Adding Box2D external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/box2d)
			find_package(Box2D REQUIRED)
		endif()

		if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			message("Unable to find Box2D with default paths.")
			set(T_BX_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1.zip")

			if(NOT EXISTS ${T_BX_DOWNLOAD_FILE})
				message("Downloading Box2D source...")
				set(T_BX_URL "https://box2d.googlecode.com/files/Box2D_v2.2.1.zip")
				file(DOWNLOAD "${T_BX_URL}" "${T_BX_DOWNLOAD_FILE}")
			endif()

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Readme.txt)
				message("Unzipping Box2D source...")
				set(T_BX_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/box2d")
				execute_process(COMMAND unzip ${T_BX_DOWNLOAD_FILE} -d ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1 ${T_BX_EXTRACT_PATH})
			endif()

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Box2D/libBox2D.a)
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/CMakeCache.txt)
					message("Generating Box2D CMake files...")
					execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" -DBOX2D_BUILD_EXAMPLES=OFF WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
				endif()

				message("Building Box2D...")
				execute_process(COMMAND cmake --build . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
			endif()

			message("Adding Box2D external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/box2d ${PROJECT_SOURCE_DIR}/external/box2d/Box2D)
			find_package(Box2D REQUIRED)
		endif()
	endif()
endif()

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include ${BOX2D_INCLUDE_DIR})
set(LIB ${BOX2D_LIBRARIES})

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_FRONTEND_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_frontend ce_game2d ce_ui)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "ce_plugBox2d")
add_library(${TARGET} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX -d)
target_include_directories(${TARGET} PUBLIC ${INCLUDE})
target_link_libraries(${TARGET} ${LIB})
add_dependencies(${TARGET} ce_base ce_frontend ce_game2d)
set(CE_PLUGBOX2D_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_plugBox2d include directories")
