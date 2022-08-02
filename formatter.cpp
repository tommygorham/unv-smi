#include "formatter.h" 
#include <algorithm>   // std::unique_copy used in std::string sanitize() 

// count whitespace  
int countws(const std::string &s){ 
  int spaces =  std::count_if(s.begin(), s.end(),
	              [](unsigned char c){ return std::isspace(c); });
    return spaces; 
} 

// clean up whitespace
std::string sanitize(const std::string &s){ 
	std::string output = ""; 
    output.clear();  // Remove this to append to an existing string
    std::unique_copy (s.begin(), s.end(), std::back_insert_iterator<std::string>(output),
                                     [](char a,char b){ return isspace(a) && isspace(b);});   
    return output; 
}
