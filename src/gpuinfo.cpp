#include "../include/gpuinfo.h" 
#include <string> 

std::string gpuProgModel(std::string gpu){
	std::string ret; 
    // Display the GPU vendor and how to program it 
        if ( gpu.find("AMD") != std::string::npos){
        	ret = "GPU Programming Model: HIP is the standard programming model for AMD accelerators"; 
        	return ret;  /* GPU found */ 
    	}
    	else if  (gpu.find("Intel") != std::string::npos){
        	ret = "GPU Programming Model: OpenCL is the standard programming model for Intel accelerators"; 
        	return ret;  /* GPU found */ 
    	}
    	else if (gpu.find("NVIDIA") != std::string::npos || gpu.find("Matrox") != std::string::npos){
        	ret = "GPU Programming Model: CUDA is the standard programming model for NVIDIA accelerators"; 
        	return ret; /* GPU found */ 
    	}
    	else if (gpu.find("Microsoft") != std::string::npos){
		ret = "GPU Programming Model: You may have integrated graphics, try using SYCL or OpenCL or Direct3D"; 
	    	return ret; 
	}
	// For GPU not found, TODO: Make this not one line so its easy to read on github
    	else {
		ret = "Cannot determine the programming model for the GPU vendor..."; 
	}
	return ret; 
}
