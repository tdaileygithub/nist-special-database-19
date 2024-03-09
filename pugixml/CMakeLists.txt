project(pugixml)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(pugixml ${SOURCES} ${HEADERS})