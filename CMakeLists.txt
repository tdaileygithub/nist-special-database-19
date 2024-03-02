# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.13.0)
project(sd19)

option(DR_MEMORY_LEAK_CHECK "Dr Memory Leak Check Build" OFF)

add_executable(showmis)
target_link_libraries(showmis PUBLIC toojpeg sqlite3 sha256 ulog miniz pugixml sd19 sd19db loadpngg fann )
target_sources(showmis PRIVATE "showmis.cpp" "sd19config.h" "sd19config.cpp" "misprocessing.h" "misprocessing.cpp" "hsfpageprocessing.h" "hsfpageprocessing.cpp" )
 
add_executable(sd19test)
target_link_libraries(sd19test PUBLIC toojpeg sqlite3 sha256 ulog miniz pugixml sd19 sd19db loadpngg fann )
target_sources(sd19test PRIVATE "sd19test.cpp" )

add_executable(fanntrain)
target_link_libraries(fanntrain PRIVATE
    d3d12.lib dxgi.lib dxguid.lib uuid.lib
    kernel32.lib user32.lib
    comdlg32.lib advapi32.lib shell32.lib
    ole32.lib oleaut32.lib
) 
target_link_libraries(fanntrain PUBLIC ulog pugixml fann imgui flatbuff fann sqlite3 sd19db loadpngg )
target_sources(fanntrain PRIVATE "mnist_fann_extern.h" "mnist_fann.cpp" "fann.cpp" "sd19config.h" "sd19config.cpp" "imgui_util.h" "imgui_util.cpp"  )

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
add_subdirectory ("mongoose")
add_subdirectory ("fann")
add_subdirectory ("imgui")
add_subdirectory ("flatbuffers") 
add_subdirectory ("lodepng") 

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()