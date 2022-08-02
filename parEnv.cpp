#include "parEnv.h" 
#include "execsh.h" 

std::string detectCppStl() // Determine C++ version based on macro 
{ 
	switch (__cplusplus){ /* this is a cross-platform solution assuming you have a C++ compiler */ 
        case 202002L: // 202002L is a preprocessor macro used by the C++20 standard 
            return "C++ Standard: C++20";  
            break; 
        case 201703L: 
            return "C++ Standard: C++17";  
            break;
        case 201402L:
            return "C++ Standard: C++14" ; 
            break; 
        case 201103L: 
            return "C++ Standard: C++11" ; 
            break;
        default: 
            return "C++ version not found"; 
    } 
 } 

std::string detectOmpVersion() // OS-independent way to get the current openmp standard 
{ 
    std::string omp_macro = execsh("echo |cpp -fopenmp -dM |grep -i open"); // store the openmp macro this command returns
    if (omp_macro.find("201511") != std::string::npos) { return "OpenMP Version: 4.5"; } /* hard setting for now */   
    else { return "OpenMP version not found"; }	// for looking up more openmp macros see https://www.openmp.org/specifications/ 
} 
