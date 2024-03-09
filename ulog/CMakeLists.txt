project(ulog)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(ulog ${SOURCES} ${HEADERS})