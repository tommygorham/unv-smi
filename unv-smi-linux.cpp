#include <iostream> 
using std::cout; 
using std::endl; 
#include <iomanip>     // std::setw  
#include <memory>      // execsh function 
#include <stdexcept>   // execsh function
#include <string_view> // C++17 read-only string  
#include <string>      // commands 
#include <array>       // execsh function
#include <map>         // storage container 
#include <algorithm>   // unique_copy used in std::string sanitize() 
#include "execsh.h" 

// function to store the result of executing a shell command 
//std::string execsh(const char* cmd);

// count whitespace
int countws(const std::string &s); 

// if too much whitespace, run this 
std::string sanitize(const std::string &s); 

int main(int argc, char* argv[])
{  
	#ifdef __linux__ 
	// CPU info map 
    std::map<std::string, std::string> m {
	{"CPU Name", "lscpu | grep -oP \'Model name:\\s*\\K.+\'"}, 
	{"CPU Arch", "lscpu | grep -oP \'Architecture:\\s*\\K.+\'"}, 
	{"CPU Sockets", "lscpu | grep -oP \'Socket\\(s\\):s*\\K.+\'"},  
	{"CPU Cores per Socket","lscpu | grep -oP 'Core\\(s\\) per socket:\\s*\\K.+\'"}, 
	{"CPU Threads Per Core", "lscpu | grep -oP 'Thread\\(s\\) per core:\\s*\\K.+\'"}, 
	{"CPU Logical Cores", "lscpu | grep -oP 'CPU\\(s\\):\\s*\\K.+\'"}
	};
	// OS info command separated, want this to print first 
	std::string os = execsh("cat -s /etc/os-release | grep -oP \"PRETTY_NAME=\\K.*\""); 
    os = sanitize(os); 	

    // process commands stored in map key, then replace the key with the command output 
    std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end()) { 
		std::string command = it->second;    // query command stored in map value 
        const char* torun   = &command[0];   // cast for execsh function  
		std::string result  = execsh(torun); // run the command and store the result 
	    int spaces = countws(result);        // check if output formatting is needed   
	    if (spaces > 10)    {  
			result = sanitize(result);       // sanitize first
	    }  
		it->second = result;                 // store  
        it++;                                // step through
    }
	// calculate total physical CPU cores from map values  
    std::string cpu_CPS = m["CPU Cores per Socket"]; 
    std::string cpu_S   = m["CPU Sockets"]; 
    unsigned int total  = std::stoi(cpu_CPS) * std::stoi(cpu_S);  
    std::string cpu_TC  = std::to_string(total);

	// print map helper
 	auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
    };
	// print map 
    cout << "##### Your Current System Configuration and Computational Resources Available #####";
    cout << "\nOS name: " << os << endl; 
	for( const auto& [key, value] : m ) {
		print_key_value(key, value);
	}
	cout << "CPU Total Physical Cores: " << cpu_TC << endl;  

	//std::string test = execsh("lscpu | grep -oP 'CPU\\(s\\):\\s*\\K.+\'");
	// cout << test; 
	#endif
	return 0; 

}

// execute shell command 
/*std::string execsh(const char* cmd){
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
*/ 
// count whitespace  
int countws(const std::string &s)
{ 
  int spaces =  std::count_if(s.begin(), s.end(),
	              [](unsigned char c){ return std::isspace(c); });
    return spaces; 
} 

// clean up whitespace
std::string sanitize(const std::string &s)
{ 
	std::string output = ""; 
    output.clear();  // Remove this to append to an existing string
    std::unique_copy (s.begin(), s.end(), std::back_insert_iterator<std::string>(output),
                                     [](char a,char b){ return isspace(a) && isspace(b);});   
    return output; 
}

