#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIRS - the Assimp include directories
#  ASSIMP_LIBRARIES - link these to use Assimp

FIND_PATH(ASSIMP_INCLUDE_DIR version.h
  HINTS
  ${ASSIMP_HOME}
  ${ENV_ASSIMP_HOME}
  PATH_SUFFIXES assimp
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(ASSIMP_LIBRARY_TEMP
  NAMES assimp
  HINTS
  ${ASSIMP_HOME}
  ${ENV_ASSIMP_HOME}
  PATH_SUFFIXES lib64 lib
  PATHS
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(ASSIMP_FOUND "NO")
IF(ASSIMP_LIBRARY_TEMP)

  SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY_TEMP} CACHE STRING "Where the Assimp Library can be found")
  # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
  SET(ASSIMP_LIBRARY_TEMP "${ASSIMP_LIBRARY_TEMP}" CACHE INTERNAL "")

  SET(ASSIMP_FOUND "YES")
ENDIF(ASSIMP_LIBRARY_TEMP)
