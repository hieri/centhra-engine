set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/ce_base/include ${PROJECT_SOURCE_DIR}/ce_frontend/include ${PROJECT_SOURCE_DIR}/external/include ${SRC_DIR} ${X11_INCLUDE_DIR})

set(LIB ce_base ce_frontend)

set(NAME "000 - Test")
add_executable(${NAME} ${SRC})
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/samples/bin)
target_link_libraries(${NAME} ${LIB})
