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

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB "")

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_FRONTEND_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_frontend)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "ce_ui")
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
add_dependencies(${TARGET} ce_base ce_frontend)
set(CE_UI_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_ui include directories")
