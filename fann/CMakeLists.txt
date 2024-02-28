cmake_minimum_required(VERSION 3.13.0)

project(fann)

add_definitions(-DFANN_NO_DLL) 
add_definitions(-D_REENTRANT)
add_definitions(-DFANN_DLL_EXPORTS)
#add_definitions(-DDISABLE_PARALLEL_FANN)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp) 

add_library(fann ${SOURCES} ${HEADERS})    