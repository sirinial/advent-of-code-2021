#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using Direction = std::pair<int, int>;
using Pos       = std::pair<int, int>;
using Window    = std::pair<Pos, Pos>;

std::ostream& operator<<(std::ostream& o, const Pos& ps)
{
    o << "(" << ps.first << "," << ps.second << ")";
    return o;
}
std::ostream& operator<<(std::ostream& o, const Window& ps)
{
    o << "[" << ps.first << "," << ps.second << "]";
    return o;
}

Pos& operator+=(Pos& p, const Direction& dir)
{
    p.first += dir.first;
    p.second += dir.second;
    return p;
}

Window getDirectionWindow(const Window& win)
{
    Window result;

    result = win;
    if(win.first.first > 0)
    {
        result.first.first = 1;
    }
    else if(win.second.first < 0)
    {
        result.second.first = -1;
    }
    if(win.first.second > 0)
    {
        result.first.second = 1;
    }
    else if(win.second.second < 0)
    {
        result.first.second  = win.first.second;
        result.second.second = std::max(-win.second.second, -win.first.second);
    }

    return result;
}

bool liesInWindow(const Pos& p, const Window& win)
{
    return (win.first.first <= p.first) && (win.first.second <= p.second) && (win.second.first >= p.first) && (win.second.second >= p.second);
}

bool hits(Direction dir, const Window& win, int& highestY)
{
    Pos pos  = {0, 0};
    highestY = 0;
    while(pos.second >= win.first.second)
    {
        pos += dir;
        if(pos.second > highestY)
        {
            highestY = pos.second;
        }

        if(dir.first < 0)
        {
            ++dir.first;
        }
        else if(dir.first > 0)
        {
            --dir.first;
        }
        --dir.second;
        if(liesInWindow(pos, win))
        {
            return true;
        }
    }
    return false;
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

    std::string line;
    infile >> line >> line;

    Window window;

    infile.get();
    infile.get();
    infile.get();
    infile >> window.first.first;

    infile.get();
    infile.get();

    infile >> window.second.first;

    infile.get();
    infile.get();
    infile.get();
    infile.get();

    infile >> window.first.second;

    infile.get();
    infile.get();

    infile >> window.second.second;

    std::cout << window << std::endl;

    const Window dirWindow = getDirectionWindow(window);

    std::cout << "Searching in " << dirWindow << std::endl;
    int count{0};
    int highestOveral = 0;
    for(int i = dirWindow.first.first; i <= dirWindow.second.first; ++i)
    {
        for(int j = dirWindow.first.second; j <= dirWindow.second.second; ++j)
        {
            int       highestY;
            Direction dir = {i, j};
            if(hits(dir, window, highestY))
            {
                if(highestY > highestOveral)
                {
                    highestOveral = highestY;
                }
                ++count;
            }
        }
    }

    std::cout << "---> " << highestOveral << std::endl;
    std::cout << "---> " << count << std::endl;
    return 0;
}
