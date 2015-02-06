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

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS} ${CE_FRONTEND_INCLUDE_DIRS} ${CE_GAME2D_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS})
list(APPEND LIB ce_base ce_frontend ce_game2d ce_ui)

if(NOT CE_NOCLIENT)
	list(APPEND INCLUDE ${CE_FRONTEND_INCLUDE_DIRS} ${CE_UI_INCLUDE_DIRS})
	list(APPEND LIB ce_frontend ce_ui)
endif()

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "ce_plugTiled")
add_library(${TARGET} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX -d)
target_include_directories(${TARGET} PUBLIC ${INCLUDE})
target_link_libraries(${TARGET} ${LIB})
add_dependencies(${TARGET} ${LIB})
set(CE_PLUGTILED_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_plugTiled include directories")
