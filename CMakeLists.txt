# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.13.0)
project(sd19)

option(DR_MEMORY_LEAK_CHECK "Dr Memory Leak Check Build" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall") 

add_executable(showmis)
target_sources(showmis PRIVATE "showmis.cpp" "sd19config.h" "misprocessing.h" "misprocessing.cpp" "hsfpageprocessing.h" "hsfpageprocessing.cpp" )

add_executable(sd19test)
target_sources(sd19test PRIVATE "sd19test.cpp" )

if(DR_MEMORY_LEAK_CHECK)
  if(MSVC)
    target_compile_options(sd19test PUBLIC /Zi /MT /EHsc /Oy- /Ob0 )
    target_compile_options(showmis  PUBLIC /Zi /MT /EHsc /Oy- /Ob0 )
  endif()
else()
  if(MSVC)
    target_compile_options(sd19test PUBLIC /EHsc /W4 /std:c++latest /02 /Qpar /openmp /fsanitize )
    target_compile_options(showmis  PUBLIC /EHsc /W4 /std:c++latest /02 /Qpar /openmp /fsanitize )
  endif()

# https://learn.microsoft.com/en-us/cpp/parallel/auto-parallelization-and-auto-vectorization?view=msvc-170
# https://devblogs.microsoft.com/cppblog/using-c17-parallel-algorithms-for-better-performance/
# compile with:
#  debug: cl /EHsc /W4 /WX /std:c++latest /Fedebug /MDd .\program.cpp
#  release: cl /EHsc /W4 /WX /std:c++latest /Ferelease /MD /O2 .\program.cpp

endif()

# Include sub-projects.
add_subdirectory ("sd19")
add_subdirectory ("sqlite3")
add_subdirectory ("toojpeg")
add_subdirectory ("doctest")
add_subdirectory ("sd19db")
add_subdirectory ("pugixml")
add_subdirectory ("miniz")
add_subdirectory ("sha256")
add_subdirectory ("ulog")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()