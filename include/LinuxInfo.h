/* 
 * This file contains Linux-specific commands for reporting information      
 */ 

#pragma once
#include "SystemInfo.h" 
#include <memory> 
#include <array> 

#ifdef __linux__
class LinuxInfo : public SystemInfo {
public:
    ~LinuxInfo() override = default;
protected:
    /* Execute a pre-defined command in the Linux infoMap */  
    std::string execsh(const char* cmd) override {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
        return result;
    }
    /* Shell Commands to gather Linux-specific info */ 
    void gatherInfo() override {
        infoMap["CPU Name"] = "lscpu | grep -oP \'Model name:\\s*\\K.+\'";
        infoMap["CPU Architecture"] =  "lscpu | grep -oP \'Architecture:\\s*\\K.+\'"; 
        infoMap["CPU Sockets Installed"] = "lscpu | grep -oP \'Socket\\(s\\):s*\\K.+\'";   
        infoMap["CPU Cores per Socket"] = "lscpu | grep -oP \'Core\\(s\\) per socket:\\s*\\K.+\'"; 
        infoMap["CPU Threads Per Core"] = "lscpu | grep -oP \'Thread\\(s\\) per core:\\s*\\K.+\'"; 
        infoMap["CPU Logical Cores"] = "lscpu | grep -oP \'CPU\\(s\\):\\s*\\K.+\'"; 
        infoMap["CPU Cache Line Size"] = "getconf -a | grep CACHE | grep -oP -m1 \'LINESIZE\\s*\\K.+\'";  
        infoMap["Stack Size Limit"] = "ulimit -a | grep stack"; 
        infoMap["RAM MemTotal"] = "cat /proc/meminfo | grep -oP \'MemTotal:\\s*\\K.+\'";     
        infoMap["RAM MemFree "] = "cat /proc/meminfo | grep -oP \'MemFree:\\s*\\K.+\'";      
        os  = execsh("cat -s /etc/os-release | grep -oP \"PRETTY_NAME=\\K.*\"");
        
        /* Remove whitespace from output */ 
        os  = sanitize(os);  
        os = trim(os); 
        gpu = execsh("lspci | grep 3D");
        
        /* If lscpi | grep 3D didn't work */ 
        if (gpu.empty()) { 
            /* Sometimes linux architectures like this command better */ 
            gpu = execsh("lspci | grep VGA"); 
        }
        
        /* Remove whitespace from output */ 
        gpu = sanitize(gpu); 
        gpu = trim(gpu); 
        /* Use string matching to try and determine the GPU Vendor */ 
        gpu_info = gpuProgModel(gpu); 
        /* Process commands stored in map key, then replace the key with the command output */  
        std::map<std::string, std::string>::iterator it = infoMap.begin();
        while (it != infoMap.end()) { 
            std::string command = it->second;    // query command stored in map value 
            const char* torun   = &command[0];   // cast for execsh function  
            std::string result  = execsh(torun); // run the command and store the result 
            result = sanitize(result);           // Remove whitespace from output 
            result = trim(result);               // Remove whitespace from output 
            it->second = result;                 // replace cmd with output of cmd
            it++;                                // step through
        }
        /* Calculate total physical CPU cores from map values */ 
        std::string cpu_CPS = infoMap["CPU Cores per Socket"]; 
        std::string cpu_S   = infoMap["CPU Sockets Installed"]; 
        
        /* These are often "-" on virtual machines :( */ 
        if (cpu_CPS.empty() || cpu_S.empty()) {
            std::cout << "CPU Cores per Socket value is empty!" << std::endl;
            cpu_TC = "Total Cores undetermined. Cores per Socket or Sockets Installed is empty"; 
        } 
        else {
            try { 
                unsigned int total  = std::stoi(cpu_CPS) * std::stoi(cpu_S);  
                cpu_TC = std::to_string(total); 
            } 
            catch (const std::invalid_argument& e) { 
                throw std::runtime_error("Conversion error: Invalid argument encountered when calculating total cores.");     
            }
            catch (const std::out_of_range& e) {
                throw std::runtime_error("Conversion error: Value out of range when calculating total cores.");
            }
        }
    }
};
#endif
