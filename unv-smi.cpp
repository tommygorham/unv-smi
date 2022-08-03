#include <iostream> 
using std::cout; 
using std::endl; 
#include <memory>              // execsh function 
#include <stdexcept>           // execsh function
#include <string_view>         // C++17 read-only string  
#include <string>              // commands 
#include <array>               // execsh function
#include <map>                 // storage container for holding CPU commands and returning CPU info  
#include <thread>              // get logical cores with std::hardware_concurrency()
#include "include/execsh.h"    // execute and store shell command from c++ 
#include "include/parEnv.h"    // C++ and OpenMP version 
#include "include/formatter.h" // whitespace helper functions: countws(), sanitize() 
#include "include/gpuinfo.h"   // gpuProgModel() 

int main(int argc, char* argv[])
{ 
	
    std::string os, gpu, gpu_info, cppv, ompv; // os independent vars to print at EOP   
    cppv = detectCppStl();     // C++ version 
    ompv = detectOmpVersion(); // OpenMP version 
    // lambda to print key, value pairs in std::map for each OS 
    auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
    };
        
	#ifdef __linux__ 
	// Map of Linux Commands for CPU info 
    std::map<std::string, std::string> m {
	    {"CPU Name", "lscpu | grep -oP \'Model name:\\s*\\K.+\'"}, 
	    {"CPU Architecture", "lscpu | grep -oP \'Architecture:\\s*\\K.+\'"}, 
	    {"CPU Sockets Installed", "lscpu | grep -oP \'Socket\\(s\\):s*\\K.+\'"},  
	    {"CPU Cores per Socket","lscpu | grep -oP 'Core\\(s\\) per socket:\\s*\\K.+\'"}, 
	    {"CPU Threads Per Core", "lscpu | grep -oP 'Thread\\(s\\) per core:\\s*\\K.+\'"}, 
	    {"CPU Logical Cores", "lscpu | grep -oP 'CPU\\(s\\):\\s*\\K.+\'"}
	};
	// OS  info command separate from map, want this to print first 
	os  = execsh("cat -s /etc/os-release | grep -oP \"PRETTY_NAME=\\K.*\""); 
    // GPU info command separate from map, needs additional processing 
    gpu = execsh("lspci | grep 3D"); 
    // process commands stored in map key, then replace the key with the command output 
	std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end()) { 
		std::string command = it->second;    // query command stored in map value 
        const char* torun   = &command[0];   // cast for execsh function  
		std::string result  = execsh(torun); // run the command and store the result 
	    int spaces = countws(result);        // check if output formatting is needed   
	    if (spaces > 10) {  
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
	cout << "CPU Total Physical Cores: " << cpu_TC << endl; 
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
    #endif // end MacOS 
    
	// final print for all OS's 
	cout << "GPU(s) detected: \n" <<  gpu << endl;  
	gpu_info = gpuProgModel(gpu);  
	cout << "\n##### Parallel Programming Environment ##### \n" << cppv << "\n" << ompv << "\n" << gpu_info <<  endl; 
    cout << "\n\nFurther Commands that can potentially be used for GPU identification\n"; 
    cout << "lspci | grep 3D\nlspci |grepVGA\nsudo lshw -C video\n"; 
    cout << "For MacOS: system_profiler SPDisplaysDataType\n" << endl; 
	
    return 0; 
}
