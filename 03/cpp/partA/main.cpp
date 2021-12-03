#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

struct BitCount
{
    int zeroes{0};
    int ones{0};
};

struct Counter
{
    std::map<int, BitCount, std::greater<int>> map;
};

int main(int argc, char *argv[])
{
    const std::string fileName = "../../input";

    std::ifstream infile(fileName);   
    std::cout << "Reading" << std::endl;
    
    std::string word;
    
    Counter count;
    
    while(infile >> word)
    {
        std::reverse(word.begin(), word.end());
        int pos = 0;
        for (char c : word)
        {
            if (c == '0')
            {
                ++count.map[pos].zeroes;
            }
            else if (c == '1')
            {
                ++count.map[pos].ones;
            }        
            ++pos;
        }
    }
    
    int gamma{0}, epsilon{0};;
    std::string gammaword, epsilonword;
    
    
    for (const auto& entry: count.map)
    {
        gamma *= 2;
        epsilon *= 2;
        if (entry.second.zeroes > entry.second.ones)
        {
            epsilonword += "1";
            epsilon += 1;
            gammaword += "0";
        }
        else if (entry.second.zeroes < entry.second.ones)
        {
            gammaword += "1";
            epsilonword +="0";
            gamma += 1;
        }
        
        std::cout << gammaword << "  " << epsilonword << std::endl; 
        std::cout << entry.first << ": " << entry.second.zeroes << "/" << entry.second.ones << std::endl;
    }
     std::cout << gamma << " * " << epsilon << ": " << (gamma * epsilon) << std::endl;   
}