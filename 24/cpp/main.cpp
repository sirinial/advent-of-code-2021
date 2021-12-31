#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#define FOREACHINPUT(X) for(int64 X = 9; X >= 1; --X)

using int64 = std::int64_t;
using Input = std::vector<int64>;

struct State
{
    int64 x = 0;
    int64 y = 0;
    int64 z = 0;
    int64 w = 0;

    int64& retrieveValueReference(std::string c)
    {
        if(c == "x")
        {
            return x;
        }
        if(c == "y")
        {
            return y;
        }
        if(c == "z")
        {
            return z;
        }
        return w;
    }
    int64 retrieveValue(std::string c)
    {
        if(c == "x")
        {
            return x;
        }
        if(c == "y")
        {
            return y;
        }
        if(c == "z")
        {
            return z;
        }
        if(c == "w")
        {
            return w;
        }
        return std::stoi(c);
    }
};

int64 calculate(const Input& input, const std::string fileName)
{
    auto iter = input.begin();

    std::ifstream infile(fileName);
    State         currentState;

    std::string cmd, s;
    int64       count = 1;

    while(infile >> cmd)
    {
        ++count;
        if(cmd == "inp")
        {
            infile >> s;

            currentState.retrieveValueReference(s) = *iter;
            ++iter;
        }
        else
        {
            infile >> s;
            int64  lhs    = currentState.retrieveValue(s);
            int64& result = currentState.retrieveValueReference(s);

            infile >> s;
            int64 rhs = currentState.retrieveValue(s);

            if(cmd == "add")
            {
                result = lhs + rhs;
            }
            else if(cmd == "mul")
            {
                result = lhs * rhs;
            }
            else if(cmd == "div")
            {
                result = lhs / rhs;
            }
            else if(cmd == "mod")
            {
                result = lhs % rhs;
            }
            else if(cmd == "eql")
            {
                result = lhs == rhs ? 1 : 0;
            }
            else
            {
                std::cerr << "Was not able to process " << cmd << std::endl;
                return 1;
            }
        }
    }
    return currentState.z;
}

std::set<Input> nbs(const Input& input)
{
    std::set<Input> result;

    for(uint i = 0; i < input.size(); ++i)
    {
        for(int64 k = 1; k <= 9; ++k)
        {
            Input el = input;
            el.at(i) = k;
            result.emplace(el);
        }
    }
    return result;
}

std::set<Input> nbsOfNbs(const Input& input)
{
    std::set<Input> result;
    for(const Input& a : nbs(input))
    {
        result.emplace(a);
        for(const Input& b : nbs(a))
        {
            result.emplace(b);
            for(const Input& c : nbs(b))
            {
                result.emplace(c);
            }
        }
    }

    return result;
}
int64 distanceToZero(const Input& input)
{
    int64 result = 0;

    for(const int64 a : input)
    {
        result += std::abs(a);
    }

    return result;
}

std::ostream& operator<<(std::ostream& o, const Input& input)
{
    for(const int64 a : input)
    {
        o << a;
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

    std::vector<Input> inputVector;

    inputVector.emplace_back(Input{9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9});

    inputVector.emplace_back(Input{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    for(auto& input : inputVector)
        {
            int64 currentValue = calculate(input, fileName);

            for(int i = 0; i < 25 && currentValue != 0; ++i)
            {
                for(const Input& nb : nbs(input))
                {
                    int64 c = calculate(nb, fileName);

                    if(c < currentValue)
                    {
                        input        = nb;
                        currentValue = c;
                    }
                }
                std::cout << currentValue << "  ---    " << input << std::endl;
            }
            std::cout << std::endl;
        }

    return 0;
}
