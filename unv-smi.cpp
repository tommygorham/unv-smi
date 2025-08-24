/* Dynamically create an object of a class based on the platform, then execute 
 * the appropriate commands for that platform to print info to the terminal */
#include "SystemInfo.h"
#include "LinuxInfo.h"
#include "MacInfo.h"
#include "WindowsInfo.h"

static constexpr float SW_VERSION = 1.4; 

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
    std::cout << "unv-smi v" << std::fixed << std::setprecision(1) << SW_VERSION 
              << "\n\n"; 
    return 0;
}
