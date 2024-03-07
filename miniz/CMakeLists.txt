project(miniz)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(miniz ${SOURCES} ${HEADERS}) 