#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct Pos
{
    Pos() = default;
    Pos(int dx, int dy)
        : x(dx)
        , y(dy)
    {}
    int x, y;

    bool operator<(const Pos& p) const
    {
        return std::tie(x, y) < std::tie(p.x, p.y);
    }
};

using Positions = std::set<Pos>;

std::ostream& operator<<(std::ostream& o, const Pos& p)
{
    o << "(" << p.x << "," << p.y << ")";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Positions& elements)
{
    o << "{ ";
    for(const Pos& p : elements)
    {
        o << p << " ";
    }
    o << "}";
    return o;
}

void print(std::ostream& o, const Positions& elements)
{
    if(elements.empty())
    {
        return;
    }

    Pos min = *elements.begin();
    Pos max = min;
    for(const Pos& p : elements)
    {
        if(p.x < min.x)
        {
            min.x = p.x;
        }
        if(p.x > max.x)
        {
            max.x = p.x;
        }
        if(p.y < min.y)
        {
            min.y = p.y;
        }
        if(p.y > max.y)
        {
            max.y = p.y;
        }
    }

    for(int y = min.y; y <= max.y; ++y)
    {
        for(int x = min.x; x <= max.x; ++x)
        {
            if(elements.find(Pos(x, y)) != elements.end())
            {
                o << '#';
            }
            else
            {
                o << '.';
            }
        }
        o << std::endl;
    }
}

std::istream& operator>>(std::istream& istream, Pos& p)
{
    char c;
    istream >> p.x >> c >> p.y;
    return istream;
}
Positions transform(const Positions& elements, const std::function<Pos(Pos)>& f)
{
    Positions result;
    for(const Pos& p : elements)
    {
        result.emplace(f(p));
    }
    return result;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Missing argument!" << std::endl;
        return 1;
    }
    const std::string fileName = argv[1];

    std::ifstream infile(fileName);
    std::cout << "Reading " << fileName << std::endl;

    std::set<Pos> positions;

    Pos p;
    while(infile.peek() != '\n' && infile >> p)
    {
        positions.emplace(p);

        if(infile.peek() == '\n')
        {
            infile.get();
        }
    }
    std::string line;
    while(infile >> line)
    {
        if(line == "fold")
        {
            char axis, equal;
            int  el;
            infile >> line >> axis >> equal >> el;

            if(axis == 'x')
            {
                positions = transform(positions, [el](const Pos& p) {
                    Pos result;
                    result.y = p.y;
                    if(p.x < el)
                    {
                        result.x = p.x;
                    }
                    else
                    {
                        result.x = 2 * el - p.x;
                    }
                    return result;
                });
            }
            else if(axis == 'y')
            {
                positions = transform(positions, [el](const Pos& p) {
                    Pos result;
                    result.x = p.x;
                    if(p.y < el)
                    {
                        result.y = p.y;
                    }
                    else
                    {
                        result.y = 2 * el - p.y;
                    }
                    return result;
                });
            }
            else
            {
                return 1;
            }
        }
    }

    print(std::cout, positions);
    return 0;
}
