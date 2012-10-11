set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)
configure_file(include/CE/ConfigDisplay.h.in include/CE/ConfigDisplay.h)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRC
	${SRC_DIR}/AppDisplay.cpp
	${SRC_DIR}/Canvas.cpp
	${SRC_DIR}/Display.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/ce_base/include ${PROJECT_SOURCE_DIR}/external/include)
add_library(ce_display STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_display PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
