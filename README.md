# unv-smi (Universal System Management Interface v 1.0)
Environment independent retrieval of system architecture for parallel programmers. 

# Description 
When optimizing an application for a system, it can be extremely benefecial to know the compute resources availabe to you. This is an attempt to report crucial details of both the hardware, the software, and the general architecture make up, regardless of the operating system & CPU/GPU chip vendors.

# Current Requirements 
C++17 

## Build with GCC
Compile: g++ -Wall -std=c++17 unv-smi-linux.cpp src/execsh.cpp src/parEnv.cpp src/formatter.cpp -o unv-smi  

Run: ./unv-smi

Further details regarding what type of information is returned, along with questions like Who, What, Where, Why, & When can be found in the [Wiki](https://github.com/tommygorham/unv-smi/wiki).

## Quick View of Details

![alma](https://user-images.githubusercontent.com/38857089/182499028-874afbb4-1aa8-4868-95e4-429ae76ddba1.png)
