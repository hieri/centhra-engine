set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS} ${CE_PLUGBOX2D_INCLUDE_DIRS})
set(LIB ce_base ce_game2d ce_plugBox2d)

ce_sample("901_Box2D" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "${LIB}")
