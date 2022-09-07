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
    // sometimes linux architectures like this command better 
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
	cout << "Total Physical CPU Cores: " << cpu_TC << endl; 
    if(gpu != ""){
		cout << "\nGPU(s) detected: " << gpu << gpu_info << endl;  
    }
    else { // if both lspci commands didn't work  
		cout <<  "\nUnable to determine GPU information" << endl; 
    } 
	#endif // End Linux OS-specific
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

