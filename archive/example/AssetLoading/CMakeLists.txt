# AssetLoading Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/AssetLoading.cpp
)

include_directories(${SRC_DIR})

# Module Project Creation
add_executable(exAssetLoading ${SRC})
set_target_properties(exAssetLoading PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exAssetLoading ce_base ce_o3d)
