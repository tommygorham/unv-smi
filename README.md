# unv-smi (Universal System Management Interface)
**Requirements:** C++ compiler that supports at least C++14.

# Description 
When programming it's often useful to know the compute resources that are availabe to you ahead of time. 
This program reports crucial details of both the hardware, the software, the general architecture, and most
importantly, the GPU(s), through executing a simple, single source, c++ program. 

# Purpose 
Initially, Universal System Management Interface (unv-smi) was created as an alternative to [nvidia-smi](https://developer.nvidia.com/nvidia-system-management-interface) 
for revealing GPU information on systems that do not have Nvidia GPUs. Though it still serves this purpose, unv-smi also reveals 
architectural information that parallel programmers or PhD researchers may find helpful. In 2022, unv-smi was cited in 
[a PhD Dissertation at the University of Tennessee at Chattanooga](https://scholar.utc.edu/theses/788/). This tool was designed 
to work across a wide range of diversified machines, including complex computer cluster architectures with many cores, down to 
single-socket laptops with integrated graphics cards. See the [example-output folder](https://github.com/tommygorham/unv-smi/tree/main/example-output) 
for examples. 


unv-smi works on Linux, Windows, and MacOs. 

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
### Linux unv-smi v1.4 
<img src="example-output/linux-clusters/Rhel8Cuda.png" />

### Windows unv-smi v1.3
<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/ms-windows/msvc19-windows.png" width="875px" />

### Apple unv-smi v1.3
<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/apple-macosx/AppleClang14-macos.png" width="875px" />

## Linux unv-smi v1.2
![version1-2outputexample](example-output/linux-clusters/version1.2(depreceated)/redhatenterprise.png)

## More information 

Additional details can be found in the [Wiki](https://github.com/tommygorham/unv-smi/wiki).
