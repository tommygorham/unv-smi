// Version 1.2
// Removed C++17 string_view to begin making more portable 
// Separate Code for Linux, Windows, & Mac OS  

// Version 1.1 
// New Linux OS Features: cache line size, RAM info, and additional gpu processing  

#include <iostream> 
using std::cout; 
using std::endl; 
#include <memory>              // execsh function 
#include <stdexcept>           // execsh function
#include <string>              // shell/terminal commands 
#include <map>                 // storage container for holding CPU commands and returning CPU info  
#include <thread>              // get logical cores with std::hardware_concurrency()
#include <iomanip>             // std::fixed, std::setprecision 
#include <new>  
#include "include/execsh.h"    // execute and store shell command from c++ 
#include "include/parEnv.h"    // C++ and OpenMP version 
#include "include/formatter.h" // whitespace helper functions: countws(), sanitize() 
#include "include/gpuinfo.h"   // gpuProgModel() 

static constexpr float SW_VERSION = 1.2; // 9/7/2022 

int main(int argc, char* argv[])
{ 
    std::string os, gpu, gpu_info,cppv, ompv; // os independent variables
    cppv = detectCppStl();     // C++ version 
    ompv = detectOmpVersion(); // OpenMP version 
	
	// lambda to print key, value pairs in std::map for each OS 
    auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
    };
#ifdef __APPLE__
    // map of MACOS Commands for CPU Info 
    cout << "\ndue to system_profiler commands, this process may take up to 30 seconds on MacOS" << endl;
    cout << "\nProcessing now ..." << endl; 
    std::map<std::string, std::string> m{ 
        {"CPU Name", "sysctl -n machdep.cpu.brand_string"}, 
        {"CPU Logical Cores", "sysctl -n hw.logicalcpu"},
        {"CPU Physical Cores", "sysctl -n hw.physicalcpu"}, 
        {"CPU Architecture", "uname -m"}, 
        {"CPU Sockets Installed", "system_profiler | grep \'Number of Processors\'"} 
  }; 

    // process commands stored in map key, then replace the key with the command output 
	std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end()) { 
        std::string command = it->second;    // query command stored in map value 
        const char* torun   = &command[0];   // cast for execsh function  
        std::string result  = execsh(torun); // run the command and store the result 
        int spaces = countws(result);        // check if output formatting is needed   
        if (spaces > 4) {  
        result = sanitize(result);   // sanitize first if excessive whitespace
        }  
        it->second = result;                 // replace cmd with output of cmd
        it++;                                // step through
    }
    // prepare output
    os = execsh("sw_vers | grep ProductVersion"); 
    gpu = execsh("system_profiler SPDisplaysDataType");  
	// print map 
	cout << "\n##### Your Current System Configuration and Computational Resources Available #####";
    cout << "\nMacOS Version: " << os;  
    for( const auto& [key, value] : m ) {
		print_key_value(key, value);
	}
	cout << "\nFor MacOS, a useful command for detecting GPU Specs is: system_profiler SPDisplaysDataType\n" << endl; 
    gpu_info = gpuProgModel(gpu);  
	cout << "GPU(s) detected: \n" << gpu <<"\n" << gpu_info << endl;
#endif // end MacOS 
	if (ompv.empty()) { 
	ompv = "OpenMP Version was not determined"; 
    } 
	// final print for all OS's and software version  
	cout << "\n##### Parallel Programming Environment ##### \n" << cppv << "\n" << ompv << endl; 
    cout <<  "\n\n____________________________________________________________________________________\n\n"; 
	cout << "Thank you for using Universal System Management Interface version " << std::fixed << std::setprecision(1) << SW_VERSION; 
	cout << "\n\n"; 
    return 0; 
} 
