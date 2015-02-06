set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE "")
set(LIB "")

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS} ${CE_PLUGBOX2D_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_game2d ce_plugBox2d)

if(NOT CE_NOCLIENT)
	list(APPEND INCLUDE ${CE_FRONTEND_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/external/opengl/include)
	list(APPEND LIB ce_frontend ce_ui)
endif()

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "551_2D_Server")
add_executable(${TARGET} ${SRC})
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/samples/bin)
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/samples/bin)
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/samples/bin)
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX -d)
target_include_directories(${TARGET} PUBLIC ${INCLUDE})
target_link_libraries(${TARGET} ${LIB})
add_dependencies(${TARGET} ${LIB})
