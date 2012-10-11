# CE Base Library

# Module Paths
set(INC_DIR ${PROJECT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Module Source
set(INC
	${INC_DIR}/ActionMap.h
	${INC_DIR}/Base.h
	${INC_DIR}/BufferObject.h
	${INC_DIR}/Canvas.h
	${INC_DIR}/Color.h
	${INC_DIR}/Font.h
	${INC_DIR}/Game.h
	${INC_DIR}/KeyCode.h
	${INC_DIR}/Image.h
	${INC_DIR}/ImageArray.h
	${INC_DIR}/Matrix4x4.h
	${INC_DIR}/Mixer.h
	${INC_DIR}/Quaternion.h
	${INC_DIR}/Rect.h
	${INC_DIR}/Resource_Base.h
	${INC_DIR}/ScanCode.h
	${INC_DIR}/Shader.h
	${INC_DIR}/uiButton.h
	${INC_DIR}/uiEntity.h
	${INC_DIR}/uiImage.h
	${INC_DIR}/uiInstance.h
	${INC_DIR}/uiRect.h
	${INC_DIR}/uiText.h
	${INC_DIR}/uiWindow.h
	${INC_DIR}/Vector2.h
	${INC_DIR}/Vector3.h
)
set(SRC
	${SRC_DIR}/Base.cpp
	${SRC_DIR}/BufferObject.cpp
	${SRC_DIR}/Canvas.cpp
	${SRC_DIR}/Cursor.cpp
	${SRC_DIR}/Font.cpp
	${SRC_DIR}/Game.cpp
	${SRC_DIR}/Image.cpp
	${SRC_DIR}/ImageArray.cpp
	${SRC_DIR}/Mixer.cpp
	${SRC_DIR}/Shader.cpp
	${SRC_DIR}/uiButton.cpp
	${SRC_DIR}/uiEntity.cpp
	${SRC_DIR}/uiImage.cpp
	${SRC_DIR}/uiInstance.cpp
	${SRC_DIR}/uiRect.cpp
	${SRC_DIR}/uiText.cpp
	${SRC_DIR}/uiWindow.cpp
)

# Dependencies
find_package(OpenGL REQUIRED)

set(LIBRARIES)
if(${MSVC})
	if(CMAKE_SIZEOF_VOID_P MATCHES 8) # x86-64
	else(CMAKE_SIZEOF_VOID_P MATCHES 8) # x86-32
		set(SDL2_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sdl2/include)
#		set(SDL2IMAGE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sdl2/include)
		set(SDL2MIXER_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sdl2/include)
		set(SDL2TTF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/sdl2/include)
		set(IL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/devil/include)

		set(SDL2_LIBRARY optimized ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Release/SDL2.lib debug ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Debug/SDL2.lib Winmm.lib Imm32.lib Version.lib)
#		set(SDL2IMAGE_LIBRARY optimized ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Release/SDL2_image.lib debug ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Debug/SDL2_image.lib)
		set(SDL2MIXER_LIBRARY optimized ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Release/SDL2_mixer.lib debug ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Debug/SDL2_mixer.lib)
		set(SDL2TTF_LIBRARY optimized ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Release/SDL2_ttf.lib debug ${PROJECT_SOURCE_DIR}/external/sdl2/lib/x86-32/msvc/Debug/SDL2_ttf.lib)
		set(IL_LIBRARIES ${PROJECT_SOURCE_DIR}/external/devil/lib/x86-32/msvc/DevIL.lib)
		set(ILU_LIBRARIES ${PROJECT_SOURCE_DIR}/external/devil/lib/x86-32/msvc/ILU.lib ${PROJECT_SOURCE_DIR}/external/devil/lib/x86-32/msvc/ILUT.lib)

		set(LIBRARIES optimized ${PROJECT_SOURCE_DIR}/src/base/ce_base.dir/Release/ce_base.res debug ${PROJECT_SOURCE_DIR}/src/base/ce_base.dir/Debug/ce_base.res)
	endif(CMAKE_SIZEOF_VOID_P MATCHES 8)

	set(SRC ${SRC} ${SRC_DIR}/ce_base.rc)
else(${MSVC})
	set(SDL2_BUILDING_LIBRARY 1)
	find_package(SDL2 REQUIRED)
#	find_package(SDL2_image REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(SDL2_ttf REQUIRED)
	find_package(DevIL REQUIRED)
endif(${MSVC})

include_directories(${SRC_DIR} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${IL_INCLUDE_DIR})

set(RES "")
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	if(CMAKE_SIZEOF_VOID_P MATCHES 8) # x86-64
		set(COPYOUTPUT elf64-x86-64)
	else(CMAKE_SIZEOF_VOID_P MATCHES 8) # x86-32
		set(COPYOUTPUT elf32-i386)
	endif(CMAKE_SIZEOF_VOID_P MATCHES 8)

	add_custom_command(OUTPUT FreeMono.ttf.o COMMAND objcopy --input binary --output ${COPYOUTPUT} --binary-architecture i386 FreeMono.ttf FreeMono.ttf.o)
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES FreeMono.ttf.o)

	set(RES ${SRC_DIR}/FreeMono.ttf.o)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Module Project Creation
add_library(ce_base STATIC ${SRC} ${INC} ${RES})
set_target_properties(ce_base PROPERTIES DEBUG_POSTFIX -d)
set_target_properties(ce_base PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(LIBRARIES ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${SDL2MIXER_LIBRARY} ${SDL2TTF_LIBRARY} ${IL_LIBRARIES} ${ILU_LIBRARIES} ${LIBRARIES})
target_link_libraries(ce_base ${LIBRARIES})
