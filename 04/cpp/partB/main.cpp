#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

std::ostream& operator<<(std::ostream& o, const std::pair<uint, uint>&pos)
{
    o << "(" << pos.first << ", " << pos.second << ")";
    return o;
}

struct BingoSheet
{
    std::vector<std::vector<int>> values;
    
    std::pair<uint, uint> size() const
    {
        std::pair<uint, uint> res{0,0};
        
        res.first = values.size();
        for (const std::vector<int>& l : values)
        {
            if (res.second < l.size())
            {
                res.second = l.size();
            }
        }
        return res;
    }
    
    int sums() const
    {
        int result = 0;
        for (const std::vector<int>& l : values)
        {
            for(const int k : l)
            {
                if (k != -1)
                {
                    result += k;
                }
            }
        }
        return result;   
    }
    
    bool containsAllMinusOnes(const std::pair<uint,uint> startingPos, const std::pair<uint,uint> dir) const
    {
        std::pair<uint, uint> pos = startingPos;
        
        std::cout << "-> Checking " << pos << " --> " << dir << std::endl;
        
        while (pos.first < values.size() && pos.second < values.at(pos.first).size())
        {
            std:: cout << "   * " << pos << ": ";
            const int l = values.at(pos.first).at(pos.second);
            std::cout << l << std::endl;
            if (l != -1)
            {
                return false;
            }
            pos.first += dir.first;
            pos.second += dir.second;
        }
        return true;
    }
    
    bool wins() const
    {
        for (uint i = 0; i < values.front().size(); ++i)
        {
            if (containsAllMinusOnes({0,i}, {1,0}))
            {
                return true;            }
        }
        for (uint i = 0; i < values.size(); ++i)
        {
            if (containsAllMinusOnes({i,0}, {0,1}))
            {
                return true;
            }
        }

        return false;
    }
    
    void mark(const int i)
    {
        for (std::vector<int>& l : values)
        {
            for(int& k : l)
            {
                if (k == i)
                {
                    k = -1;
                }
            }
        }
    }
};

std::ostream& operator<<(std::ostream& o, const BingoSheet& b)
{
    for (const std::vector<int>& l : b.values)
    {
        for (const int i : l)
        {
            if (i != -1)
            {
                o << " " << (i > 9 ? "" : " ") << i << " " << std::flush;
            }
            else
            {
                o << "   ";
            }
        }
        o << std::endl;
    }

    return o;
}
std::ostream& operator<<(std::ostream& o, const std::vector<BingoSheet>& sheets)
{
    for (const auto& s : sheets)
    {
        o << s << std::endl;
    }
    return o;
}

std::vector<int> readValues(const std::string& line)
{
    std::vector<int> result;
    
    std::stringstream ss(line);
    
    for (int i; ss >> i;)
    {
        result.emplace_back(i);
        if (ss.peek() == ',')
        {
            ss.ignore();
        }
    }
    
    return result;
}

BingoSheet readSheet(std::ifstream& infile)
{
    BingoSheet result;

    int  val;
    
    std::vector<int> line;
    while (infile >> val)
    {
        line.emplace_back(val);
        
        char c2 = infile.peek();
        if (c2 == '\n')
        {
            result.values.emplace_back(line);
            line = {};
            infile.get();
            
            c2 = infile.peek();
            if (c2 == '\n')
            {
                return result;
            }
        }
    }
    if (line.empty() == false)
    {
        result.values.emplace_back(line);
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    const std::string fileName = "../../input";

    std::ifstream infile(fileName);   
    std::cout << "Reading " << fileName << std::endl;
    
    std::vector<int> values;
    
    {
        std::string firstLine;
        infile >> firstLine;
        values = readValues(firstLine);
    }
    
    std::vector<BingoSheet> sheets;
    
    {
        while (infile.peek() != EOF)
        {
            sheets.emplace_back(readSheet(infile));
        }
    }
    
    for (const BingoSheet& sh: sheets)
    {
        std::cout << sh << std::endl;
        const auto s = sh.size();
        if (s.first != s.second)
        {
            return 1;
        }
    }
    
    
    for (const int i : values)
    {
        std::cout << "-> Doing " << i << std::endl;
        
        std::vector<BingoSheet> newSheets, winningSheets;
        
        for (BingoSheet& sh: sheets)
        {
            sh.mark(i);
            
            if (sh.wins() == false)
            {
                newSheets.emplace_back(sh);
            }
            else
            {
                winningSheets.emplace_back(sh);
            }
        }
        sheets = std::move(newSheets);
        
        if (sheets.empty())
        {
            const BingoSheet& sh = winningSheets.front();    
            
            std::cout << sh.sums() << " * " << i << ": " << (sh.sums() * i) << std::endl;
                
            std::cout << sh.size() << std::endl;
                
            std::cout << sh << std::endl;
            
            return 0;
        }
        
        std::cout << sheets;
    }
    
    return 0;
}
