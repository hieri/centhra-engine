# HelloWorld Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/HelloWorld.cpp
)

include_directories(${SRC_DIR})

# Module Project Creation
add_executable(exHelloWorld ${SRC})
set_target_properties(exHelloWorld PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exHelloWorld ce_base)
