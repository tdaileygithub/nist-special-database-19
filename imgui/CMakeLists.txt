project(imgui)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(imgui ${SOURCES} ${HEADERS})