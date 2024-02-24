# https://www.sqlite.org/compile.html
add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
add_definitions(-DSQLITE_ENABLE_ICU)
add_definitions(-DSQLITE_ENABLE_RBU)
add_definitions(-DSQLITE_ENABLE_STAT4)

add_compile_definitions(SQLITE_DEFAULT_AUTOVACUUM=1)
add_compile_definitions(SQLITE_DEFAULT_FOREIGN_KEYS=1)

target_sources(showmis PRIVATE 
	sqlite3.h 
	sqlite3.c 
	sqlite3ext.h
)

target_sources(sd19test PRIVATE 
	sqlite3.h 
	sqlite3.c 
	sqlite3ext.h
)