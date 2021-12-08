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
    
    int day = 0;
    
    while (day < 80)
    {
        int nbOfNewElements{0};
        
        for (int& a : elements)
        {
            if (a > 0)
            {
                --a;
            }
            else
            {
                a = 6;
                ++nbOfNewElements;
            }
        }
        
        for (int i = 0; i < nbOfNewElements; ++i)
        {
            elements.emplace_back(8);
        }
        
        ++day;
        
        std::cout << "After day " << day << ":" << elements << std::endl;;
    }
    
    std::cout << elements.size() << std::endl;
    
    
    return 0;
}
