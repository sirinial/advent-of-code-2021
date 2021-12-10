#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <set>

using Pos = std::pair<int,int>;
using PosSet = std::set<Pos>;

PosSet nbs(const Pos& p)
{
    int i = p.first;
    int j = p.second;
    return PosSet{{i+1,j}, {i-1,j}, {i, j-1}, {i,j+1}};
}

std::ostream& operator<<(std::ostream& o, const Pos& p)
{
    o << "(" << p.first << "," << p.second << ")";
    return o;
}

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
    
    
    PosSet bassinOf(const Pos& ps) const
    {
        PosSet toDo{ps};
        PosSet done;
        
        int round = 0;
        while (toDo.empty() == false)
        {
            std::cout << "Round " << round << std::endl;
            PosSet newTodo;
            for (const Pos& p: toDo)
            {
                const int v = valueOf(p.first, p.second);
                
                std::cout << "   - Trying " << p << " with value " << v << std::endl;
                for (const Pos& nb: nbs(p))
                {
                    const int w = valueOf(nb.first, nb.second);
                    std::cout << "      * Nb " << nb << " with value " << w << std::endl;
                    if (w < 9  && v < w && done.find(nb) == done.end() && toDo.find(nb) == toDo.end())
                    {
                        std::cout << "    --> Adding to todo" << std::endl;
                        newTodo.emplace(nb);
                    }
                }
                done.emplace(p);                
            }
            toDo = newTodo;  
        }
        return done;
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
    
    std::vector<int> bassinSizes;
    
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
                
                const PosSet bassin =  map.bassinOf({i,j});
                
                std::cout << bassin.size() << std::endl;
                
                bassinSizes.emplace_back(bassin.size());
                
            }
            ++j;
        }
        ++i;
    }
    std::sort(bassinSizes.begin(), bassinSizes.end());
    std::reverse(bassinSizes.begin(), bassinSizes.end());
    
    
    int result = bassinSizes.at(0) * bassinSizes.at(1) * bassinSizes.at(2);
    std::cout << result << std::endl;
    
    
    return 0;
}
