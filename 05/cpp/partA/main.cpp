#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>


struct Coord
{
    int x;
    int y;
    
    Coord() = default;
    Coord(int sX, int sY) : x(sX), y(sY) {}
    
    Coord operator+(const Coord& other) const
    {
         return Coord(x + other.x, y + other.y);
    }
    
    bool operator==(const Coord& other) const
    {
        return std::tie(x,y) == std::tie(other.x, other.y);
    }
    
    bool operator<(const Coord& other) const
    {
        return std::tie(x,y) < std::tie(other.x, other.y);
    }
    bool operator!=(const Coord& other) const
    {
        return std::tie(x,y) != std::tie(other.x, other.y);
    }
    
    int absDistance(const Coord& other) const
    {
        return std::abs(x - other.x) + std::abs(y-other.y);
    }
};

std::istream& operator>> (std::istream& is, Coord& coord)
{
    char c;
    is >> coord.x >> c >> coord.y;    
    return is;
}

std::ostream& operator<<(std::ostream& o, const Coord& coord)
{
    o << coord.x << "," << coord.y;
    return o;
}
struct Count
{
    int co{0};
    
    void increase()
    {
        ++co;
    }
};

struct Line
{
    Coord start;
    Coord finish;
    
    bool isHorizontal() const
    {
        return start.y == finish.y;
    }
    
    bool isVertical() const
    {
        return start.x == finish.x;
    }
};

std::istream& operator>> (std::istream& is, Line& line)
{
    char c;
    is >> line.start >> c >> c >> line.finish; 
    return is;
}
std::ostream& operator<<(std::ostream& o, const Line& line)
{
    o << line.start << " -> " << line.finish;
    return o;
}

Coord moveTo(const Coord& co, const Coord& target)
{
    Coord result = co;
    if (co.x != target.x)
    {
        if (co.x < target.x)
        {
            ++result.x;
        }
        else
        {
             --result.x;
        }
    }
    if (co.y != target.y)
    {
        if (co.y < target.y)
        {
            ++result.y;
        }
        else
        {
             --result.y;
        }
    }
    return result;
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
    
    
    std::map<Coord, Count> counter;
    
    Line l;
    while (infile >> l)
    {
        if (l.isVertical() == false && l.isHorizontal() == false)
        {
            continue;
        }
        std::cout << "Doing " << l << std::endl;
        Coord co = l.start;
        
        counter[co].increase();
        
        while (co != l.finish)
        {
            std::cout << " - " << co << std::endl;
            co = moveTo(co, l.finish);
            
            counter[co].increase();
        }        
    }
    
    std::cout << "Solution:" << std::endl;
    Count count;
    for (const auto& entry: counter)
    {
        if (entry.second.co > 1)
        {
             count.increase();
             std::cout << entry.first << ": " << entry.second.co << std::endl;
        }
    }
    std::cout << count.co << std::endl;
    
    
    return 0;
}
