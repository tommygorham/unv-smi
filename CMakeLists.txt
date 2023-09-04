cmake_minimum_required(VERSION 3.16) 
project(unv-smi)

include_directories(include) 
set(CMAKE_CXX_FLAGS " -Wall ") 
set(CMAKE_CXX_STANDARD 14) 

if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    add_definitions(-DUSE_APPLECLANG)
endif()

add_executable(unv-smi unv-smi.cpp)  
