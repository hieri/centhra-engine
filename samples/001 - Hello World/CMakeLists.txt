set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/ce_base/include ${SRC_DIR})

set(NAME "001 - Hello World")
add_executable(${NAME} ${SRC})
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/samples/bin)
target_link_libraries(${NAME} ce_base)
