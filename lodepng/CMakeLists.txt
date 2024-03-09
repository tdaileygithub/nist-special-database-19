project(loadpngg)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(loadpngg ${SOURCES} ${HEADERS}) 