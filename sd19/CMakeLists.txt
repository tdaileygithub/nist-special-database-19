# CMakeList.txt : CMake project for sd19, include source and define
# project specific logic here.
#

#include_directories(${CMAKE_SOURCE_DIR}/sqlite3) 
#add_library(sqlite3 STATIC sqlite3/sqlite3.c sqlite3/sqlite3.h sqlite3/sqlite3ext.h)
#add_executable(sqlite sqlite3/sqlite3.c sqlite3/shell.c sqlite3/sqlite3.h sqlite3/sqlite3ext.h)
#set_target_properties(sqlite PROPERTIES OUTPUT_NAME sqlite3)

add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
add_definitions(-DSQLITE_ENABLE_JSON1)
add_definitions(-DSQLITE_ENABLE_RBU)
add_definitions(-DSQLITE_ENABLE_STAT4)

# Uncomment this for single-threaded variant (faster)
add_definitions(-DSQLITE_THREADSAFE=0)

# Add source to this project's executable.
add_executable (
showmis
"showmis.cpp" 
"ihead.h" 
"mis.h" 
"fatalerr.cpp" 
"fatalerr.h" 
"syserr.cpp" 
"syserr.h" 
"readmis.cpp"
 "readmis.h" 
 "readrast.cpp" 
 "readrast.h" 
 "readihdr.cpp" 
 "readihdr.h" 
 "getnset.cpp"
 "getnset.h" 
 "defs.h" 
 "validcmp.h" 
 "validcmp.cpp" 
 "invbytes.cpp" 
 "invbytes.h" 
 "imagutil.cpp" 
 "imagutil.h" 
 "grp4decomp.h" 
 "grp4decomp.cpp" 
 "time.cpp" 
 "bits2bytes.cpp" 
 "bits2bytes.h" 
 "freemis.cpp" 
 "freemis.h"
 "toojpeg.cpp"
 "toojpeg.h"

 "mfs.h" "normalize.cpp" "normalize.h" "bytes2bits.cpp" "byte2bit.h" "allocmis.cpp" "allocmis.h" "mishdr.cpp" "mishdr.h" "nullihdr.cpp" 
 "nullihdr.h" "strppath.cpp" "strppath.h" "bincopy.cpp" "bincopy.h" "bitmasks.h" "bitmasks.cpp" "morphchr.cpp" "morphchr.h" "zoom.cpp" "zoom.h" 
 "imageops.cpp" "imageops.h" "char2bin.cpp" "char2bin.h" "masks.cpp" "masks.h" "bits.h" "binops.h" "binfill.cpp" "binfill.h" "chrpixel.cpp" 
 "chrpixel.h" "corners.cpp" "corners.h" "histgram.cpp" "histgram.h" "thres.cpp" "thres.h" "newext.cpp" "newext.h" "writemis.cpp" "writemis.h" "writrast.cpp" "writrast.h" "writhdr.cpp" "writhdr.h" "grp4comp.h" "grp4comp.cpp" "shear.cpp" "shear.h"

 "sqlite3.c" 
 "sqlite3.h" 
 "sqlite3ext.h"
 

 ) 

 #"mfs.h" "normalize.cpp" "normalize.h" "bytes2bits.cpp" "byte2bit.h" "allocmis.cpp" "allocmis.h" "mishdr.cpp" "mishdr.h" "nullihdr.cpp" 
 #"nullihdr.h" "strppath.cpp" "strppath.h" "bincopy.cpp" "bincopy.h" "bitmasks.h" "bitmasks.cpp" "morphchr.cpp" "morphchr.h" "zoom.cpp" "zoom.h" 
 #"imageops.cpp" "imageops.h" "char2bin.cpp" "char2bin.h" "masks.cpp" "masks.h" "bits.h" "binops.h" "binfill.cpp" "binfill.h" "chrpixel.cpp" 
 #"chrpixel.h" "corners.cpp" "corners.h" "histgram.cpp" "histgram.h" "thres.cpp" "thres.h" "newext.cpp" "newext.h" "writemis.cpp" "writemis.h" "writrast.cpp" "writrast.h" "writhdr.cpp" "writhdr.h" "grp4comp.h" "grp4comp.cpp" "shear.cpp" "shear.h"


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET showmis PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
