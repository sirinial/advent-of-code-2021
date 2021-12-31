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

    std::cout << polymer << std::endl;

    Transformator transf;

    for(int i = 1; i <= 10; ++i)
    {
        std::cout << i << std::endl;
        polymer = applyTransformatorToString(polymer, transformator);

        std::cout << "   " << polymer.size() << std::endl;
    }

    std::cout << "===========================" << std::endl;
    std::cout << polymer.size() << std::endl;

    std::map<char, int> counter;

    for(char c : polymer)
    {
        counter.emplace(c, 0);
        ++counter[c];
    }
    int min = counter.begin()->second;
    int max = min;

    for(const auto& el : counter)
    {
        if(el.second < min)
        {
            min = el.second;
        }
        if(el.second > max)
        {
            max = el.second;
        }
    }
    std::cout << (max - min) << std::endl;

    return 0;
}
