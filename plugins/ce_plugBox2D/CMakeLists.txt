set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/PhysicsHandler.cpp
)
set(INC
	${INC_DIR}/CE/Plugin/Box2D/Base.h
	${INC_DIR}/CE/Plugin/Box2D/PhysicsHandler.h
)

find_package(OpenGL REQUIRED)
find_package(Box2D REQUIRED)

set(INCLUDE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/modules/ce_base/include
	${PROJECT_SOURCE_DIR}/modules/ce_frontend/include
	${PROJECT_SOURCE_DIR}/modules/ce_game2d/include
	${BOX2D_INCLUDE_DIR}
)

set(LIB
	ce_base
	ce_frontend
	ce_game2d
	${OPENGL_LIBRARIES}
	${BOX2D_LIBRARIES}
#	optimized ${BOX2D_LIBRARY_RELEASE}
#	debug ${BOX2D_LIBRARY_DEBUG}
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(X11 REQUIRED)
	set(LIB ${LIB} ${X11_LIBRARIES})
	if(CE_FRONTEND_USEXCB)
		set(LIB ${LIB} ${PROJECT_SOURCE_DIR}/external/lib/libxcb.so ${PROJECT_SOURCE_DIR}/external/lib/libX11-xcb.so)
	endif(CE_FRONTEND_USEXCB)
	set(INCLUDE ${INCLUDE} ${X11_INCLUDE_DIR})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(${INCLUDE})

set(NAME "ce_plugBox2D")
add_library(${NAME} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(${NAME} ${LIB})
