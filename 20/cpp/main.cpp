#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Algo = std::vector<bool>;
std::ostream& operator<<(std::ostream& o, const std::vector<bool>& algo)
{
    for(const auto& p : algo)
    {
        std::cout << (p ? '#' : '.') << std::flush;
    }

    return o;
}

struct Pixels
{
    bool inverted = false;

    int minFirst  = 0;
    int maxFirst  = 0;
    int minSecond = 0;
    int maxSecond = 0;

    bool valueOf(const int f, const int s) const
    {
        auto finder = values.find({f, s});
        if(finder != values.end())
        {
            return inverted == false;
        }
        return inverted;
    }

    void emplace(const std::pair<int, int>& p, bool value)
    {
        if(value == inverted)
        {
            return;
        }
        if(values.empty())
        {
            minFirst  = p.first;
            maxFirst  = p.first;
            minSecond = p.second;
            maxSecond = p.second;
        }
        else
        {
            if(p.first < minFirst)
            {
                minFirst = p.first;
            }
            if(p.first > maxFirst)
            {
                maxFirst = p.first;
            }
            if(p.second < minSecond)
            {
                minSecond = p.second;
            }
            if(p.second > maxSecond)
            {
                maxSecond = p.second;
            }
        }
        values.emplace(p);
    }
    std::set<std::pair<int, int>> values;
};

Pixels applyAlgo(const Pixels& pixels, const Algo& algo)
{
    Pixels result;

    const bool inversedAlgo = algo.at(0);

    if(inversedAlgo)
    {
        result.inverted = (pixels.inverted == false);
    }
    else
    {
        result.inverted = pixels.inverted;
    }

    int d = 4;

    for(int first = pixels.minFirst - d; first <= pixels.maxFirst + d; ++first)
    {
        for(int second = pixels.minSecond - d; second <= pixels.maxSecond + d; ++second)
        {
            int pos = 0;
            for(int dFirst = -1; dFirst <= 1; ++dFirst)
            {
                for(int dSecond = -1; dSecond <= 1; ++dSecond)
                {
                    pos = pos << 1;
                    if(pixels.valueOf(first + dFirst, second + dSecond))
                    {
                        pos += 1;
                    }
                }
            }

            result.emplace({first, second}, algo.at(pos));
        }
    }

    return result;
}
std::ostream& operator<<(std::ostream& o, const Pixels& pixels)
{
    if(pixels.values.empty())
    {
        return o;
    }

    for(int first = pixels.minFirst - 2; first <= pixels.maxFirst + 2; ++first)
    {
        for(int second = pixels.minSecond - 2; second <= pixels.maxSecond + 2; ++second)
        {
            if(pixels.valueOf(first, second))
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

    return o;
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

    Algo algo;

    for(int i = 0; i < 512; ++i)
    {
        char c = infile.get();
        algo.emplace_back(c == '#');
    }
    std::cout << algo << std::endl;
    {
        std::string line;
        getline(infile, line);

        std::cout << line << std::endl;
    }

    int y = 0;
    int x = 0;

    Pixels pixels;

    while(infile.peek() != EOF)
    {
        char c = infile.get();
        if(c == '\n')
        {
            y = 0;
            ++x;
        }
        if(c == '#')
        {
            pixels.emplace({x, y}, true);
        }

        ++y;
    }
    std::cout << std::endl;

    for(int i = 0; i < 50; ++i)
    {
        if(i == 2)
        {
            std::cout << pixels.values.size() << std::endl;
        }
        pixels = applyAlgo(pixels, algo);
    }

    std::cout << pixels.values.size() << std::endl;

    return 0;
}
