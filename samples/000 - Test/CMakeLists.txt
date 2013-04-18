set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/main.cpp
)

set(INCLUDE
	${PROJECT_SOURCE_DIR}/modules/ce_base/include
	${PROJECT_SOURCE_DIR}/modules/ce_frontend/include
	${PROJECT_SOURCE_DIR}/modules/ce_game2d/include
	${PROJECT_SOURCE_DIR}/modules/ce_ui/include
)

find_package(OpenGL REQUIRED)

set(LIB
	ce_base
	ce_frontend
	ce_game2d
	ce_ui
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

set(NAME "000_Test")
add_executable(${NAME} ${SRC})
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/samples/bin)
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/samples/bin)
set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/samples/bin)
target_link_libraries(${NAME} ${LIB})
