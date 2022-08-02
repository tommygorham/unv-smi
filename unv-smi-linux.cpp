#include <iostream> 
using std::cout; 
using std::endl; 
#include <iomanip>   // std::setw  
#include <memory>    // execsh 
#include <stdexcept> // execsh 
#include <string_view>  
#include <string>
#include <array>
#include <map>
#include <algorithm> // unique_copy used in std::string sanitize() 

// function to store the result of executing a shell command 
std::string execsh(const char* cmd);

// count whitespace
int countws(const std::string &s); 

// if too much whitespace, run this 
std::string sanitize(const std::string &s); 

int main(int argc, char* argv[])
{  
	#ifdef __linux__ 
    std::map<std::string, std::string> m {
    {"GPU Name", "lspci"}, 
	{"CPU Name", "lscpu | grep -oP \'Model name:\\s*\\K.+\'"}, 
	{"CPU Arch", "lscpu | grep -oP \'Architecture:\\s*\\K.+\'"}, 
	{"CPU Sockets", "lscpu | grep -oP \'Socket\\(s\\):s*\\K.+\'"},  
	{"OS  Name", "cat -s /etc/os-release | grep -oP \"PRETTY_NAME=\\K.*\""}
	};

    /*
    std::string test = execsh("lscpu | grep -oP \'Socket\\(s\\):s*\\K.+\'"); 
    int spaces = countws(test);  
	cout << "\nSpaces: " << spaces << endl; 
	test = sanitize(test); 
	cout << test << endl; 
    int spacesaft =  countws(test);  
	cout << "\nSpaces: " << spacesaft << endl;  
	*/ 
	// add another key with empty string for the value 
	// m["GPU"]; 
   
    // can change values already initialized 
	// m["OS  Name"] = "somethingelse"; 
	
	// print just value 
    // cout << m["OS  Name"]; 
	
    // iterate over keys
    std::map<std::string, std::string>::iterator it = m.begin();
    while (it != m.end())
    {
        // Accessing the key 
		std::string command = it->second;  // query command stored in map value 
        const char* torun   = &command[0]; // cast for execsh function  
		std::string result  = execsh(torun); // run the command and store the result 
	    int spaces = countws(result); // check if output formatting is needed   
	    if (spaces > 10)    {  
			result = sanitize(result); // sanitize first
	    }  
		it->second = result; // store  
		//cout << "\n" <<  result << endl;
        it++; // step through
    }
	
	// print map helper
 	auto print_key_value = [](const auto& key, const auto& value) {
        cout <<  key << ": " << value;
    };
	// print map 
    std::cout << "\n##### Your Current System Configuration and Computational Resources Available #####\n";
    for( const auto& [key, value] : m ) {
		print_key_value(key, value);
	}
	 
	//std::string test = execsh("ls -a"); 
	#endif
	return 0; 

}

// execute shell command 
std::string execsh(const char* cmd){
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

