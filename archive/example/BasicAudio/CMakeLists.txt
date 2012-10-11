# BasicAudio Example

# Module Paths
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
	${SRC_DIR}/BasicAudio.cpp
)

include_directories(${SRC_DIR})

set(LIBRARIES)

# Module Project Creation
add_executable(exBasicAudio ${SRC})
set_target_properties(exBasicAudio PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example)
target_link_libraries(exBasicAudio ce_base)

set(LIBRARIES ce_base ${LIBRARIES})
target_link_libraries(exBasicAudio ${LIBRARIES})
