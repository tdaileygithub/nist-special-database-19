project(mongoose)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(mongoose ${SOURCES} ${HEADERS})