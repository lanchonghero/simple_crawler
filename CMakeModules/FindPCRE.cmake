# Lan Chong<lanchonghero@163.com>
# Find the PCRE includes and library
#
#  PCRE_INCLUDE_DIR - where to find pcre.h, etc.
#  PCRE_LIBRARIES   - List of libraries when using PCRE.
#  PCRE_FOUND       - True if PCRE found.

IF (PCRE_INCLUDE_DIR)
  SET(PCRE_FIND_QUIETLY TRUE) # be silent if already in cache
ENDIF (PCRE_INCLUDE_DIR)

FIND_PATH(PCRE_INCLUDE_DIR pcre.h
  /usr/local/include/pcre
  /usr/include/pcre
  /usr/include
)

SET(PCRE_NAMES pcre)
FIND_LIBRARY(PCRE_LIBRARY
  NAMES ${PCRE_NAMES}
  PATHS /usr/lib /usr/local/lib
  PATH_SUFFIXES pcre
)

IF (PCRE_INCLUDE_DIR AND PCRE_LIBRARY)
  SET(PCRE_FOUND TRUE)
  SET( PCRE_LIBRARIES ${PCRE_LIBRARY} )
ELSE (PCRE_INCLUDE_DIR AND PCRE_LIBRARY)
  SET(PCRE_FOUND FALSE)
  SET( PCRE_LIBRARIES )
ENDIF (PCRE_INCLUDE_DIR AND PCRE_LIBRARY)

IF (PCRE_FOUND)
  IF (NOT PCRE_FIND_QUIETLY)
    MESSAGE(STATUS "Found PCRE: ${PCRE_LIBRARY}")
  ENDIF (NOT PCRE_FIND_QUIETLY)
ELSE (PCRE_FOUND)
  IF (PCRE_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for PCRE libraries named ${PCRE_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find PCRE library")
  ENDIF (PCRE_FIND_REQUIRED)
ENDIF (PCRE_FOUND)

MARK_AS_ADVANCED(
  PCRE_LIBRARY
  PCRE_INCLUDE_DIR
)
