#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

using int64 = int64_t;
using Line = std::string;
using LineIterator = Line::const_iterator;

char determineEndChar(const char d)
{
    if (d == '(')
    {
        return ')';
    }
    if (d == '[')
    {
        return ']';
    }
    if (d == '{')
    {
        return '}';
    }
    if (d == '<')
    {
        return '>';
    }
    return 'N';
}

void print(const LineIterator begin, const LineIterator end)
{
    std::cout << '"';
    auto iter = begin;
    while(iter != end)
    {
        std::cout << *iter << std::flush;
        ++iter;
    }
    std::cout << '"';
}

enum class LineCorrectness
{
    Complete,
    Incomplete,
    Incorrect
};

struct SeparationResult
{
    SeparationResult(LineCorrectness f) : success(f){}
    LineCorrectness success;
    LineIterator iter;
    std::vector<char> neededChars;
};

SeparationResult separate(const LineIterator begin, const LineIterator end)
{
    if (begin == end)
    {
        SeparationResult result(LineCorrectness::Complete);
        result.iter = begin;
        return result;
    }
    char beginChar = *begin;
    
    char endChar = determineEndChar(beginChar);
    if (endChar == 'N')
    {
        SeparationResult result(LineCorrectness::Incorrect);
        result.iter = begin;
        return result;
    }
    LineIterator iter = std::next(begin);

    if (endChar == *iter)
    {
        SeparationResult result(LineCorrectness::Complete);
        result.iter = std::next(iter);
        return result;
    }
    
    while (iter != end)
    {
        auto nextResult = separate(iter, end);
    
        if (nextResult.success == LineCorrectness::Incorrect)
        {
            return nextResult;
        }
        if (nextResult.iter == end)
        {
            SeparationResult result(LineCorrectness::Incomplete);
            result.neededChars = nextResult.neededChars;
            result.neededChars.emplace_back(endChar);
            result.iter = end;
            return result;
        }
    
        if (endChar == *nextResult.iter)
        {
            SeparationResult result(LineCorrectness::Complete);
            result.iter = std::next(nextResult.iter);
            return result;
        }
        
        iter = nextResult.iter;
    }

    SeparationResult result(LineCorrectness::Incomplete);
    result.neededChars.emplace_back(endChar);
    result.iter = end;
    return result;
}

struct SResult 
{
    LineCorrectness lineCorr = LineCorrectness::Complete;
    std::vector<char> neededChars;
};

SResult validLine(const LineIterator begin, const LineIterator end)
{
    auto iter = begin;
    
    while (iter != end)
    {
        auto sepResult = separate(iter, end);
        
        if (sepResult.success != LineCorrectness::Complete)
        {
            SResult r;
            r.neededChars = sepResult.neededChars;
            r.lineCorr = sepResult.success;
            return r;
        }
        iter = sepResult.iter;
    }
    SResult r;
    r.lineCorr = LineCorrectness::Complete; 
    return r;
}

SResult validLine(const Line& l)
{
    return validLine(l.begin(), l.end());
}


int64 scoreFunction(const char c)
{
    if (c == ')')
    {
        return 1;
    }
    if (c == ']')
    {
        return 2;
    }
    if (c == '}')
    {
        return 3;
    }
    if (c == '>')
    {
        return 4;
    }
    std::cerr << "Problem for " << c << std::endl;
    return 0;
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
    

    std::vector<int64> allScores;
    std::string line;
    while (infile >> line)
    {
        SResult r = validLine(line);
        if (r.lineCorr == LineCorrectness::Incomplete)
        {
            std::cout << line << std::endl; 
            
            int64 score = 0;
            for (const char c: r.neededChars)
            {
                score *= 5;
                score += scoreFunction(c);
                std::cout << "  " << c << std::flush;
            }
            std::cout << "   --> " << score << std::endl;
            
            allScores.emplace_back(score);
        }
    }
    std::sort(allScores.begin(), allScores.end());
    
    std::cout << allScores.at(allScores.size()/2) << std::endl;
    
    return 0;
}
