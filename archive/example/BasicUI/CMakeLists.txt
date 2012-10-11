# BasicUI Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/BasicUI.cpp
)

include_directories(${SRC_DIR})

set(LIBRARIES)

# Module Project Creation
add_executable(exBasicUI ${SRC})
set_target_properties(exBasicUI PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exBasicUI ce_base)

set(LIBRARIES ce_base ${LIBRARIES})
target_link_libraries(exBasicUI ${LIBRARIES})
