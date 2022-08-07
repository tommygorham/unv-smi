# unv-smi (Universal System Management Interface v 1.0)
A modern, environment-independent retrieval of system architecture information for parallel programmers that use C++. 

## Description 
When optimizing an application for a system, it can be extremely beneficial to know the compute resources availabe to you. This is an initial attempt to report crucial details of both the hardware, the software, and the general architecture make up, regardless of the operating system & CPU/GPU chip vendors (amongst other hardware restraints). unv-smi works on both computer clusters and laptops.  

## Current Requirements 
### NOTE: This is a C++ tool 
* **C++17**: due to use of std::string_view and decomposition declarations in this code, C++17 is currently a requirement for full functionality of unv-smi.
However, as you will notice in the [example output](https://github.com/tommygorham/unv-smi/tree/dev-linux/example-output), compiling with C++14 has worked in the past, and I am contemplating making this available again for older c++ versions. 

## Build with CMake 

```
mkdir build
cd build 
cmake .. 
``` 
## Build directly with GCC and c++17 (or newer) 

``` 
g++ -Wall -std=c++17 unv-smi.cpp src/execsh.cpp src/parEnv.cpp src/formatter.cpp src/gpuinfo.cpp -o unv-smi  
```

## Build directly with Clang and c++17 (or newer) 

```
clang++ -Wall -std=c++17 unv-smi.cpp src/execsh.cpp src/parEnv.cpp src/formatter.cpp src/gpuinfo.cpp -o unv-smi  
```

## Run Universal System Management Interface

```
./unv-smi
```

Further details regarding what type of information is returned, along with questions like Who, What, Where, Why, & When can be found in the [Wiki](https://github.com/tommygorham/unv-smi/wiki).

## Quick View of Details
Example Output: Linux Cluster 
![alma](https://user-images.githubusercontent.com/38857089/182499028-874afbb4-1aa8-4868-95e4-429ae76ddba1.png)

Example Output: MacOS Yosemite

<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/apple-macosx/MacOSX_Yosemite.png" height="700px" /> 

Example Output: Windows running Ubuntu via WSL2

<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/wsl2/Ubuntu20.04LTS.png" height="450px" /> 

Example Output: Windows 11

<img src ="https://github.com/tommygorham/unv-smi/blob/main/example-output/ms-windows/WinPro11.png" height="500px" /> 


## One could also implement this in Python 

```python 
import subprocess # module for writing to cmd/shell 

def determineOmpVersion(omp_version_cmd):
    # store the omp macro 
    ret = subprocess.check_output(omp_version_cmd, shell=True) 
    # convert to string 
    omp_version_macro = ret.decode('utf-8')
    # print version via https://www.openmp.org/specifications/
    if (omp_version_macro.__contains__('201511')): 
        print("OpenMP Version: 4.5") 

omp_version_cmd = "echo |cpp -fopenmp -dM |grep -i open" # store the command to get the omp macro
detectOmpVersion(omp_version_cmd) 
```
