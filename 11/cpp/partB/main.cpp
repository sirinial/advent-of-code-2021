
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <set>

using Pos = std::pair<int, int>;

std::ostream& operator<<(std::ostream& o, const Pos& p)
{
    o << "(" << p.first << "," << p.second << ")";
    return o;
}
struct Map
{
    std::vector<std::vector<int>> values;
    
    void increaseAllByOne()
    {
        for (auto& entry: values)
        {
            for (int& a: entry)
            {
                ++a;
            }
        }
    }
    
    std::set<Pos> getAllTens(const std::set<Pos>& excludes) const
    {
        std::set<Pos> result;    
        int i = 0; 
        for (const auto& entry: values)
        {
            int j = 0;
            for (const int& a: entry)
            {
                 if (a == 10)
                 {
                     if (excludes.find({i,j}) == excludes.end())
                     {
                         result.emplace(i,j);
                     }
                 }
                 ++j;
            }
            ++i;
        }
        
        return result;
    }
    
    void setZero(const Pos& p)
    {
        values.at(p.first).at(p.second) = 0;
    }
    
    void increase(const Pos& p)
    {
        if (static_cast<int>(values.size()) <= p.first ||  p.first < 0)
        {
            return;
        }
        std::vector<int>& elements = values.at(p.first);
        if (static_cast<int>(elements.size()) <= p.second || p.second < 0)
        {
            return;
        }
        int& l = elements.at(p.second);
        if (l < 10)
        {
            ++l;
        }
    }
    
    void flash(const Pos& p)
    {
        std::cout << " -> Flashing " << p << std::endl;
        for (int i = -1; i < 2; ++i)
        {
            for(int j = -1; j <2; ++j)
            {
                increase({p.first + i, p.second + j});        
            }
        }
    }
    
    int doStep()
    {
        increaseAllByOne();
        
        std::set<Pos> flashedElements;
        
        std::set<Pos> toFlash = getAllTens(flashedElements);
        
        while (toFlash.empty() == false)
        {
            for (const Pos& p : toFlash)
            {
                flashedElements.emplace(p);
                flash(p);
            }
            toFlash = getAllTens(flashedElements);
        }
        
         for (const Pos& p : flashedElements)
         {
             setZero(p);
         }   
         
         return static_cast<int>(flashedElements.size());
    }
};

std::ostream& operator<<(std::ostream& o, const Map& m)
{
    for (const auto& l: m.values)
    {
        for (const auto& v : l)
        {
            o << " " << v;
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
    
    for (int i = 0; true; )
    {
        ++i;
        if ( map.doStep() == 100)
        {
            std::cout << i << " does the trick" << std::endl;
            return 0;
        }
        std::cout << map << std::endl;
    }
    return 0;
}
