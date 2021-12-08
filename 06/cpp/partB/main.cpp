#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

struct Count
{
    int64_t co{0};
    void increase()
    {
        ++co;
    }
    void increase(const Count& other)
    {
        co += other.co;
    }
};

std::ostream& operator<<(std::ostream& o, const std::map<int64_t, Count>& elements)
{
    for (const auto& a: elements)
    {
        o << "{" << a.first << ": " << a.second.co << "}";
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
    

    std::map<int64_t, Count> elements;
    int64_t a;
    while (infile >> a)
    {
        elements[a].increase();
        
        if (infile.peek() == ',')
        {
            infile.get();
        }
    }
    std::cout << elements << std::endl;
    
    int day = 0;
    
    while (day < 256)
    {
        std::map<int64_t, Count> newElements;
    
        for (const auto& entry : elements)
        {
            if (entry.first > 0)
            {
                newElements[entry.first - 1].increase(entry.second);
            }
            else
            {
                newElements[6].increase(entry.second);
                newElements[8].increase(entry.second);
            }
        }
        
        elements = std::move(newElements);
        
        ++day;
        
        std::cout << "After day " << day << ":" << elements << std::endl;;
    }
    
    Count total;
    
    for (const auto& entry : elements)
    {
        total.increase(entry.second);
    }

    std::cout << total.co << std::endl;
    
    
    return 0;
}
