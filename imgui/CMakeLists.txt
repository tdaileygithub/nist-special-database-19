cmake_minimum_required(VERSION 3.13.0)

project(imgui)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.cpp)

add_library(imgui ${SOURCES} ${HEADERS})