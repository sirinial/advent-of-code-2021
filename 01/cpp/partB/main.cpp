#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    const std::string fileName = "../../input";

    std::ifstream infile(fileName);   
    std::cout << "Counting" << std::endl;
    
    int pos1, pos2, pos3, newOne;
    infile >> pos1 >> pos2 >> pos3;
    
    int counter{0};
    
    while(infile >> newOne)
    {
        if (pos1 < newOne)
        {
            ++counter;
        } 
        pos1 = pos2;
        pos2 = pos3;
        pos3 = newOne;
    }
    std::cout << counter << std::endl;
}