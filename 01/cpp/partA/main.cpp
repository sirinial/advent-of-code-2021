#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    const std::string fileName = "../../input";

    std::ifstream infile(fileName);   
    std::cout << "Counting" << std::endl;
    
    int lastOne, newOne;
    infile >> lastOne;
    
    int counter;
    
    while(infile >> newOne)
    {
        if (newOne > lastOne)
        {
            ++counter;
        } 
        lastOne = newOne;
    }
    std::cout << counter << std::endl;
}