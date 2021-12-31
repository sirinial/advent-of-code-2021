#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using int64 = std::int64_t;

using Position = std::pair<int64, int64>;

enum class Herd
{
    East,
    South,
    Nothing
};

struct SecondFirstCompare
{
    bool operator()(const Position& lhs, const Position& rhs) const
    {
        return std::tie(lhs.second, lhs.first) < std::tie(rhs.second, rhs.first);
    }
};
using SeaCucumbers = std::map<Position, Herd, SecondFirstCompare>;

std::ostream& operator<<(std::ostream& o, const SeaCucumbers& s)
{
    if(s.empty())
    {
        return o;
    }
    int64 lastSecond = s.begin()->first.second;

    for(const auto& entry : s)
    {
        if(entry.first.second != lastSecond)
        {
            o << std::endl;
            lastSecond = entry.first.second;
        }
        if(entry.second == Herd::Nothing)
        {
            o << ".";
        }
        else if(entry.second == Herd::East)
        {
            o << ">";
        }
        else if(entry.second == Herd::South)
        {
            o << "v";
        }
    }
    return o;
}

Position direction(const Herd h)
{
    if(h == Herd::East)
    {
        return {1, 0};
    }
    if(h == Herd::South)
    {
        return {0, 1};
    }
    return {0, 0};
}

SeaCucumbers doStepForHerd(const Herd h, const SeaCucumbers& animals)
{
    SeaCucumbers result = animals;

    const Position directionOfH = direction(h);

    for(const auto& entry : animals)
    {
        if(entry.second == h)
        {
            Position target = {entry.first.first + directionOfH.first, entry.first.second + directionOfH.second};
            if(animals.find(target) == animals.end())
            {
                if(directionOfH.first != 0)
                {
                    target.first = 0;
                }
                else
                {
                    target.second = 0;
                }
            }

            if(animals.at(target) == Herd::Nothing)
            {
                result.at(entry.first) = Herd::Nothing;
                result.at(target)      = h;
            }
        }
    }

    return result;
}

SeaCucumbers doStep(const SeaCucumbers& animals)
{
    return doStepForHerd(Herd::South, doStepForHerd(Herd::East, animals));
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

    SeaCucumbers animals;
    Position     pos = {0, 0};
    while(infile.peek() != EOF)
    {
        const char c = infile.get();

        if(c == '\n')
        {
            ++pos.second;
            pos.first = 0;
        }
        else
        {
            if(c == '.')
            {
                animals.emplace(pos, Herd::Nothing);
            }
            else if(c == '>')
            {
                animals.emplace(pos, Herd::East);
            }
            else if(c == 'v')
            {
                animals.emplace(pos, Herd::South);
            }
            else
            {
                std::cerr << "Was not able to read " << c << std::endl;
            }

            ++pos.first;
        }
    }

    int i = 0;

    while(true)
    {
        std::cout << i << std::endl;

        std::cout << animals << std::endl;

        SeaCucumbers newAnimals = doStep(animals);
        if(animals == newAnimals)
        {
            return 0;
        }

        animals = newAnimals;

        ++i;
    }

    return 0;
}
