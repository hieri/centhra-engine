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
find_package(Box2D)
if(NOT BOX2D_FOUND)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
		message("Unable to find Box2D with default paths.")
		set(T_BX_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1.zip")

		if(NOT EXISTS ${T_BX_DOWNLOAD_FILE})
			message("Downloading Box2D source...")
			set(T_BX_URL "https://box2d.googlecode.com/files/Box2D_v2.2.1.zip")
			file(DOWNLOAD "${T_BX_URL}" "${T_BX_DOWNLOAD_FILE}")
		endif(NOT EXISTS ${T_BX_DOWNLOAD_FILE})

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Readme.txt)
			message("Unzipping Box2D source...")
			set(T_BX_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/box2d")
			execute_process(COMMAND ${PROJECT_SOURCE_DIR}/external/unzip.bat ${T_BX_DOWNLOAD_FILE} ${PROJECT_SOURCE_DIR}/external)
			execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1 ${T_BX_EXTRACT_PATH})
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Readme.txt)

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Box2D.sln)
			message("Generating Box2D CMake files...")
			execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Box2D.sln)

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)
			message("Building Box2D...")
			execute_process(COMMAND cmake --build . --config Release WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/box2d/Box2D/Release/Box2D.lib ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)
			execute_process(COMMAND cmake --build . --config Debug WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
			execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/box2d/Box2D/Debug/Box2D.lib ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D-d.lib)
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)

		message("Adding Box2D external prefix.")
		set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/box2d)
		find_package(Box2D REQUIRED)
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		message("Unable to find Box2D with default paths.")
		set(T_BX_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1.zip")

		if(NOT EXISTS ${T_BX_DOWNLOAD_FILE})
			message("Downloading Box2D source...")
			set(T_BX_URL "https://box2d.googlecode.com/files/Box2D_v2.2.1.zip")
			file(DOWNLOAD "${T_BX_URL}" "${T_BX_DOWNLOAD_FILE}")
		endif(NOT EXISTS ${T_BX_DOWNLOAD_FILE})

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Readme.txt)
			message("Unzipping Box2D source...")
			set(T_BX_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/box2d")
			execute_process(COMMAND unzip ${T_BX_DOWNLOAD_FILE} -d ${PROJECT_SOURCE_DIR}/external)
			execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/Box2D_v2.2.1 ${T_BX_EXTRACT_PATH})
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/Readme.txt)

		if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)
			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/CMakeCache.txt)
				message("Generating Box2D CMake files...")
				execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" -DBOX2D_BUILD_EXAMPLES=OFF WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/CMakeCache.txt)

			message("Building Box2D...")
			execute_process(COMMAND cmake --build . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/box2d)
		endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/box2d/lib/Box2D.lib)

		message("Adding Box2D external prefix.")
		set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/box2d)
		find_package(Box2D REQUIRED)
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
endif(NOT BOX2D_FOUND)

set(INCLUDE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/modules/ce_base/include
	${PROJECT_SOURCE_DIR}/modules/ce_frontend/include
	${PROJECT_SOURCE_DIR}/modules/ce_game2d/include
	${BOX2D_INCLUDE_DIR}
)

set(LIB
	ce_base
	ce_frontend
	ce_game2d
	${OPENGL_LIBRARIES}
	${BOX2D_LIBRARIES}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(X11 REQUIRED)
	set(LIB ${LIB} ${X11_LIBRARIES})
	if(CE_FRONTEND_USEXCB)
		set(LIB ${LIB} ${PROJECT_SOURCE_DIR}/external/lib/libxcb.so ${PROJECT_SOURCE_DIR}/external/lib/libX11-xcb.so)
	endif(CE_FRONTEND_USEXCB)
	set(INCLUDE ${INCLUDE} ${X11_INCLUDE_DIR})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(${INCLUDE})

set(NAME "ce_plugBox2d")
add_library(${NAME} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(${NAME} ${LIB})
