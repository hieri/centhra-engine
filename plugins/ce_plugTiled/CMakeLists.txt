set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/TMX.cpp
	${SRC_DIR}/pugixml.cpp
)
set(INC
	${INC_DIR}/CE/Plugin/Tiled/Base.h
	${INC_DIR}/CE/Plugin/Tiled/TMX.h
	${INC_DIR}/pugiconfig.hpp
	${INC_DIR}/pugixml.hpp
)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_game2d)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

ce_library("ce_plugTiled" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "")
set(CE_PLUGTILED_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_plugTiled include directories")
