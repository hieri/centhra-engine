# 3DCharacter Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/3DCharacter.cpp
)

include_directories(${SRC_DIR})

# Module Project Creation
add_executable(ex3DCharacter ${SRC})
set_target_properties(ex3DCharacter PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(ex3DCharacter ce_base ce_o3d)
