set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE ${CE_BASE_INCLUDE_DIRS})
set(LIB ce_base)

ce_sample("200_Frontend_App" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "${LIB}")
