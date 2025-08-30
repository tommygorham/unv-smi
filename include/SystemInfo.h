// This file is the driver for reporting information on some operating system
#pragma once
#include <iostream>  // std::cout, std::endl
#include <string>    // main datatype we are working with 
#include <map>       // main data structure to map sh commands to output targets 
#include <algorithm> // std::count_if  
#include <memory>    // execsh function 
#include <stdexcept> // execsh function
#include <thread>    // std::hardware_concurrency()
#include <iomanip>   // std::fixed, std::setprecision 
#include <regex>     // Output formatting 

class SystemInfo {
public:
    virtual ~SystemInfo() = default;
    void printInfo() {
        gatherInfo();              
        cppv = detectCppStl();     
        std::cout << "OS: " << os << std::endl;
        #ifdef _WIN32 
        std::cout << "Total Physical CPU Cores: " << cpu_TC << std::endl; 
        std::cout << "Total Logical CPU Cores: " << cpu_LC << std::endl; 
        #endif 
        for (auto it = infoMap.begin(); it != infoMap.end(); ++it) {
            std::cout << it->first << ": " << it->second;
            #ifdef _WIN32 
            if (std::next(it) != infoMap.end()) {
                std::cout << std::endl;
            } 
            #else 
            std::cout << std::endl;
            #endif 
        }
        if (gpu != "") {
            std::cout << "GPU(s) detected: " << gpu << "\n" << gpu_info << std::endl;
        }
        else { 
            std::cout <<  "\nUnable to determine GPU information" << std::endl; 
        }
        #ifdef USE_APPLECLANG 
        ompv = "Apple-provided clang does not support OpenMP by default"; 
        #else
        ompv = detectOmpVersion(); 
        if (ompv.empty()) { 
            ompv = "OpenMP Version was not determined"; 
        }
        #endif 
    /* Final print for all OS's and software version  */ 
    std::cout <<  cppv << "\n" << ompv << std::endl; 
    std::cout <<  "\n_________________________________________________________\n"; 
    } // End printInfo() 
protected:
    std::string os;
    std::string gpu;
    std::string gpu_info;
    std::string cppv;
    std::string ompv;
    std::string cpu_TC;
    std::string cpu_LC; 
    std::map<std::string, std::string> infoMap;
    virtual void gatherInfo() = 0; 
    virtual std::string execsh(const char* cmd) = 0; 
    // Output formatting function to count whitespace 
    static int countws(const std::string &s) { 
        int spaces =  std::count_if(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); });
        return spaces; 
    } 
    // Output formatting function that removes consecutive whitespaces
    static std::string sanitize(const std::string &s) { 
        std::string output = ""; 
        std::unique_copy (s.begin(), s.end(), std::back_insert_iterator<std::string>(output),
                          [](char a,char b){ return isspace(a) && isspace(b);});   
        return output; 
    }
    // Output formatting function to extract the numeric portion from the string
    static std::string extractNumber(const std::string &s) {
        std::regex e("\\d+");  // Match digits
        std::smatch match;
        if (std::regex_search(s, match, e)) {
            return match.str();
        }
        return "";
    }
    static std::string trim(const std::string& s) {
        auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
        auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
        return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
    }
    // Output formatting helper 
    static std::string extractOSName(const std::string& s) {
        size_t pos = s.find(":");
        if (pos != std::string::npos) {
            return trim(s.substr(pos + 1));  // Get trimmed substring after the colon
        }
        return s;
    }
    // Function to determine Gpu Vendor 
    std::string gpuProgModel(std::string gpu) {
        // Display the GPU vendor and what to program it with  
        std::string ret; 
        if ( gpu.find("AMD") != std::string::npos) {
            ret = "GPU Programming Model: ROCm with HIP\n";
            return ret;  
        }
        else if  (gpu.find("Intel") != std::string::npos) {
            ret = "GPU Programming Model: Intel oneAPI"; 
            return ret; 
        }
        else if (gpu.find("NVIDIA") != std::string::npos || gpu.find("Matrox") != std::string::npos) {
            ret = "GPU Programming Model: CUDA"; 
            return ret; 
        }
        else if (gpu.find("Apple") != std::string::npos) {
            ret = "GPU Programming Model: Metal"; 
            return ret; 
        }
        else if (gpu.find("Microsoft") != std::string::npos) {
            ret = "A GPU with compute capability was not detected";
            return ret; 
        }
        else if (gpu.find("Virtio") != std::string::npos) { 
            ret = "A virtual graphics adapter without GPU Compute was detected";
            return ret;
        }
        else {
            ret = "Cannot determine the programming model for the GPU vendor..."; 
        }
        return ret; 
    } 
    std::string detectCppStl() { 
        #ifdef _MSC_VER
        #if _MSC_VER >= 1928 
        return "C++ Standard: Likely C++20 or newer, using MSVC";
        #elif _MSC_VER >= 1914 
        return "C++ Standard: Likely C++17, using MSVC";
        #elif _MSC_VER >= 1900 
        return "C++ Standard: Likely C++14, using MSVC";
        #else 
        return "C++ Standard: Unknown version, using an older MSVC";
        #endif
        #endif
        switch (__cplusplus) {
            case 202002L:
                return "C++ Standard: C++20, 202002";  
            case 201709L:
                return "C++ Standard: C++20, GCC not the latest version, 201709L"; 
            case 201707L:
                return "C++ Standard: C++20, Clang not the latest version, 201707L"; 
            case 201704L: 
                return "C++ Standard: C++20, VC++ Compiler not the latest version, 201704L"; 
            case 201703L: 
                return "C++ Standard: C++17, 201703L";  
            case 201402L:
                return "C++ Standard: C++14,  201402L";  
            case 201103L: 
                return "C++ Standard: C++11, 201103L"; 
            default: 
                return "C++ version not found. The value of __cplusplus is: " + std::to_string(__cplusplus);
        }
    } 
    std::string detectOmpVersion() {
        // Return the OpenMP version from the macro, see https://www.openmp.org/specifications/
        #ifdef CLANG_COMPILER
        std::string omp_macro = execsh("echo |clang -fopenmp -dM -E - < /dev/null | grep -i openmp");
        #else
        std::string omp_macro = execsh("echo |cpp -fopenmp -dM |grep -i open"); // store the openmp macro this command returns
        #endif
        if (omp_macro.find("201811") != std::string::npos) { return "OpenMP Version: 5.0"; }
        if (omp_macro.find("201511") != std::string::npos) { return "OpenMP Version: 4.5"; }
        if (omp_macro.find("201307") != std::string::npos) { return "OpenMP Version: 4.0"; }
        if (omp_macro.find("200805") != std::string::npos) { return "OpenMP Version: 3.0"; }
        else { return "OpenMP version not found"; }
    }
};
