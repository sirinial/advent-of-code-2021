#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <set>

using Signal = std::string;
using Signals = std::vector<Signal>;

Signals readValues(std::istream& instream)
{
    std::vector<std::string> result;
    
    std::string l;
    
    while(instream.peek() != '\n' && instream >> l && l != "|")
    {
         result.emplace_back(l);  
    }
    if (instream.peek() == '\n')
    {
        instream.get();
    }
    return result;
}

struct Entry
{
    Signals uniqueSignals;
    Signals output;
};

std::ostream& operator<<(std::ostream& o, const Signals& s)
{
    auto iter = s.begin();
    while (iter != s.end())
    {
        if (iter != s.begin())
        {
            o << " ";
        }
        o << *iter;        
        ++iter;
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const Entry& entry)
{
    o << entry.uniqueSignals << " | " << entry.output;
    return o;
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
    
    std::set<int> wantedSizes{3,2,7,4};
    
    std::vector<Entry> entries;
    
    int total = 0;
    while (infile.peek() != EOF)
    {
        Entry e;
        e.uniqueSignals = readValues(infile);
        e.output = readValues(infile);
        
        std::cout << "Read " << e << std::endl;;
        
        int nb{0};
        
        for (const Signal& s: e.output)
        {
            std::cout << "   - " << s << std::endl;
            
            auto finder = wantedSizes.find(s.size());
            
            if (finder != wantedSizes.end())
            {
                ++nb;
            }
        }
        std::cout << "     " << nb << std::endl;        
        total += nb;
        
        entries.emplace_back(e);
    }
    
    std::cout << "Total: " << total << std::endl;
    
    
    return 0;
}
