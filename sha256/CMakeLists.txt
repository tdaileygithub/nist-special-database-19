project(sha256)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(sha256 ${SOURCES} ${HEADERS})