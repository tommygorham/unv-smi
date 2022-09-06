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

static constexpr float SW_VERSION = 1.1; // 8/29/2022 
// v1.1 New Linux OS Features: cache line size, RAM info, and additional gpu processing  

int main(int argc, char* argv[])
{ 
    std::string os, gpu, gpu_info,cppv, ompv; // os independent variables
    cppv = detectCppStl();     // C++ version 
	ompv = detectOmpVersion(); 
	// kill program for now if C++17 not detected 
    if ( __cplusplus < 201703L){ 
		cout << "\nYou are using " << cppv << "\n";  
		cout << "\nC++17 is currently needed for full/appropriate functionality of this program\nTerminating...\n";  
		return -1; 
	}
	// lambda to print key, value pairs in std::map for each OS 
    auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
    };
        
	#ifdef __linux__ 
	// Map of Linux Commands for CPU-related info 
	std::map<std::string, std::string> m {
	    {"CPU Name", "lscpu | grep -oP \'Model name:\\s*\\K.+\'"}, 
	    {"CPU Architecture", "lscpu | grep -oP \'Architecture:\\s*\\K.+\'"}, 
	    {"CPU Sockets Installed", "lscpu | grep -oP \'Socket\\(s\\):s*\\K.+\'"},  
	    {"CPU Cores per Socket","lscpu | grep -oP \'Core\\(s\\) per socket:\\s*\\K.+\'"}, 
	    {"CPU Threads Per Core", "lscpu | grep -oP \'Thread\\(s\\) per core:\\s*\\K.+\'"}, 
	    {"CPU Logical Cores", "lscpu | grep -oP \'CPU\\(s\\):\\s*\\K.+\'"}, 
		{"CPU Cache Line Size", "getconf -a | grep CACHE | grep -oP -m1 \'LINESIZE\\s*\\K.+\'"},  
		{"Stack Size Limit", "ulimit -a | grep stack"}, 
        {"RAM MemTotal", "cat /proc/meminfo | grep -oP \'MemTotal:\\s*\\K.+\'"}, 	
        {"RAM MemFree ", "cat /proc/meminfo | grep -oP \'MemFree:\\s*\\K.+\'"} 	
    };
    // OS ad GPU commands separate from map for formatting purposes 
    os  = execsh("cat -s /etc/os-release | grep -oP \"PRETTY_NAME=\\K.*\""); 
	gpu = execsh("lspci | grep 3D");
    if(gpu.empty()){ 
		gpu = execsh("lspci | grep VGA"); 
	}
	gpu_info = gpuProgModel(gpu); 
    // process commands stored in map key, then replace the key with the command output 
	std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end()) { 
		std::string command = it->second;    // query command stored in map value 
        const char* torun   = &command[0];   // cast for execsh function  
		std::string result  = execsh(torun); // run the command and store the result 
	    int spaces = countws(result);        // check if output formatting is needed   
	    if (spaces > 2) {  
			result = sanitize(result);       // sanitize first if excessive whitespace
	    }  
		it->second = result;                 // replace cmd with output of cmd
        it++;                                // step through
    }
	// calculate total physical CPU cores from map values  
    std::string cpu_CPS = m["CPU Cores per Socket"]; 
    std::string cpu_S   = m["CPU Sockets Installed"]; 
    unsigned int total  = std::stoi(cpu_CPS) * std::stoi(cpu_S);  
    std::string cpu_TC  = std::to_string(total);
	// print map 
	cout << "\n##### Your Current System Configuration and Computational Resources Available #####";
    cout << "\nOS name: " << os;
	for( const auto& [key, value] : m ) {
		print_key_value(key, value);
	 }
	cout << "Total Physical CPU Cores: " << cpu_TC; 
	// try alternative lspci command, only for linux and if not detected gpu yet  
    if(gpu != ""){ 
	    gpu_info = gpuProgModel(gpu);  
		cout << "\nGPU(s) detected: " <<  gpu  << gpu_info << endl;  
    }
	else{ 
		cout << "\nUnable to determine GPU information" << endl; 
	} 
	#endif // End Linux OS 

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
	
	// final print for all OS's and software version  
	cout << cppv << ompv << endl; 
	cout <<  "\n____________________________________________________________________________________\n\n"; 
	cout << "Thank you for using Universal System Management Interface version " << std::fixed << std::setprecision(1) << SW_VERSION; 
	cout << "\n\n"; 
    
	return 0; 
	
}

