set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
	${SRC_DIR}/Base.cpp
)
set(INC
	${INC_DIR}/CE/Plugin/Box2D/Base.h
)

set(INCLUDE
	${PROJECT_SOURCE_DIR}/modules/ce_base/include
	${PROJECT_SOURCE_DIR}/modules/ce_frontend/include
	${PROJECT_SOURCE_DIR}/modules/ce_game2d/include
)

set(LIB
	ce_base
	ce_frontend
	ce_game2d
	${OPENGL_LIBRARIES}
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
add_library(${NAME} STATIC ${SRC} ${INC})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(${NAME} ${LIB})
