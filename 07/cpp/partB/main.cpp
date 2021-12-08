#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

std::ostream& operator<<(std::ostream& o, const std::vector<int>& elements)
{
    for (const int a: elements)
    {
        o << " " << a;
    }
    return o;
}

int fuelNeeded(const int pos, const std::vector<int>& elements)
{
    int result = 0;
    for (const int a : elements)
    {
        result += std::abs(pos - a)*(std::abs(pos-a) + 1)/2;
    }
    return result;
}

bool findBetter(int& pos, const std::vector<int>& elements)
{
    int currentFuelNeeded = fuelNeeded(pos, elements);
    
    if (currentFuelNeeded > fuelNeeded(pos - 1, elements))
    {
        --pos;
        return true;
    }
    else if (currentFuelNeeded > fuelNeeded(pos +1, elements))
    {
        ++pos;
        return true;
    }
    return false;
}


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
    

    std::vector<int> elements;    
    int a;
    while (infile >> a)
    {
        elements.emplace_back(a);
        
        if (infile.peek() == ',')
        {
            infile.get();
        }
    }
    std::cout << elements << std::endl;

    int min = 0;
    int max = 0;
    
    
    for (const int a: elements)
    {
        if (a < min)
        {
            min = a;
        }
        else if(a > max)
        {
            max = a;
        }
    }
    
    
    int pos = 0;    
    
    int lowestFuelNeeded = fuelNeeded(pos, elements);
        
    for (int a = min; a <= max; ++a)
    {
        int fuelNeededForA = fuelNeeded(a, elements);
        if (fuelNeededForA < lowestFuelNeeded)
        {
            pos = a;
            lowestFuelNeeded = fuelNeededForA;
        }
    }
    std::cout << pos << ": " << lowestFuelNeeded << std::endl;
    
    return 0;
}
