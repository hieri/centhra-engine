set(VERSION_MAJOR 0)
set(VERSION_MINOR 03)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/App.h
	${INC_DIR}/Audio.h
	${INC_DIR}/Base.h
	${INC_DIR}/Color.h
	${INC_DIR}/ConfigBase.h
	${INC_DIR}/Font.h
	${INC_DIR}/Group.h
	${INC_DIR}/Image.h
	${INC_DIR}/Matrix4x4.h
	${INC_DIR}/Plugin.h
	${INC_DIR}/Quaternion.h
	${INC_DIR}/Socket.h
	${INC_DIR}/Thread.h
	${INC_DIR}/Vector2.h
	${INC_DIR}/Vector3.h
)

set(SRC
	${SRC_DIR}/App.cpp
	${SRC_DIR}/Audio.cpp
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/Font.cpp
	${SRC_DIR}/Group.cpp
	${SRC_DIR}/Image.cpp
	${SRC_DIR}/Plugin.cpp
	${SRC_DIR}/Socket.cpp
	${SRC_DIR}/Thread.cpp
)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPOSIXTHREAD ${CE_BASE_USEPOSIXTHREAD})
	set(BASE_USEPOSIXSOCKET ${CE_BASE_USEPOSIXSOCKET})
	if(${BASE_USEPOSIXTHREAD})
		set(LIB ${LIB} pthread)
	endif(${BASE_USEPOSIXTHREAD})
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPOSIXTHREAD 0)
	set(BASE_USEPOSIXSOCKET 0)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(INCLUDE ${INCLUDE} ${PROJECT_SOURCE_DIR}/external/OpenGL/include)
	set(BASE_USEWINTHREAD ${CE_BASE_USEWINTHREAD})
	set(BASE_USEWINSOCKET ${CE_BASE_USEWINSOCKET})
	if(${BASE_USEWINSOCKET})
		set(LIB ${LIB} ws2_32)
	endif(${BASE_USEWINSOCKET})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(BASE_USEWINTHREAD "0")
	set(BASE_USEWINSOCKET "0")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

find_package(DevIL REQUIRED)
set(LIB ${LIB} ${IL_LIBRARIES} ${ILU_LIBRARIES})
set(INCLUDE ${INCLUDE} ${IL_INCLUDE_DIR}/..)

find_package(Freetype REQUIRED)
set(LIB ${LIB} ${FREETYPE_LIBRARIES})
set(INCLUDE ${INCLUDE} ${FREETYPE_INCLUDE_DIRS})

find_package(OpenAL REQUIRED)
set(LIB ${LIB} ${OPENAL_LIBRARY})
set(INCLUDE ${INCLUDE} ${OPENAL_INCLUDE_DIR})

configure_file(${INC_DIR}/ConfigBase.h.in ${INC_DIR}/ConfigBase.h)

include_directories(${INCLUDE})
set(NAME "ce_base")
add_library(${NAME} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX -d)
target_link_libraries(${NAME} ${LIB})
