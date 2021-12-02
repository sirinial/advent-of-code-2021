#include <string>
#include <iostream>
#include <fstream>

struct Position
{
    int horizontal{0};
    int depth{0};
    int aim{0};
};
std::ostream& operator<<(std::ostream& o, const Position& p)
{
    o << "{horizontal: " << p.horizontal << ", depth: " << p.depth << ", aim: " << p.aim << "}";
    return o;
}

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
        std::cout << word << " " << inc << std::endl;
        std::cout << "    --> " << std::flush;
        if (word == "forward")
        {
            pos.horizontal += inc;
            pos.depth += pos.aim*inc;
        }
        else if (word == "down")
        {
            pos.aim += inc;
//            pos.depth += inc;
        }
        else if (word == "up")
        {
//            pos.depth -= inc;
            pos.aim -= inc;
        }
        else
        {
            std::cerr << "Did not recognize " << word << std::endl;
        }
        std::cout << pos << std::endl;
    }
    std::cout << pos.depth << ", " << pos.horizontal << std::endl;
    std::cout << (pos.depth * pos.horizontal) << std::endl;
}