PROJECT(simple_crawler)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(
  /usr/include/libxml2/
)

AUX_SOURCE_DIRECTORY(./ CUR_SRCS)

SET(LIBRARIES
  libxml2.so
  libxslt.so
  libpcre.so
  libcurl.so
  libmysqlclient.so
)

ADD_EXECUTABLE(simple_crawler ${CUR_SRCS})
TARGET_LINK_LIBRARIES(simple_crawler ${LIBRARIES})
