set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/CE)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(INC
	${INC_DIR}/App.h
	${INC_DIR}/Audio.h
	${INC_DIR}/Base.h
	${INC_DIR}/Color.h
	${INC_DIR}/ConfigBase.h
	${INC_DIR}/FloatTable.h
	${INC_DIR}/Font.h
	${INC_DIR}/Group.h
	${INC_DIR}/Math.h
	${INC_DIR}/Image.h
	${INC_DIR}/Matrix4x4.h
	${INC_DIR}/Mutex.h
	${INC_DIR}/Network.h
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
	${SRC_DIR}/FloatTable.cpp
	${SRC_DIR}/Font.cpp
	${SRC_DIR}/Group.cpp
	${SRC_DIR}/Math.cpp
	${SRC_DIR}/Image.cpp
	${SRC_DIR}/Mutex.cpp
	${SRC_DIR}/Network.cpp
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
		list(APPEND LIB pthread)
	endif(${BASE_USEPOSIXTHREAD})
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BASE_USEPOSIXTHREAD 0)
	set(BASE_USEPOSIXSOCKET 0)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(BASE_USEWINTHREAD ${CE_BASE_USEWINTHREAD})
	set(BASE_USEWINSOCKET ${CE_BASE_USEWINSOCKET})
	if(${BASE_USEWINSOCKET})
		list(APPEND LIB ws2_32)
	endif(${BASE_USEWINSOCKET})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(BASE_USEWINTHREAD "0")
	set(BASE_USEWINSOCKET "0")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# DevIL
