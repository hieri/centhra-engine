
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/ConfigUI.h
	${INC_DIR}/UI.h
	${INC_DIR}/UI/Control.h
	${INC_DIR}/UI/ButtonCtrl.h
	${INC_DIR}/UI/ColorCtrl.h
	${INC_DIR}/UI/ImageCtrl.h
	${INC_DIR}/UI/TextCtrl.h
	${INC_DIR}/UI/TextButtonCtrl.h
	${INC_DIR}/UI/TextEditCtrl.h
)

set(SRC
	${SRC_DIR}/Control.cpp
	${SRC_DIR}/ButtonCtrl.cpp
	${SRC_DIR}/ColorCtrl.cpp
	${SRC_DIR}/ImageCtrl.cpp
	${SRC_DIR}/TextCtrl.cpp
	${SRC_DIR}/TextButtonCtrl.cpp
	${SRC_DIR}/TextEditCtrl.cpp
)

configure_file(${INC_DIR}/ConfigUI.h.in ${INC_DIR}/ConfigUI.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/modules/ce_base/include ${PROJECT_SOURCE_DIR}/modules/ce_frontend/include ${PROJECT_SOURCE_DIR}/external/include)
set(NAME "ce_ui")
add_library(${NAME} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(${NAME} ${LIB})
