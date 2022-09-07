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
// v1.1 New Linux OS Features: cache line size, RAM info, and additional gpu processing  

bool gpuCheck(std::string info); // helper function for detecting gpu on linux

int main(int argc, char* argv[])
{ 
    std::string os, gpu, gpu_info,cppv, ompv; // os independent variables
    cppv = detectCppStl();     // C++ version 
    ompv = detectOmpVersion(); // OpenMP version 

	// lambda to print key, value pairs in std::map for each OS 
    auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
    };
#ifdef _WIN32
	// Map of Windows Commands for CPU Info 
	std::map<std::string, std::string> m {
        {"CPU Name", "wmic cpu get name | more +2" },  
    	{"CPU Sockets", "systeminfo | findstr  /C:\"Processor(s)\""}, 
    	{"CPU Architecture", "echo|set /p=%PROCESSOR_ARCHITECTURE%"},
    	{"CPU Cores", "wmic cpu get NumberOfCores | more +2"}
    }; 
    // process commands stored in map key, then replace the key with the command output 
    std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end()) { 
        std::string command = it->second;    // query command stored in map value 
        const char* torun   = &command[0];   // cast for execsh function  
		std::string result  = execsh(torun); // run the command and store the result 
		result = sanitize(result);           // have to sanitize always on Windows it seems, won't hurt
		// extra processing for "wmic" commands
		if(command.find("wmic") != std::string::npos) { 
            result = result.erase(0, result.find("\n")+1);
            }
        it->second = result;                  // store  
        it++;                                 // step through
     }
	// prepare output 
    os = execsh("systeminfo | findstr  /C:\"OS Name\"");
    os = sanitize(os); 
    gpu = execsh("wmic path win32_videocontroller get name");
    gpu = sanitize(gpu);
    int l_cores = std::thread::hardware_concurrency(); // using std::thread, can't find windows CMD for this     
	// print map 
	cout << os;  
    cout << "CPU Logical Cores " << l_cores << endl;  
    for( const auto& [key, value] : m ) {
		print_key_value(key, value);
        cout << endl; 
	}
	gpu_info = gpuProgModel(gpu);   
    cout << "GPU(s) detected: \n" <<  gpu << "\n" << gpu_info << endl;  
#endif // end Windows OS 
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
