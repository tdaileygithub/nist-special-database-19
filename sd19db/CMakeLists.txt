project(sd19db)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(sd19db ${SOURCES} ${HEADERS})