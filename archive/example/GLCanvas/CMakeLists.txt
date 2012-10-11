# GLCanvas Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/GLCanvas.cpp
)

include_directories(${SRC_DIR})

# Module Project Creation
add_executable(exGLCanvas ${SRC})
set_target_properties(exGLCanvas PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exGLCanvas ce_base)
