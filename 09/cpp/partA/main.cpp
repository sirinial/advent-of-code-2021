#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

struct Map
{
    std::vector<std::vector<int>> values;
    
    int valueOf(const int i, const int j) const
    {
        if (i < 0 || j < 0)
        {
            return 10;
        }
        
        if (static_cast<int>(values.size()) <= i)
        {
            return 10;
        }
        const std::vector<int>& line = values.at(i);
        
        if (static_cast<int>(line.size()) <= j)
        {
            return 10;
        }
        return line.at(j);
    }
};

std::ostream& operator<<(std::ostream& o, const Map& m)
{
    for (const auto& l: m.values)
    {
        for (const auto& v : l)
        {
            o << v;
        }
        o << std::endl;        
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
    
    Map map;
    while (infile.peek() != EOF)
    {
        std::vector<int> line;
        
        while (infile.peek() != EOF && infile.peek() != '\n')
        {
            line.emplace_back(infile.get() - static_cast<int>('0'));
        }
        infile.get();
        
        map.values.emplace_back(line);
    }
    std::cout << map << std::endl;
    
    int total = 0;
    
    int i = 0;
    for (const auto& l: map.values)
    {
        int j = 0;
        for (const int v : l)
        {
            if (map.valueOf(i,j) != v)
            {
                std::cerr << "Problem with order" << std::endl;
            }
            bool isLowest = true;
            for (const std::pair<int, int>& nb: std::vector<std::pair<int, int>>{{i+1,j}, {i-1,j}, {i, j-1}, {i,j+1}})
            {
                if (v >= map.valueOf(nb.first, nb.second))
                {
                    isLowest = false;
                }
                
            }
            if (isLowest)
            {
                std::cout << i << ", " << j << std::endl;
                total += v+1;
            }
            ++j;
        }
        ++i;
    }
    std::cout << total << std::endl;
    
    
    return 0;
}
