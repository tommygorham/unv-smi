cmake_minimum_required(VERSION 3.14)
project(unv-smi)

# CXX Compiler Settings 
set(CMAKE_CXX_STANDARD 14)
if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  add_definitions(-DUSE_APPLECLANG)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_definitions(CLANG_COMPILER)
endif()

# Build Targets
include_directories(include) # Location of Header files
add_executable(unv-smi unv-smi.cpp)
