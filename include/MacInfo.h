/* 
 * This file contains Mac-specific commands for reporting information 
 */ 

#pragma once
#include "SystemInfo.h"
#include <memory> 
#include <array>

#ifdef __APPLE__
class MacInfo : public SystemInfo {
public:
    ~MacInfo() override = default;
protected: 
    // Function to execute a pre-defined command on a Unix-based system 
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

    // Function for retrieving Operating System information on MacOS    
    std::string getOsInfo() {
        // Extract MacOS name (e.g., Yosemite) & erase the extra white space in the returned string 
        std::string productName = execsh("sw_vers | grep ProductName | cut -d':' -f2");
        productName.erase(productName.begin(), std::find_if(productName.begin(), productName.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        productName.erase(std::find_if(productName.rbegin(), productName.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), productName.end());
        // Extract MacOS version & erase the extra white space in the returned string 
        std::string productVersion = execsh("sw_vers | grep ProductVersion | cut -d':' -f2");
        productVersion.erase(productVersion.begin(), std::find_if(productVersion.begin(), productVersion.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        productVersion.erase(std::find_if(productVersion.rbegin(), productVersion.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), productVersion.end());
        return productName + " " + productVersion;
    }
    
    // Function for retrieving additional architectural info on Mac OS
    void gatherInfo() override {
        // Key, Value Map of MacOS Commands (<what the cmd returns>, <cmd>) 
        infoMap["CPU Name"] = "sysctl -n machdep.cpu.brand_string";  
        infoMap["CPU Logical Cores"] = "sysctl -n hw.logicalcpu";
        infoMap["CPU Physical Cores"] = "sysctl -n hw.physicalcpu"; 
        infoMap["CPU Architecture"] = "uname -m";  
        infoMap["Memory"] = "system_profiler SPHardwareDataType | grep 'Memory:' | awk -F': ' '{print $2}'";
        infoMap["Memory Type"] = "system_profiler SPMemoryDataType | grep 'Type:' | awk -F': ' '{print $2}'";
        infoMap["Memory Manufacturer"] = "system_profiler SPMemoryDataType | grep 'Manufacturer:' | awk -F': ' '{print $2}'";
        infoMap["GPU Cores"] = "system_profiler SPDisplaysDataType | grep 'Total Number of Cores' | awk -F': ' '{print $2}'";

        // Execute each key's value in the map & replace each value with the output 
        std::map<std::string, std::string>::iterator it = infoMap.begin();
        while (it != infoMap.end()) { 
            std::string command = it->second;    // query command stored in map value 
            const char* torun   = &command[0];   // cast for execsh function  
            std::string result  = execsh(torun); // run the command and store the result 
            result = sanitize(result);           // output formatting 
            result = trim(result);               // output formatting 
            it->second = result;                 // replace cmd with output of cmd
            it++;                                // step through
        }
        // Retrieve OS info and GPU info after processing key,value map
        os = getOsInfo();  
        gpu = execsh("system_profiler SPDisplaysDataType | grep 'Chipset Model' | awk -F': ' '{print$2}'");
        gpu = sanitize(gpu); 
        gpu = trim(gpu);
        gpu_info = gpuProgModel(gpu);  
    }
};
#endif
