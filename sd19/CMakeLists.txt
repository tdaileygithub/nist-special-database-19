project(sd19)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(sd19 ${SOURCES} ${HEADERS})