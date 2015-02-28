set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_FRONTEND_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS})
set(LIB ce_base ce_frontend ce_game2d ce_ui)

ce_sample("500_2D_Collision" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "${LIB}")
