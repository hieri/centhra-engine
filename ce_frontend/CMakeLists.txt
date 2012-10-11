set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)
configure_file(include/CE/ConfigFrontend.h.in include/CE/ConfigFrontend.h)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRC
	${SRC_DIR}/AppFrontend.cpp
	${SRC_DIR}/Canvas.cpp
	${SRC_DIR}/Frontend.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/ce_base/include ${PROJECT_SOURCE_DIR}/external/include)
add_library(ce_frontend STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_frontend PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
