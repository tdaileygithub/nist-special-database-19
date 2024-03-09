project(flatbuff)

add_compile_definitions(FLATBUFFERS_MAX_64_BUFFER_SIZE=99999)
add_compile_definitions(FLATBUFFERS_MAX_BUFFER_SIZE=99999)
add_compile_definitions(FLATBUFFERS_MAX_BUFFER_SIZE=99999)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(flatbuff ${SOURCES} ${HEADERS})