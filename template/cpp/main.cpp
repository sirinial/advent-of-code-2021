#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Missing argument!" << std::endl;
        return 1;
    }
    const std::string fileName = argv[1];

    std::ifstream infile(fileName);   
    std::cout << "Reading " << fileName << std::endl;
    
    std::string line;
    while (infile >> line)
    {
        std::cout << line << std::endl;
    }
    
    
    return 0;
}
