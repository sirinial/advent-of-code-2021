#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using CharPair = std::pair<char, char>;

using Transformator = std::map<CharPair, std::string>;

std::string applyTransformatorToString(const std::string& polymer, const Transformator& transformator)
{
    std::string newPolymer = polymer.substr(0, 1);

    auto iter     = polymer.begin();
    auto nextIter = std::next(iter);
    while(nextIter != polymer.end())
    {
        if(transformator.find({*iter, *nextIter}) != transformator.end())
        {
            newPolymer += transformator.at({*iter, *nextIter});
        }

        newPolymer += +*nextIter;
        ++iter;
        ++nextIter;
    }
    return newPolymer;
}

struct SCount
{
    std::int64_t nb{0};
};

struct SCountPerChar
{
    SCountPerChar() = default;
    SCountPerChar(const std::string& s)
    {
        for(const char c : s)
        {
            ++perChar[c].nb;
        }
    }

    std::map<char, SCount> perChar;

    SCountPerChar& operator+=(const SCountPerChar& c)
    {
        for(const auto& el : c.perChar)
        {
            perChar[el.first].nb += el.second.nb;
        }
        return *this;
    }

    std::int64_t diffOfMaxAndMin() const
    {
        std::int64_t min = perChar.begin()->second.nb;
        std::int64_t max = min;

        for(const auto& el : perChar)
        {
            if(el.second.nb < min)
            {
                min = el.second.nb;
            }
            if(el.second.nb > max)
            {
                max = el.second.nb;
            }
        }
        return (max - min);
    }
};

std::ostream& operator<<(std::ostream& o, const SCountPerChar& c)
{
    o << "{ ";

    for(const auto& el : c.perChar)
    {
        o << el.first << ": " << el.second.nb << " ";
    }
    o << "}";
    return o;
}

using Cache = std::map<std::pair<int, std::string>, SCountPerChar>;

SCountPerChar countResult(const std::string& polymer, const int depth, const Transformator& trans, Cache& cache)
{
    if(depth <= 0)
    {
        return SCountPerChar(polymer);
    }
    std::pair<int, std::string> cacheKey = {depth, polymer};

    auto finder = cache.find(cacheKey);
    if(finder != cache.end())
    {
        return finder->second;
    }

    SCountPerChar& result = cache[cacheKey];

    auto iter     = polymer.begin();
    auto nextIter = std::next(iter);

    ++result.perChar[*iter].nb;

    while(nextIter != polymer.end())
    {
        std::string s = *iter + trans.at({*iter, *nextIter}) + *nextIter;
        result += countResult(s, depth - 1, trans, cache);

        --result.perChar[*iter].nb;
        ++iter;
        ++nextIter;
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

    std::string polymer;
    infile >> polymer;

    Transformator transformator;
    std::string   line;
    while(infile >> line)
    {
        CharPair p = {line.at(0), line.at(1)};

        infile >> line >> line;

        transformator.emplace(p, line);
    }

    std::cout << transformator.size() << std::endl;

    Cache         cache;
    SCountPerChar result = countResult(polymer, 40, transformator, cache);
    std::cout << "Count result: " << polymer << " - " << 40 << ": " << result << std::endl;

    std::cout << result.diffOfMaxAndMin() << std::endl;

    return 0;
}
