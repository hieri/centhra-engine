set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/Game2D.h
	${INC_DIR}/UI/CameraView2DCtrl.h
	${INC_DIR}/UI/Editor2D.h
	${INC_DIR}/Game2D/AppGame2D.h
	${INC_DIR}/Game2D/Camera.h
	${INC_DIR}/Game2D/DefaultPhysicsHandler.h
	${INC_DIR}/Game2D/Entity.h
	${INC_DIR}/Game2D/Explosion.h
	${INC_DIR}/Game2D/Graph.h
	${INC_DIR}/Game2D/PhysicalGroup.h
	${INC_DIR}/Game2D/PhysicalObject.h
	${INC_DIR}/Game2D/PhysicalText.h
	${INC_DIR}/Game2D/PhysicsHandler.h
	${INC_DIR}/Game2D/Projectile.h
	${INC_DIR}/Game2D/Prop.h
	${INC_DIR}/Game2D/TileMap.h
	${INC_DIR}/Game2D/TileSet.h
	${INC_DIR}/Game2D/TiledBackground.h
	${INC_DIR}/Game2D/Trigger.h
	${INC_DIR}/Game2D/Wall.h
	${INC_DIR}/Game2D/World.h
)

set(SRC
	${SRC_DIR}/AppGame2D.cpp
	${SRC_DIR}/Camera.cpp
	${SRC_DIR}/CameraView2DCtrl.cpp
	${SRC_DIR}/DefaultPhysicsHandler.cpp
	${SRC_DIR}/Editor2D.cpp
	${SRC_DIR}/Entity.cpp
	${SRC_DIR}/Explosion.cpp
	${SRC_DIR}/Graph.cpp
	${SRC_DIR}/PhysicalGroup.cpp
	${SRC_DIR}/PhysicalObject.cpp
	${SRC_DIR}/PhysicalText.cpp
	${SRC_DIR}/PhysicsHandler.cpp
	${SRC_DIR}/Projectile.cpp
	${SRC_DIR}/Prop.cpp
	${SRC_DIR}/TileMap.cpp
	${SRC_DIR}/TileSet.cpp
	${SRC_DIR}/TiledBackground.cpp
	${SRC_DIR}/Trigger.cpp
	${SRC_DIR}/Wall.cpp
	${SRC_DIR}/World.cpp
)

configure_file(${INC_DIR}/ConfigGame2D.h.in ${INC_DIR}/ConfigGame2D.h)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS})
list(APPEND LIB ce_base)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(DEPENDS ce_base)
ce_library("ce_game2d" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "${DEPENDS}")
set(CE_GAME2D_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_game2d include directories")
