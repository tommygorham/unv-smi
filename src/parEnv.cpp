#include "../include/parEnv.h" 
#include "../include/execsh.h" 
// Determine C++ version based on macro 
std::string detectCppStl(){ 
	switch (__cplusplus){ /* this is a cross-platform solution assuming you have a C++ compiler */ 
        case 202002L: // 202002L is a preprocessor macro used by the C++20 standard 
            return "C++ Standard: C++20, 202002";  
            break; 
        case 201709L:
            return "C++ Standard: C++20, GCC not the latest version, 201709L"; 
            break; 
        case 201707L:
            return "C++ Standard: C++20, Clang not the latest version, 201707L"; 
            break; 
        case 201704L: 
            return "C++ Standard: C++20, VC++ Compiler not the latest version, 201704L"; 
            break; 
        case 201703L: 
            return "C++ Standard: C++17, 201703L";  
            break;
        case 201402L:
            return "C++ Standard: C++14,  201402L";  
            break; 
        case 201103L: 
            return "C++ Standard: C++11, 201103L"; 
            break;
        default: 
            return "C++ version not found";  
    } 
 } 

// Unfinished way to get the current openmp standard 
// Breaks if cpp env variable mapped to old compiler 
// more logic needed 
std::string detectOmpVersion() {
std::string omp_macro = execsh("echo |cpp -fopenmp -dM |grep -i open"); // store the openmp macro this command returns
    if(omp_macro.empty()) { return "\nOpenMP version not found. Your cpp environment variable is attached to an old compiler"; } 
	else if (omp_macro.find("201511") != std::string::npos) { return "\nOpenMP Version: 4.5"; }   
    else { return "\nOpenMP version not found, use C++ Version Macro to find information"; }	// for looking up more openmp macros see https://www.openmp.org/specifications/ 
} 

