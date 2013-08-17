
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/ConfigUI.h
	${INC_DIR}/UI.h
	${INC_DIR}/UI/Control.h
	${INC_DIR}/UI/ButtonCtrl.h
	${INC_DIR}/UI/ColorCtrl.h
	${INC_DIR}/UI/ImageCtrl.h
	${INC_DIR}/UI/TextCtrl.h
	${INC_DIR}/UI/TextEditCtrl.h
)

set(SRC
	${SRC_DIR}/UI.cpp
	${SRC_DIR}/Control.cpp
	${SRC_DIR}/ButtonCtrl.cpp
	${SRC_DIR}/ColorCtrl.cpp
	${SRC_DIR}/ImageCtrl.cpp
	${SRC_DIR}/TextCtrl.cpp
	${SRC_DIR}/TextEditCtrl.cpp
)

configure_file(include/CE/ConfigUI.h.in include/CE/ConfigUI.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/modules/ce_base/include ${PROJECT_SOURCE_DIR}/modules/ce_frontend/include ${PROJECT_SOURCE_DIR}/external/include)
add_library(ce_ui STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_ui PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
