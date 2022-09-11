# unv-smi (Universal System Management Interface v 1.1)
An instantaneous C++17 retrieval of system architecture including availabe hardware resources, and parallel
programming heuristics.

## Description 
When optimizing a software application for a specific machine, I have found it can be extremely beneficial to know, ahead
of time, the compute resources that are availabe to you for parallel exploitation and other optimization techniques. 

This program aims to report crucial details of both the hardware, the software, and the general architecture make up
through executing a simple, single source, c++17 program. 

## Why is this useful? 
unv-smi works as long as you have a relatively modern c++ compiler. 

E.g., Progably doesn't work with GCC4, see (tested compilers)

This means, regardless of the operating system & and the various other hardware/software restraints that come about when trying to  
reveal useful specs about a system, you can identify for instance, the preferred programming model for your GPU(s).  

This includes complex computer cluster architectures and laptops with integrated graphics cards.  

## Current Requirements 
### NOTE: This is a C++17 tool 
* Structures bindings, aka decomposition declarations, are used in this code to iterate through an std::map. Thus, a
compiler that supports this C++17 feature is requirement for full functionality of unv-smi.

Prior versions of this code, as you will notice in the [example
output](https://github.com/tommygorham/unv-smi/tree/dev-linux/example-output), did not use this feature, and can be used
for older compilers that only support up to say c++14. 

## Building unv-smi with CMake 

```
mkdir build
cd build 
cmake .. 
make 

``` 

## Preferred C++ Compilers for unv-smi  
unv-smi has been extensively tested with GCC and Clang. 


### Building unv-smi directly with GCC 

``` 
g++ -Wall -std=c++17 unv-smi.cpp src/execsh.cpp src/parEnv.cpp src/formatter.cpp src/gpuinfo.cpp -o unv-smi  
```

### Building unv-smi directly with Clang 

```
clang++ -Wall -std=c++17 unv-smi.cpp src/execsh.cpp src/parEnv.cpp src/formatter.cpp src/gpuinfo.cpp -o unv-smi  
```

## Running the Universal System Management Interface to display system info

```
./unv-smi
```

# Tested Compilers 
GCC 8/9/10/11/12 
Clang 10/11/12/13

Further details regarding what type of information is returned, along with questions like Who, What, Where, Why, & When can be found in the [Wiki](https://github.com/tommygorham/unv-smi/wiki).

## Quick View of Details from version 1.0 
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
