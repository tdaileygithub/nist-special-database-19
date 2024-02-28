cmake_minimum_required(VERSION 3.13.0)

project(sha256)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(sha256 ${SOURCES} ${HEADERS})