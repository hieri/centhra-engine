set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS})
set(LIB ce_base ce_game2d ce_ui)

ce_sample("300_User_Interface" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "${LIB}")