if(NOT IL_FOUND)
	find_package(DevIL)
	if(NOT IL_FOUND)
		if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
			message("Unable to find DevIL with default paths.")
			set(T_IL_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/DevIL-SDK-x86-1.7.8.zip")

			if(NOT EXISTS ${T_IL_DOWNLOAD_FILE})
				message("Downloading DevIL SDK...")
				set(T_IL_URL "http://downloads.sourceforge.net/project/openil/DevIL%20Windows%20SDK/1.7.8/DevIL-SDK-x86-1.7.8.zip")
				file(DOWNLOAD "${T_IL_URL}" "${T_IL_DOWNLOAD_FILE}")
			endif(NOT EXISTS ${T_IL_DOWNLOAD_FILE})

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/devil/README)
				message("Unzipping DevIL SDK...")
				set(T_IL_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/devil")
				execute_process(COMMAND ${PROJECT_SOURCE_DIR}/external/unzip.bat ${T_IL_DOWNLOAD_FILE} ${T_IL_EXTRACT_PATH})

				execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/devil/lib/DevIL.dll ${PROJECT_SOURCE_DIR}/samples/bin/DevIL.dll)
				execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/devil/lib/ILU.dll ${PROJECT_SOURCE_DIR}/samples/bin/ILU.dll)
				execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/devil/lib/ILUT.dll ${PROJECT_SOURCE_DIR}/samples/bin/ILUT.dll)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/devil/README)

			message("Adding DevIL external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/devil)
			find_package(DevIL REQUIRED)
		endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

		if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			message("Unable to find DevIL with default paths.")

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/devil/README)
				set(T_IL_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/DevIL-1.7.8.tar.gz")

				if(NOT EXISTS ${T_IL_DOWNLOAD_FILE})
					message("Downloading DevIL source...")
					set(T_IL_URL "http://downloads.sourceforge.net/project/openil/DevIL/1.7.8/DevIL-1.7.8.tar.gz")
					file(DOWNLOAD "${T_IL_URL}" "${T_IL_DOWNLOAD_FILE}")
				endif(NOT EXISTS ${T_IL_DOWNLOAD_FILE})

				message("Unzipping DevIL source...")
				set(T_IL_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/devil")
				execute_process(COMMAND cmake -E tar jxf ${T_IL_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/devil-1.7.8 ${T_IL_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/devil/README)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/zlib/README)
				set(T_Z_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/zlib-1.2.8.tar.gz")

				if(NOT EXISTS ${T_Z_DOWNLOAD_FILE})
					message("Downloading zlib source...")
					set(T_Z_URL "http://zlib.net/zlib-1.2.8.tar.gz")
					file(DOWNLOAD "${T_Z_URL}" "${T_Z_DOWNLOAD_FILE}")
				endif(NOT EXISTS ${T_Z_DOWNLOAD_FILE})

				message("Unzipping zlib source...")
				set(T_Z_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/zlib")
				execute_process(COMMAND cmake -E tar jxf ${T_Z_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/zlib-1.2.8 ${T_Z_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/zlib/README)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/zlib/libz.a)
				message("Building zlib...")
				execute_process(COMMAND ./configure --static WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/zlib)
				execute_process(COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/zlib)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/zlib/libz.a)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libpng/README)
				set(T_PNG_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/libpng-1.2.51.tar.gz")

				if(NOT EXISTS ${T_PNG_DOWNLOAD_FILE})
					message("Downloading libpng source...")
					set(T_PNG_URL "http://downloads.sourceforge.net/project/libpng/libpng12/1.2.51/libpng-1.2.51.tar.gz")
					file(DOWNLOAD "${T_PNG_URL}" "${T_PNG_DOWNLOAD_FILE}")
				endif(NOT EXISTS ${T_PNG_DOWNLOAD_FILE})

				message("Unzipping libpng source...")
				set(T_PNG_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/libpng")
				execute_process(COMMAND cmake -E tar jxf ${T_PNG_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/libpng-1.2.51 ${T_PNG_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libpng/README)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libpng/.libs/libpng.a)
				message("Building libpng...")
				execute_process(COMMAND ./configure --enable-shared=no WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/libpng)
				execute_process(COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/libpng)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libpng/.libs/libpng.a)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libjpeg/README)
				set(T_JPEG_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/jpegsrc.v9a.tar.gz")

				if(NOT EXISTS ${T_JPEG_DOWNLOAD_FILE})
					message("Downloading libjpeg source...")
					set(T_JPEG_URL "http://www.ijg.org/files/jpegsrc.v9a.tar.gz")
					file(DOWNLOAD "${T_JPEG_URL}" "${T_JPEG_DOWNLOAD_FILE}")
				endif(NOT EXISTS ${T_JPEG_DOWNLOAD_FILE})

				message("Unzipping libjpeg source...")
				set(T_JPEG_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/libjpeg")
				execute_process(COMMAND cmake -E tar jxf ${T_JPEG_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/jpeg-9a ${T_JPEG_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libjpeg/README)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libjpeg/.libs/libjpeg.a)
				message("Building libjpeg...")
				execute_process(COMMAND ./configure --enable-shared=no WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/libjpeg)
				execute_process(COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/libjpeg)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/libjpeg/.libs/libjpeg.a)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/devil/lib/.libs/libIL.a)
				message("Building devil...")
				set(ENV{LDFLAGS} "-L${PROJECT_SOURCE_DIR}/external/libjpeg/.libs -L${PROJECT_SOURCE_DIR}/external/libpng/.libs -L${PROJECT_SOURCE_DIR}/external/zlib")
				set(ENV{CPPFLAGS} "-I${PROJECT_SOURCE_DIR}/external/libjpeg -I${PROJECT_SOURCE_DIR}/external/libpng -I${PROJECT_SOURCE_DIR}/external/zlib")
				set(ENV{LIBS} "-lstdc++")
				execute_process(COMMAND ./configure --disable-shared --enable-ILU=yes --enable-ILUT=yes --enable-zlib=yes --enable-exr=no --enable-mng=no --enable-jp2=no --enable-tiff=no WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/devil)
				set(ENV{LDFLAGS})
				set(ENV{CPPFLAGS})
				set(ENV{LIBS})
				execute_process(COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/devil)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/devil/lib/.libs/libIL.a)

			message("Adding DevIL external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/devil ${PROJECT_SOURCE_DIR}/external/devil/lib/.libs)
			find_package(DevIL REQUIRED)

			if(IL_FOUND)
				set(FULL_IL_LIBRARIES ${IL_LIBRARIES} ${PROJECT_SOURCE_DIR}/external/libpng/.libs/libpng.a ${PROJECT_SOURCE_DIR}/external/libjpeg/.libs/libjpeg.a ${PROJECT_SOURCE_DIR}/external/zlib/libz.a)
				unset(IL_LIBRARIES CACHE)
				set(IL_LIBRARIES ${FULL_IL_LIBRARIES} CACHE FILEPATH "The file that corresponds to the base il library.")
				unset(FULL_IL_LIBRARIES)
			endif(IL_FOUND)
		endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	endif(NOT IL_FOUND)
endif(NOT IL_FOUND)
list(APPEND LIB ${IL_LIBRARIES} ${ILU_LIBRARIES})
list(APPEND INCLUDE ${IL_INCLUDE_DIR}/..)

# FreeType
if(NOT FREETYPE_FOUND)
	find_package(Freetype)
	if(NOT FREETYPE_FOUND)
		if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
			message("Unable to find FreeType with default paths.")
			set(T_FT_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/freetype-2.5.3.tar.bz2")

			if(NOT EXISTS ${T_FT_DOWNLOAD_FILE})
				message("Downloading FreeType source...")
				set(T_FT_URL "http://downloads.sourceforge.net/project/freetype/freetype2/2.5.3/freetype-2.5.3.tar.bz2")
				file(DOWNLOAD "${T_FT_URL}" "${T_FT_DOWNLOAD_FILE}")
			endif(NOT EXISTS ${T_FT_DOWNLOAD_FILE})

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/README)
				message("Unzipping FreeType source...")
				set(T_FT_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/freetype")
				execute_process(COMMAND cmake -E tar jxf ${T_FT_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/freetype-2.5.3 ${T_FT_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/README)

			if(MINGW)
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/libfreetype.a)
					if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/CMakeCache.txt)
						message("Generating FreeType CMake files...")
						execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
					endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/CMakeCache.txt)

					message("Building FreeType...")
					execute_process(COMMAND cmake --build . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
				endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/libfreetype.a)
			else()
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype.lib)
					if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/CMakeCache.txt)
						message("Generating FreeType CMake files...")
						execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
					endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/CMakeCache.txt)

					message("Building FreeType...")
					execute_process(COMMAND cmake --build . --config Release WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
					execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/freetype/Release/freetype.lib ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype.lib)
					execute_process(COMMAND cmake --build . --config Debug WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
					execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/freetype/Debug/freetype.lib ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype_D.lib)
				endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/lib/freetype.lib)
			endif()

			message("Adding FreeType external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/freetype)
			find_package(Freetype REQUIRED)
		endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

		if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			message("Unable to find FreeType with default paths.")
			set(T_FT_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/freetype-2.5.3.tar.bz2")

			if(NOT EXISTS ${T_FT_DOWNLOAD_FILE})
				message("Downloading FreeType source...")
				set(T_FT_URL "http://downloads.sourceforge.net/project/freetype/freetype2/2.5.3/freetype-2.5.3.tar.bz2")
				file(DOWNLOAD "${T_FT_URL}" "${T_FT_DOWNLOAD_FILE}")
			endif(NOT EXISTS ${T_FT_DOWNLOAD_FILE})

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/README)
				message("Unzipping FreeType source...")
				set(T_FT_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/freetype")
				execute_process(COMMAND cmake -E tar jxf ${T_FT_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/freetype-2.5.3 ${T_FT_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/README)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/libfreetype.a)
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/CMakeCache.txt)
					message("Generating FreeType CMake files...")
					execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
				endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/CMakeCache.txt)

				message("Building FreeType...")
				execute_process(COMMAND cmake --build . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/freetype)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/freetype/libfreetype.a)

			message("Adding FreeType external prefix.")
			unset(FREETYPE_INCLUDE_DIR_freetype2 CACHE)
			set(FREETYPE_INCLUDE_DIR_freetype2 ${PROJECT_SOURCE_DIR}/external/freetype/include CACHE FILEPATH "")
			message("FT: ${FREETYPE_INCLUDE_DIR_freetype2}")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/freetype)
			find_package(Freetype REQUIRED)
		endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	endif(NOT FREETYPE_FOUND)
endif(NOT FREETYPE_FOUND)
list(APPEND LIB ${FREETYPE_LIBRARIES})
list(APPEND INCLUDE ${FREETYPE_INCLUDE_DIRS})

# OpenAL
if(NOT OPENAL_FOUND)
	find_package(OpenAL)
	if(NOT OPENAL_FOUND)
		if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
			message("Unable to find OpenAL with default paths.")
			set(T_AL_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/openal-soft-1.15.1-bin.zip")

			if(NOT EXISTS ${T_AL_DOWNLOAD_FILE})
				message("Downloading OpenAL Soft SDK...")
				set(T_AL_URL "http://kcat.strangesoft.net/openal-soft-1.15.1-bin.zip")
				file(DOWNLOAD "${T_AL_URL}" "${T_AL_DOWNLOAD_FILE}")
			endif(NOT EXISTS ${T_AL_DOWNLOAD_FILE})

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/readme.txt)
				message("Unzipping OpenAL Soft SDK...")
				set(T_AL_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/openal-soft")
				execute_process(COMMAND ${PROJECT_SOURCE_DIR}/external/unzip.bat ${T_AL_DOWNLOAD_FILE} ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/openal-soft-1.15.1-bin ${T_AL_EXTRACT_PATH})
				execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/openal-soft/lib/Win32/libOpenAL32.dll.a ${PROJECT_SOURCE_DIR}/external/openal-soft/lib/Win32/OpenAL32.lib)
				execute_process(COMMAND cmake -E copy ${PROJECT_SOURCE_DIR}/external/openal-soft/Win32/soft_oal.dll ${PROJECT_SOURCE_DIR}/samples/bin/OpenAL32.dll)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/readme.txt)

			message("Adding OpenAL Soft external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/openal-soft ${PROJECT_SOURCE_DIR}/external/openal-soft/lib/Win32)
			find_package(OpenAL REQUIRED)
		endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

		if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			message("Unable to find OpenAL with default paths.")
			set(T_AL_DOWNLOAD_FILE "${PROJECT_SOURCE_DIR}/external/openal-soft-1.15.1.tar.bz2")

			if(NOT EXISTS ${T_AL_DOWNLOAD_FILE})
				message("Downloading OpenAL Soft SDK...")
				set(T_AL_URL "http://kcat.strangesoft.net/openal-releases/openal-soft-1.15.1.tar.bz2")
				file(DOWNLOAD "${T_AL_URL}" "${T_AL_DOWNLOAD_FILE}")
			endif(NOT EXISTS ${T_AL_DOWNLOAD_FILE})

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/README)
				message("Unzipping OpenAL Soft SDK...")
				set(T_AL_EXTRACT_PATH "${PROJECT_SOURCE_DIR}/external/openal-soft")
				execute_process(COMMAND cmake -E tar jxf ${T_AL_DOWNLOAD_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)
				execute_process(COMMAND cmake -E rename ${PROJECT_SOURCE_DIR}/external/openal-soft-1.15.1 ${T_AL_EXTRACT_PATH})
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/README)

			if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/libopenal.so)
				if(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/CMakeCache.txt)
					message("Generating OpenAL Soft CMake files...")
					execute_process(COMMAND cmake -G "${CMAKE_GENERATOR}" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/openal-soft)
				endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/CMakeCache.txt)

				message("Building OpenAL Soft...")
				execute_process(COMMAND cmake --build . WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/openal-soft)
			endif(NOT EXISTS ${PROJECT_SOURCE_DIR}/external/openal-soft/libopenal.so)

			message("Adding OpenAL Soft external prefix.")
			set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJECT_SOURCE_DIR}/external/openal-soft)
			set(OPENALDIR ${PROJECT_SOURCE_DIR}/external/openal-soft)
			find_package(OpenAL REQUIRED)
		endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	endif(NOT OPENAL_FOUND)
endif(NOT OPENAL_FOUND)
list(APPEND LIB ${OPENAL_LIBRARY})
list(APPEND INCLUDE ${OPENAL_INCLUDE_DIR})

configure_file(${INC_DIR}/ConfigBase.h.in ${INC_DIR}/ConfigBase.h)

list(REMOVE_DUPLICATES INCLUDE)
list(REMOVE_DUPLICATES LIB)

set(TARGET "ce_base")
add_library(${TARGET} STATIC ${SRC} ${INC} ${RES})
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPTU_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib)
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX -d)
target_include_directories(${TARGET} PUBLIC ${INCLUDE})
target_link_libraries(${TARGET} ${LIB})
set(CE_BASE_INCLUDE_DIRS ${INCLUDE} CACHE PATH "ce_base include directories.")
