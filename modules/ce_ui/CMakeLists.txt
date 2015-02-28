set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/ConfigUI.h
	${INC_DIR}/UI.h
	${INC_DIR}/UI/Control.h
	${INC_DIR}/UI/ButtonCtrl.h
	${INC_DIR}/UI/ColorCtrl.h
	${INC_DIR}/UI/ImageCtrl.h
	${INC_DIR}/UI/ScrollCtrl.h
	${INC_DIR}/UI/Skin.h
	${INC_DIR}/UI/TextCtrl.h
	${INC_DIR}/UI/TextButtonCtrl.h
	${INC_DIR}/UI/TextDropDownCtrl.h
	${INC_DIR}/UI/TextEditCtrl.h
)

set(SRC
	${SRC_DIR}/Control.cpp
	${SRC_DIR}/ButtonCtrl.cpp
	${SRC_DIR}/ColorCtrl.cpp
	${SRC_DIR}/ImageCtrl.cpp
	${SRC_DIR}/ScrollCtrl.cpp
	${SRC_DIR}/Skin.cpp
	${SRC_DIR}/TextCtrl.cpp
	${SRC_DIR}/TextButtonCtrl.cpp
	${SRC_DIR}/TextDropDownCtrl.cpp
	${SRC_DIR}/TextEditCtrl.cpp
)

configure_file(${INC_DIR}/ConfigUI.h.in ${INC_DIR}/ConfigUI.h)

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB "")

list(APPEND INCLUDE ${CE_BASE_INCLUDE_DIRS})
list(APPEND LIB ce_base)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

ce_library("ce_ui" "${INCLUDE}" "${SRC}" "${INC}" "${RES}" "${LIB}" "ce_base")
set(CE_UI_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_ui include directories")
