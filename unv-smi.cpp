/*  unv-smi.cpp 
 *  
 *  Author: Thomas Gorham 
 *  September 2023 
 *  
 */ 

#include "SystemInfo.h"
#include "LinuxInfo.h"
#include "MacInfo.h"
#include "WindowsInfo.h"

static constexpr float SW_VERSION = 1.3; // 9/2/2023 

int main() {
    std::unique_ptr<SystemInfo> sysInfo;

    #ifdef __linux__
        std::cout << "Linux System Detected" << std::endl;   
        sysInfo = std::make_unique<LinuxInfo>();
    #elif __APPLE__
        std::cout << "Apple System Detected" << std::endl;   
        sysInfo = std::make_unique<MacInfo>();
    #elif _WIN32
        std::cout << "Windows System Detected" << std::endl;   
        sysInfo = std::make_unique<WindowsInfo>();
    #endif

    if (sysInfo) {
        sysInfo->printInfo();
    }
    std::cout << "Thank you for using Universal System Management Interface\n"
              << "unv-smi v" << std::fixed << std::setprecision(1) << SW_VERSION 
	          << "\n\n"; 
    return 0;
}
