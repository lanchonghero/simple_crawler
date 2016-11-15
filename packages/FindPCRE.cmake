# - Find PCRE
# Find the PCRE includes and library
#
#  PCRE_INCLUDE_DIR - where to find pcre.h, etc.
#  PCRE_LIBRARIES   - List of libraries when using PCRE.
#  PCRE_FOUND       - True if PCRE found.

IF (PCRE_INCLUDE_DIR)
  # Already in cache, be silent
  SET(PCRE_FIND_QUIETLY TRUE)
ENDIF (PCRE_INCLUDE_DIR)
#从下面的路径找pcre.h，将找到的路径放到PCRE_INCLUDE_DIR这个变量中。
FIND_PATH(PCRE_INCLUDE_DIR pcre.h
  /usr/local/include/pcre
  /usr/include/pcre
  /usr/include
)
#设置要寻找的链接库的名字，这里应该是找libpcre.so
SET(PCRE_NAMES pcre)
#将libpcre.so的路径放到PCRE_LIBRARY
FIND_LIBRARY(PCRE_LIBRARY
  NAMES ${PCRE_NAMES}
  PATHS /usr/lib /usr/local/lib
  PATH_SUFFIXES pcre
)

#如果上面找到pcre的头文件和pcre的库文件，就将PCRE_FOUND设为真
IF (PCRE_INCLUDE_DIR AND PCRE_LIBRARY)
  SET(PCRE_FOUND TRUE)
  SET( PCRE_LIBRARIES ${PCRE_LIBRARY} )
ELSE (PCRE_INCLUDE_DIR AND PCRE_LIBRARY)
  SET(PCRE_FOUND FALSE)
  SET( PCRE_LIBRARIES )
ENDIF (PCRE_INCLUDE_DIR AND PCRE_LIBRARY)

#如果找到并且没有设置QUIET，则输出搜到动态库路径
#如果找不到就输出错误信息
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

#将他标记为高级变量，具体什么作用没查到
MARK_AS_ADVANCED(
  PCRE_LIBRARY
  PCRE_INCLUDE_DIR
)
