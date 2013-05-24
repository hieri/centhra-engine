set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/Game2D.h
	${INC_DIR}/UI/GameView2DCtrl.h
	${INC_DIR}/Game2D/Entity.h
	${INC_DIR}/Game2D/Graph.h
	${INC_DIR}/Game2D/Plane.h
	${INC_DIR}/Game2D/Sprite.h
	${INC_DIR}/Game2D/TileMap.h
	${INC_DIR}/Game2D/Zone.h
	${INC_DIR}/Game2D/ZoneCamera.h
	${INC_DIR}/Game2D/ZoneEntity.h
	${INC_DIR}/Game2D/ZoneTrigger.h
)

set(SRC
	${SRC_DIR}/Game2D.cpp
	${SRC_DIR}/GameView2DCtrl.cpp
	${SRC_DIR}/Entity.cpp
	${SRC_DIR}/Graph.cpp
	${SRC_DIR}/Plane.cpp
	${SRC_DIR}/Sprite.cpp
	${SRC_DIR}/TileMap.cpp
	${SRC_DIR}/Zone.cpp
	${SRC_DIR}/ZoneCamera.cpp
	${SRC_DIR}/ZoneEntity.cpp
	${SRC_DIR}/ZoneTrigger.cpp
)

configure_file(include/CE/ConfigGame2D.h.in include/CE/ConfigGame2D.h)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/modules/ce_base/include ${PROJECT_SOURCE_DIR}/modules/ce_frontend/include  ${PROJECT_SOURCE_DIR}/modules/ce_ui/include)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(INCLUDE ${INCLUDE} ${PROJECT_SOURCE_DIR}/external/OpenGL/include)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

include_directories(${INCLUDE})
add_library(ce_game2d STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_game2d PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
