#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

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

struct SeparationResult
{
    SeparationResult(bool f) : success(f){}
    bool success;
    LineIterator iter;
    char c = 'W';
};

SeparationResult separate(const LineIterator begin, const LineIterator end)
{
    if (begin == end)
    {
        SeparationResult result(true);
        result.iter = begin;
        return result;
    }
    char beginChar = *begin;
    
    char endChar = determineEndChar(beginChar);
    if (endChar == 'N')
    {
        SeparationResult result(false);
        result.c = beginChar;
        result.iter = begin;
        return result;
    }
    LineIterator iter = std::next(begin);

    if (endChar == *iter)
    {
        SeparationResult result(true);
        result.iter = std::next(iter);
        return result;
    }
    
    while (iter != end)
    {
        auto nextResult = separate(iter, end);
    
        if (nextResult.success == false)
        {
            return nextResult;
        }
        if (nextResult.iter == end)
        {
            SeparationResult result(true);
            result.iter = end;
            return result;
        }
    
        if (endChar == *nextResult.iter)
        {
            SeparationResult result(true);
            result.iter = std::next(nextResult.iter);
            return result;
        }
        
        iter = nextResult.iter;
    }

    SeparationResult result(true);
    result.iter = end;
    return result;
}

struct SResult 
{
    bool success = true;
    char c;
};

SResult validLine(const LineIterator begin, const LineIterator end)
{
    auto iter = begin;
    
    while (iter != end)
    {
        auto sepResult = separate(iter, end);
        
        if (sepResult.success == false)
        {
            SResult r;
            r.success = false;
            r.c = sepResult.c;
            return r;
        }
        iter = sepResult.iter;
    }
    SResult r;
    r.success = true; 
    return r;
}

SResult validLine(const Line& l)
{
    return validLine(l.begin(), l.end());
}


int errorScore(const char c)
{
    if (c == ')')
    {
        return 3;
    }
    if (c == ']')
    {
        return 57;
    }
    if (c == '}')
    {
        return 1197;
    }
    if (c == '>')
    {
        return 25137;
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
    
    int errorScoreTotal = 0;
    
    std::string line;
    while (infile >> line)
    {
        SResult r = validLine(line);
        if (r.success == false)
        {
            std::cout << line << ": " << r.c << std::endl; 
            errorScoreTotal += errorScore(r.c);
        }
    }
    std::cout << "Error score total: " << errorScoreTotal << std::endl;    
    
    return 0;
}
