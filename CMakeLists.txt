cmake_minimum_required(VERSION 3.16) 
project(unv-smi)

include_directories(include) 
set(CMAKE_CXX_FLAGS " -Wall -std=c++17 ") 

add_executable(unv-smi unv-smi.cpp src/execsh.cpp src/parEnv.cpp src/formatter.cpp src/gpuinfo.cpp)  
