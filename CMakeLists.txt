cmake_minimum_required(VERSION 3.0)
project(minitracer)

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
  add_definitions(-DX86)
elseif ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
  add_definitions(-DAMD64)  
else ()
  message ( FATAL_ERROR "unsupported architecture" )
endif ()

add_executable(test ${sources})
target_compile_options(test PUBLIC -std=c++17 -Wall)

target_include_directories(test PUBLIC src/)

set(CPACK_PACKAGE_NAME "minitracer")
set(CPACK_PACKAGE_VERSION "1.0.0")
