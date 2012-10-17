set(VERSION_MAJOR 0)
set(VERSION_MINOR 01)

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/App.h
	${INC_DIR}/Base.h
	${INC_DIR}/ConfigBase.h
	${INC_DIR}/Thread.h
)

set(SRC
	${SRC_DIR}/App.cpp
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/Thread.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPTHREAD ${CE_BASE_USEPTHREAD})
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPTHREAD 0)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(BASE_USEWINTHREAD ${CE_BASE_USEWINTHREAD})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(BASE_USEWINTHREAD "0")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

configure_file(include/CE/ConfigBase.h.in include/CE/ConfigBase.h)

set(LIB "")
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(LIB ${LIB} pthread)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ce_base STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_base PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(ce_base ${LIB})
