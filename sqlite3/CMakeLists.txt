cmake_minimum_required(VERSION 3.13.0)

project(sqlite3)

# https://www.sqlite.org/compile.html
add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
add_definitions(-DSQLITE_ENABLE_RBU)
add_definitions(-DSQLITE_ENABLE_STAT4)
add_compile_definitions(SQLITE_DEFAULT_AUTOVACUUM=1)
add_compile_definitions(QLITE_DEFAULT_FOREIGN_KEYS=1)

file(GLOB HEADERS *.h)

file(GLOB SOURCES *.c)

add_library(sqlite3 ${SOURCES} ${HEADERS})