# Basic3D Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/Basic3D.cpp
)

include_directories(${SRC_DIR})

# Module Project Creation
add_executable(exBasic3D ${SRC})
set_target_properties(exBasic3D PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exBasic3D ce_base ce_o3d)
