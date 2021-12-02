#include <string>
#include <iostream>
#include <fstream>

struct Position
{
    int horizontal{0};
    int depth{0};
};
int main(int argc, char *argv[])
{
    const std::string fileName = "../../input";

    std::ifstream infile(fileName);   
    std::cout << "Reading" << std::endl;
    
    std::string word;
    int inc;
    
    Position pos;
    
    while(infile >> word >> inc)
    {
        if (word == "forward")
        {
            pos.horizontal += inc;
        }
        else if (word == "down")
        {
            pos.depth += inc;
        }
        else if (word == "up")
        {
            pos.depth -= inc;
        }
        else
        {
            std::cerr << "Did not recognize " << word << std::endl;
        }
    }
    std::cout << pos.depth << ", " << pos.horizontal << std::endl;
    std::cout << (pos.depth * pos.horizontal) << std::endl;
}