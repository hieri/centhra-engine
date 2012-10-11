# CE 3D Object Library

# Module Paths
set(INC_DIR ${PROJECT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Module Source
set(INC
	${INC_DIR}/Asset.h
	${INC_DIR}/o3dAsset.h
	${INC_DIR}/o3dBox.h
	${INC_DIR}/o3dCamera.h
	${INC_DIR}/o3dCharacter.h
	${INC_DIR}/o3dEntity.h
	${INC_DIR}/o3dInstance.h
	${INC_DIR}/o3dObject.h
	${INC_DIR}/uiView3d.h
)
set(SRC
	${SRC_DIR}/Asset.cpp
	${SRC_DIR}/o3dAsset.cpp
	${SRC_DIR}/o3dBox.cpp
	${SRC_DIR}/o3dCamera.cpp
	${SRC_DIR}/o3dCharacter.cpp
	${SRC_DIR}/o3dEntity.cpp
	${SRC_DIR}/o3dInstance.cpp
	${SRC_DIR}/o3dObject.cpp
	${SRC_DIR}/o3dInstance.cpp
	${SRC_DIR}/uiView3d.cpp
)
#BULLET_ROOT

# Dependencies
find_package(OpenGL REQUIRED)

if(${MSVC})
	if(CMAKE_SIZEOF_VOID_P MATCHES 8) # x86-64
	else(CMAKE_SIZEOF_VOID_P MATCHES 8) # x86-32
		set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sdl2/include)
		set(BULLET_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/external/bullet/include)
		set(ASSIMP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/assimp/include)

		set(BULLET_LIBRARIES optimized ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/BulletDynamics.lib debug ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/BulletDynamics_debug.lib optimized ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/BulletCollision.lib debug ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/BulletCollision_debug.lib optimized ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/LinearMath.lib debug ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/LinearMath_debug.lib optimized ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/BulletSoftBody.lib debug ${PROJECT_SOURCE_DIR}/external/bullet/lib/x86-32/msvc/BulletSoftBody_debug.lib)
		set(ASSIMP_LIBRARIES optimized ${PROJECT_SOURCE_DIR}/external/assimp/lib/x86-32/msvc/assimp.lib debug ${PROJECT_SOURCE_DIR}/external/assimp/lib/x86-32/msvc/assimp-d.lib)
	endif(CMAKE_SIZEOF_VOID_P MATCHES 8)
else(${MSVC})
	set(SDL2_BUILDING_LIBRARY 1)
	find_package(SDL2 REQUIRED)
	find_package(Bullet REQUIRED)
	find_package(Assimp REQUIRED)
endif(${MSVC})

include_directories(${SRC_DIR} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${BULLET_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})

# Module Project Creation
add_library(ce_o3d STATIC ${INC} ${SRC})
set_target_properties(ce_o3d PROPERTIES DEBUG_POSTFIX -d)
set_target_properties(ce_o3d PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(ce_o3d ce_base ${BULLET_LIBRARIES} ${ASSIMP_LIBRARIES})
