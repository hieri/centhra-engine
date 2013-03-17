set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/Game2D.h
	${INC_DIR}/UI/GameView2D.h
	${INC_DIR}/Game2D/Entity.h
	${INC_DIR}/Game2D/Plane.h
	${INC_DIR}/Game2D/TileMap.h
	${INC_DIR}/Game2D/Zone.h
	${INC_DIR}/Game2D/ZoneCamera.h
	${INC_DIR}/Game2D/ZoneEntity.h
)

set(SRC
	${SRC_DIR}/Game2D.cpp
	${SRC_DIR}/GameView2D.cpp
	${SRC_DIR}/Entity.cpp
	${SRC_DIR}/Plane.cpp
	${SRC_DIR}/TileMap.cpp
	${SRC_DIR}/Zone.cpp
	${SRC_DIR}/ZoneCamera.cpp
	${SRC_DIR}/ZoneEntity.cpp
)

configure_file(include/CE/ConfigGame2D.h.in include/CE/ConfigGame2D.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/modules/ce_base/include ${PROJECT_SOURCE_DIR}/modules/ce_ui/include ${PROJECT_SOURCE_DIR}/external/include)
add_library(ce_game2d STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_game2d PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
