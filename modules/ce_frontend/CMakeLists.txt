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
	${SRC_DIR}/Frontend.cpp
	${SRC_DIR}/Keyboard.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(FRONTEND_USEXLIB ${CE_FRONTEND_USEXLIB})
	set(FRONTEND_USEXCB ${CE_FRONTEND_USEXCB})
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(FRONTEND_USEXLIB 0)
	set(FRONTEND_USEXCB 0)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(FRONTEND_USEWIN ${CE_FRONTEND_USEWIN})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(FRONTEND_USEWIN "0")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

configure_file(${INC_DIR}/ConfigFrontend.h.in ${INC_DIR}/ConfigFrontend.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/modules/ce_base/include ${PROJECT_SOURCE_DIR}/external/include)
set(NAME "ce_frontend")
add_library(${NAME} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(${NAME} ${LIB})
