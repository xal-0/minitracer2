cmake_minimum_required (VERSION 3.0)
project (minitracer)

option (DEBUG_INFO_DWARF "Enable DWARF debugging info (GCC/clang)")

option (EXE_FORMAT_PE "Enable the PE executable format (Windows)")
option (EXE_FORMAT_ELF "Enable the ELF executable format (Unix)")

if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
  add_definitions (-DX86)
elseif ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
  add_definitions (-DAMD64)
else ()
  message (FATAL_ERROR "unsupported architecture")
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions (-DCLANG)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions (-DGCC)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message (FATAL_ERROR "unsupported architecture")
else ()
  message (FATAL_ERROR "unsupported architecture")
endif ()

set (sources src/minitracer.cpp src/debug/debuginfo.cpp src/debug/byteio.cpp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set (DEBUG_INFO_DWARF ON)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set (DEBUG_INFO_DWARF ON)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set (EXE_FORMAT_PE OFF)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set (EXE_FORMAT_PE ON)
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set (EXE_FORMAT_ELF ON)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set (EXE_FORMAT_ELF OFF)
endif ()

if (${EXE_FORMAT_ELF})
  add_definitions (-DELF)
  set (sources ${sources} src/debug/elf.cpp)
endif ()

if (${EXE_FORMAT_PE})
  add_definitions (-DPE)
  set (sources ${sources} src/debug/pe.cpp)
endif ()

if (${DEBUG_INFO_DWARF})
  add_definitions (-DDWARF)
  set (sources ${sources} src/debug/dwarf.cpp)
endif ()

set(CMAKE_CXX_STANDARD 14)

add_library(minitracer OBJECT ${sources})
set_target_properties(minitracer PROPERTIES VERSION 0.1)

add_library(minitracer-static SHARED $<TARGET_OBJECTS:minitracer>)
add_library(minitracer-shared STATIC $<TARGET_OBJECTS:minitracer>)

target_include_directories(minitracer PUBLIC src/)

set(CPACK_PACKAGE_NAME "minitracer")
set(CPACK_PACKAGE_VERSION "0.0.1")
