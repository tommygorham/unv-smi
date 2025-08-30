# unv-smi (Universal System Management Interface)
This CLI tool identifies some basic specs on any machine, any architecture, with any GPU(s). 

It was created out of curiosity and the innate, vast difference in commands across operating systems that i grew tired of having
to remember. 

**Compatibility:** Heterogeneous HPC clusters or standalone PCs running Linux, Windows, or MacOS.   
**Requirements:** C++ compiler that supports at least C++14.

This CLI tool was used to document results across different systems in [a PhD Dissertation at the University of Tennessee at Chattanooga](https://scholar.utc.edu/theses/788/). 

## Build

```
mkdir bld && cd bld 
cmake .. 
make 
``` 
## Run 

```
./unv-smi
```

### Linux

### MacOS 


### Windows 
<img src="https://github.com/tommygorham/unv-smi/blob/main/example-output/msvc19-windows.png" width="875px" />

[Additional examples](example-output)

[Additional details](https://github.com/tommygorham/unv-smi/wiki)
