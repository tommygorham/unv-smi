# unv-smi (Universal System Management Interface)
A simplified mechanism for instantaneously identifying your GPU + other system specs on any computer running any OS.   

Requirements: C++ compiler that supports at least C++14.

# Description 
When programming it's often useful to know the compute resources that are availabe to you ahead of time. 

This program aims to report crucial details of both the hardware, the software, the general architecture, and most
importantly, the GPU through executing a simple, single source, c++ program. 

# Why is this useful? 
This program was created as an alternative to [nvidia-smi](https://developer.nvidia.com/nvidia-system-management-interface). 
because I couldn't find an equivalent command on an AMD machine. 

All you need is a C++ compiler, no NVIDIA Graphics card is necessary to identify your GPU(s). 

Universal System Management Interface was designed to work across a wide range of diversified machines, including complex computer cluster architectures with many cores, down to single-socket laptops with integrated graphics cards.  

Works with GCC, Clang++, MSVC, and AppleClang compilers and Apple Silicon Macs. 

# Build

```
mkdir build
cd build 
cmake .. 
make 
``` 
# Run 

```
./unv-smi
```

# Example Output 
### Windows PC (MSVC 19) 
<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/ms-windows/msvc19-windows.png" width="875px" />

### Mac mini (AppleClang 14) 
<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/apple-macosx/AppleClang14-macos.png" width="875px" />

### WSL2 (Clang 12) 
<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/wsl2/Clang12-wls2.png" width="875x" />

## More information 

Further details regarding what type of information is returned can be found in the [Wiki](https://github.com/tommygorham/unv-smi/wiki).
