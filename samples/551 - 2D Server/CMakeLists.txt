set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE "")
set(LIB "")

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS} ${CE_PLUGBOX2D_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_game2d ce_plugBox2d)

if(CE_CLIENT)
	list(APPEND INCLUDE ${CE_UI_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/external/opengl/include)
	list(APPEND LIB ce_ui)
endif()

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

ce_sample("551_2D_Server" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "${LIB}")
