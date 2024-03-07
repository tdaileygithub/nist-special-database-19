project(toojpeg)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(toojpeg ${SOURCES} ${HEADERS})