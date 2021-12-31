#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using int64         = std::int64_t;
using Input         = std::vector<bool>;
using InputIterator = Input::const_iterator;

int64 readHex(const char c)
{
    if('0' <= c && c <= '9')
    {
        return c - '0';
    }
    if('A' <= c && c <= 'Z')
    {
        return c - 'A' + 10;
    }

    return 0;
}

int64 readInt(InputIterator& iter, const int size)
{
    int64 result = 0;

    for(int j = 0; j < size; ++j)
    {
        result = result << 1;
        if(*iter)
        {
            result += 1;
        }
        ++iter;
    }

    return result;
}

int64 read(InputIterator& iter, int64& total)
{
    int64 packetVersion = readInt(iter, 3);

    total += packetVersion;

    int64 id = readInt(iter, 3);

    if(id == 4)
    {
        bool  stop  = false;
        int64 value = 0;
        while(stop == false)
        {
            int64 part = readInt(iter, 5);
            value      = (value << 4);
            value += (part & 15);

            stop = (part >> 4) == 0;
        }
        return value;
    }
    std::vector<int64> values;
    if(*iter)
    {
        ++iter;
        int64 numberOfSubpackets = readInt(iter, 11);

        for(int64 i = 0; i < numberOfSubpackets; ++i)
        {
            values.emplace_back(read(iter, total));
        }
    }
    else
    {
        ++iter;
        int64 lengthOfSubPackets = readInt(iter, 15);

        InputIterator endOfBlock = std::next(iter, lengthOfSubPackets);

        while(iter != endOfBlock)
        {
            values.emplace_back(read(iter, total));
        }
    }
    if(id == 0)
    {
        int64 result = 0;
        for(const int64 v : values)
        {
            result += v;
        }
        return result;
    }
    else if(id == 1)
    {
        int64 result = 1;
        for(const int64 v : values)
        {
            result *= v;
        }
        return result;
    }
    else if(id == 2)
    {
        int64 result = values.front();
        for(const int64 v : values)
        {
            result = std::min(v, result);
        }
        return result;
    }
    else if(id == 3)
    {
        int64 result = values.front();
        for(const int64 v : values)
        {
            result = std::max(v, result);
        }
        return result;
    }
    else if(id == 5)
    {
        return values.at(0) > values.at(1) ? 1 : 0;
    }
    else if(id == 6)
    {
        return values.at(0) < values.at(1) ? 1 : 0;
    }
    else if(id == 7)
    {
        return values.at(0) == values.at(1) ? 1 : 0;
    }

    return 0;
}

std::ostream& operator<<(std::ostream& o, const std::vector<bool>& f)
{
    for(const bool b : f)
    {
        if(b)
        {
            o << "1";
        }
        else
        {
            o << "0";
        }
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

    while(infile.peek() != EOF)
    {
        std::vector<bool> input;

        while(infile.peek() != '\n' && infile.peek() != EOF)
        {
            char c = infile.get();
            std::cout << c << std::flush;
            const int64 i = readHex(c);
            for(int j = 3; j >= 0; --j)
            {
                input.emplace_back(i & (1 << j));
            }
        }
        std::cout << std::endl;

        std::cout << input << std::endl;

        int64         total  = 0;
        InputIterator iter   = input.begin();
        int64         result = read(iter, total);
        std::cout << "  ----> " << total << std::endl;
        std::cout << "  ----> " << result << std::endl;
        if(infile.peek() == '\n')
        {
            infile.get();
        }
    }

    return 0;
}
