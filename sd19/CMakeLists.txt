target_sources(showmis PRIVATE 
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
 
 "mfs.h" "normalize.cpp" "normalize.h" "bytes2bits.cpp" "byte2bit.h" "allocmis.cpp" "allocmis.h" "mishdr.cpp" "mishdr.h" "nullihdr.cpp" 
 "nullihdr.h" "strppath.cpp" "strppath.h" "bincopy.cpp" "bincopy.h" "bitmasks.h" "bitmasks.cpp" "morphchr.cpp" "morphchr.h" "zoom.cpp" "zoom.h" 
 "imageops.cpp" "imageops.h" "char2bin.cpp" "char2bin.h" "masks.cpp" "masks.h" "bits.h" "binops.h" "binfill.cpp" "binfill.h" "chrpixel.cpp" 
 "chrpixel.h" "corners.cpp" "corners.h" "histgram.cpp" "histgram.h" "thres.cpp" "thres.h" "newext.cpp" "newext.h" "writemis.cpp" "writemis.h" "writrast.cpp" "writrast.h" "writhdr.cpp" "writhdr.h" "grp4comp.h" "grp4comp.cpp" "shear.cpp" "shear.h"
  ) 
    
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET showmis PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
