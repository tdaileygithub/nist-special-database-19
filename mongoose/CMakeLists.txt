cmake_minimum_required(VERSION 3.13.0)

project(mongoose)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(mongoose ${SOURCES} ${HEADERS})