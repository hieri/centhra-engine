set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE
	${PROJECT_SOURCE_DIR}/modules/ce_base/include
)

set(LIB
	ce_base
)

include_directories(${INCLUDE})

set(NAME "100_Multithreading")
add_executable(${NAME} ${SRC})
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/samples/bin)
target_link_libraries(${NAME} ${LIB})
