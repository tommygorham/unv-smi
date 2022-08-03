#include <iostream> 
using std::cout; 
using std::endl; 
#include <memory>      // execsh function 
#include <stdexcept>   // execsh function
#include <string_view> // C++17 read-only string  
#include <string>      // commands 
#include <array>       // execsh function
#include <map>         // storage container for holding CPU commands and returning CPU info  
#include <thread>      // for Windows logical cores 
#include "include/execsh.h"    // execute and store shell command from c++ 
#include "include/parEnv.h"    // C++ and OpenMP version 
#include "include/formatter.h" // whitespace helper functions: countws(), sanitize() 
#include "include/gpuinfo.h" 

int main(int argc, char* argv[])
{
    std::string os, gpu, gpu_info, cppv, ompv;   
    cppv = detectCppStl();     // C++ version 
	ompv = detectOmpVersion(); // OpenMP version 
    // lambda to print key, value pairs in std::map 
    auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
        };

#ifdef __linux__ 
	// Map of Commands for CPU info 
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
			result = sanitize(result);   // sanitize first
	        }  
		it->second = result;                 // store  
                it++;                                // step through
        }
	// calculate total physical CPU cores from map values  
        std::string cpu_CPS = m["CPU Cores per Socket"]; 
        std::string cpu_S   = m["CPU Sockets Installed"]; 
        unsigned int total  = std::stoi(cpu_CPS) * std::stoi(cpu_S);  
        std::string cpu_TC  = std::to_string(total);
	
	// print map helper
 	/*auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
        };*/ 
	// print map 
        cout << "\n##### Your Current System Configuration and Computational Resources Available #####";
        cout << "\nOS name: " << os; 
	for( const auto& [key, value] : m ) {
		print_key_value(key, value);
	}
	cout << "CPU Total Physical Cores: " << cpu_TC << endl; // end of cpu info 
#endif

#ifdef _WIN32
    std::map<std::string, std::string> m {
    {"CPU Name", "wmic cpu get name | more +2" },  
    {"CPU Sockets", "systeminfo | findstr  /C:\"Processor(s)\""}, 
    {"CPU Architecture", "echo|set /p=%PROCESSOR_ARCHITECTURE%"},
    {"CPU Cores", "wmic cpu get NumberOfCores | more +2"}
    }; 

    std::map<std::string, std::string>::iterator it = m.begin();
        while (it != m.end()) { 
		std::string command = it->second;    // query command stored in map value 
                const char* torun   = &command[0];   // cast for execsh function  
		std::string result  = execsh(torun); // run the command and store the result 
			result = sanitize(result);   // have to sanitize all first on Windows
		    if(command.find("wmic") != std::string::npos) 
            { 
               result = result.erase(0, result.find("\n")+1);
            }
            it->second = result;                 // store  
            it++;                                // step through
        }

    os = execsh("systeminfo | findstr  /C:\"OS Name\"");
    os = sanitize(os); 
    gpu = execsh("wmic path win32_videocontroller get name");
    gpu = sanitize(gpu);
    int l_cores = std::thread::hardware_concurrency(); /*max openmp threads = logical cpus on machine */    

    cout << os;  
    cout << "CPU Logical Cores " << l_cores << endl;  
    for( const auto& [key, value] : m ) {
		print_key_value(key, value);
        cout << endl; 
	}
  
    // std::string temp_p_cores = m["CPU Cores"];
    // temp_p_cores = sanitize(temp_p_cores); 
    // cout << "\nTEMP: " <<  std::stoi(temp_p_cores); 
    // int thr_per_core = l_cores/p_cores; 
    // cout << "\nThreads per Core: " << thr_per_core; 
      
#endif 

	cout << "GPU(s) detected: \n" <<  gpu << endl;  
	gpu_info = gpuProgModel(gpu);  
	cout << "\n##### Parallel Programming Environment ##### \n" << cppv << "\n" << ompv << "\n" << gpu_info <<  endl; 
        cout <<"\n\nFurther Commands that can potentially be used for GPU identification\nlspci | grep 3D\nlspci | grep VGA\nsudo lshw -C video" << endl; 
	return 0; 
}
