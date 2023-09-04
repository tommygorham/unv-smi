/*  WindowsInfo.h
 *  
 *  Author: Thomas Gorham 
 *  September 2023 
 *  
 */ 

#pragma once
#include "SystemInfo.h"
#include <memory> 
#include <array> 

#ifdef _WIN32
class WindowsInfo : public SystemInfo {
public:
    ~WindowsInfo() override = default;
protected: 
   /* Execute a pre-defined command on Windows systems */ 
    std::string execsh(const char* cmd)  { 
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if (!pipe) {
            throw std::runtime_error("_popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
 
    // Implementation to gather Windows-specific info.
    void gatherInfo() override {
        infoMap["CPU Name"] = "wmic cpu get name | more +2";   
        infoMap["CPU Sockets"] = "systeminfo | findstr  /C:\"Processor(s)\""; 
        infoMap["CPU Architecture"] = "echo|set /p=%PROCESSOR_ARCHITECTURE%";
        // process commands stored in map key, then replace the key with the command output 
        std::map<std::string, std::string>::iterator it = infoMap.begin();
        while (it != infoMap.end()) { 
            std::string command = it->second;    // query command stored in map value 
            const char* torun   = &command[0];   // cast for execsh function  
            std::string result  = execsh(torun); // run the command and store the result 
            result = sanitize(result);           // always sanitize on Windows 
            // extra processing for "wmic" commands
            if(command.find("wmic") != std::string::npos) { 
                result = sanitizeWmicOutput(result); 
                result = sanitize(result); 
            } else if (command.find("systeminfo") != std::string::npos) {
                result = sanitizeSystemInfoOutput(result);
            }
            it->second = result;                  // store  
            it++;                                 // step through
        }
        // prepare output 
        os = execsh("systeminfo | findstr  /C:\"OS Name\"");
        os = sanitize(os); 
        os = extractOSName(os); 
        gpu = execsh("wmic path win32_videocontroller get name");
        gpu = sanitize(gpu);
        gpu = sanitizeWmicOutput(gpu); 
        cpu_TC = execsh("wmic cpu get NumberOfCores | more +2");
        cpu_TC = extractNumber(cpu_TC); 
        int l_cores = std::thread::hardware_concurrency(); // using std::thread, can't find windows CMD for this     
        cpu_LC = std::to_string(l_cores); 
        // print map 
        gpu_info = gpuProgModel(gpu);   
    }
private: 
    static std::string sanitizeWmicOutput(const std::string& s) {
        size_t pos = s.rfind("Name");
        if (pos != std::string::npos) {
            std::string extracted = s.substr(pos + 4);
            size_t firstNonWhitespacePos = extracted.find_first_not_of(" \t\n\r\f\v");

            if (firstNonWhitespacePos != std::string::npos) {
                return extracted.substr(firstNonWhitespacePos);
            }
        }
        return s;
    }

    static std::string sanitizeSystemInfoOutput(const std::string &s) {
        size_t pos = s.find(":");
        if (pos != std::string::npos) {
           // +2 to skip the colon and the space after it
           return s.substr(pos + 2);
        }
        return s;
    }
};

#endif
