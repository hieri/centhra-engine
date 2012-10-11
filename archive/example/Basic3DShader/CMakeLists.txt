# Basic3DShader Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/Basic3DShader.cpp
)

include_directories(${SRC_DIR})

# Module Project Creation
add_executable(exBasic3DShader ${SRC})
set_target_properties(exBasic3DShader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exBasic3DShader ce_base ce_o3d)
