set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/Game3D/Entity.h
	${INC_DIR}/Game3D/PhysicalGroup.h
	${INC_DIR}/Game3D/PhysicalObject.h
	${INC_DIR}/Game3D/PhysicsHandler.h
	${INC_DIR}/Game3D/DefaultPhysicsHandler.h
)

set(SRC
	${SRC_DIR}/Entity.cpp
	${SRC_DIR}/PhysicalGroup.cpp
	${SRC_DIR}/PhysicalObject.cpp
	${SRC_DIR}/PhysicsHandler.cpp
	${SRC_DIR}/DefaultPhysicsHandler.cpp
)

configure_file(${INC_DIR}/ConfigGame3D.h.in ${INC_DIR}/ConfigGame3D.h)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_FRONTEND_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_frontend ce_ui)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "ce_game3d")
add_library(${TARGET} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX -d)
include_directories(${TARGET} ${INCLUDE})
target_link_libraries(${TARGET} ${LIB})
add_dependencies(${TARGET} ce_base ce_frontend ce_ui)
set(CE_GAME3D_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_game3d include directories")
