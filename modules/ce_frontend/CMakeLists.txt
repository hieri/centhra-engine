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
	${INC_DIR}/ScanCode.h
)

set(SRC
	${SRC_DIR}/AppFrontend.cpp
	${SRC_DIR}/Canvas.cpp
	${SRC_DIR}/Keyboard.cpp
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

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB "")

find_package(OpenGL REQUIRED)
list(APPEND INCLUDE ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/external/opengl/include ${CE_BASE_INCLUDE_DIRS})
list(APPEND LIB ce_base ${OPENGL_LIBRARIES})

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

ce_library_module("ce_frontend" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "ce_base" "${LIB}" "")
set(CE_FRONTEND_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_frontend include directories")
