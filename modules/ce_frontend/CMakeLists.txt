set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/AppFrontend.h
	${INC_DIR}/Canvas.h
	${INC_DIR}/ConfigFrontend.h
	${INC_DIR}/Event.h
	${INC_DIR}/Frontend.h
	${INC_DIR}/Keyboard.h
	${INC_DIR}/KeyCode.h
	${INC_DIR}/RenderPrimitives.h
	${INC_DIR}/ScanCode.h
)

set(SRC
	${SRC_DIR}/AppFrontend.cpp
	${SRC_DIR}/Canvas.cpp
	${SRC_DIR}/Keyboard.cpp
	${SRC_DIR}/RenderPrimitives.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(FRONTEND_USEXLIB ${CE_FRONTEND_USEXLIB})
	set(FRONTEND_USEXCB ${CE_FRONTEND_USEXCB})
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(FRONTEND_USEXLIB 0)
	set(FRONTEND_USEXCB 0)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(FRONTEND_USEWIN ${CE_FRONTEND_USEWIN})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(FRONTEND_USEWIN "0")
endif()

configure_file(${INC_DIR}/ConfigFrontend.h.in ${INC_DIR}/ConfigFrontend.h)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(T_GLEXT_FILE "${PROJECT_SOURCE_DIR}/external/`/include/GL/glext.h")
	set(T_WGLEXT_FILE "${PROJECT_SOURCE_DIR}/external/opengl/include/GL/wglext.h")

	if(NOT EXISTS ${T_GLEXT_FILE})
		message("Downloading glext.h...")
		set(T_GLEXT_URL "http://www.opengl.org/registry/oldspecs/glext.h")
		file(DOWNLOAD "${T_GLEXT_URL}" "${T_GLEXT_FILE}")
	endif()

	if(NOT EXISTS ${T_WGLEXT_FILE})
		message("Downloading wglext.h...")
		set(T_WGLEXT_URL "http://www.opengl.org/registry/oldspecs/wglext.h")
		file(DOWNLOAD "${T_WGLEXT_URL}" "${T_WGLEXT_FILE}")
	endif()
endif()

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(X11 REQUIRED)
	list(APPEND LIB ${X11_LIBRARIES})
	list(APPEND INCLUDE ${X11_INCLUDE_DIR})

	if(${CE_FRONTEND_USEXCB})
		find_package(XCB REQUIRED)
		list(APPEND LIB ${XCB_LIBRARY} ${XLIB_XCB_LIBRARY})
		list(APPEND INCLUDE ${XCB_INCLUDE_PATH} ${XLIB_XCB_INCLUDE_PATH})
	else()
		find_package(XCB)
		if(XCB_FOUND)
			list(APPEND LIB ${XCB_LIBRARY} ${XLIB_XCB_LIBRARY})
		endif()
	endif()

	list(APPEND LIB dl Xau Xdmcp)
endif()

find_package(OpenGL REQUIRED)
list(APPEND INCLUDE ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/external/opengl/include ${CE_BASE_INCLUDE_DIRS})
list(APPEND LIB ce_base ${OPENGL_LIBRARIES})

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "ce_frontend")
add_library(${TARGET} STATIC ${SRC} ${INC})
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX -d)
target_include_directories(${TARGET} PUBLIC ${INCLUDE})
target_link_libraries(${TARGET} ${LIB})
add_dependencies(${TARGET} ce_base)
set(CE_FRONTEND_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_frontend include directories")
