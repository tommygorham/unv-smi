# unv-smi (Universal System Management Interface v 1.3)
A simplified mechanism for instantaneously identifying your GPU + other system specs on any computer running any OS.   

All you need is a C++ compiler. 

# What's New in Version 1.3 

### Major improvements were made in version 1.3 (September 4, 2023 Release):  

* Lightened restriction on the C++ standard while still utilizing modern C++ 
    - C++14 and up 

* Formatted output across all OS's 
    - Output strings now look uniform across diversified systems

* Improved modularity and portability using best C++ practices 
    - unv-smi has been refactored to implement object-oriented design
    - inheritance/virtual functions, static functions, smart pointers, and additional error handling 
    - Simplified build command when not using CMake 

* Easier to contribute for future development
   - Simplified file structure and CMake, just 4 header files plus unv-smi.cpp 
   - WindowsInfo.h,  LinuxInfo.h, and MacInfo.h all derive from SystemInfo.h 
     thus it is now easier to contribute new code to this repository. 
     E.g., Windows developers only need to edit one file ie (WindowsInfo.h) 

* unv-smi now works on Microsoft Visual Studio out of the box

* unv-smi now reports gpu info on Apple Silicon devices 


# Description 
When optimizing a software application for a specific machine, it's useful to know the compute resources that are availabe to you 
for parallel exploitation and other optimization techniques ahead of time. 

This program aims to report crucial details of both the hardware, the software, and the general architecture make up
through executing a simple, single source, c++ program. 

# Why is this useful? 
This program was created as an alternative to [nvidia-smi](https://developer.nvidia.com/nvidia-system-management-interface). 

All you need is a C++ compiler, no NVIDIA Graphics card is necessary to identify your GPU(s). 

Universal System Management Interface was designed to work across a wide range of diversified machines, including complex computer cluster architectures with many cores, down to single-socket laptops with integrated graphics cards.  

# How to build unv-smi with CMake (preferred) 

```
mkdir build
cd build 
cmake .. 
make 
``` 

# Alternative: How to build unv-smi without CMake  

``` 
g++ -Wall -Iinclude unv-smi.cpp -o unv-smi  
```

# How to Run 

```
./unv-smi
```

# Tested Compilers v1.3 (Fall 2023) 
| Compiler | Versions               |
|----------|-------|
| GCC     | 12    |
| Clang++  | 12    |
| MSVC    | 19    |
| AppleClang | 14 | 

# Version 1.3 Example Output 


## More information 

Further details regarding what type of information is returned can be found in the [Wiki](https://github.com/tommygorham/unv-smi/wiki).