set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)
configure_file(include/CE/ConfigBase.h.in include/CE/ConfigBase.h)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRC
	${SRC_DIR}/App.cpp
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/Thread.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ce_base STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_base PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(ce_base pthread)
