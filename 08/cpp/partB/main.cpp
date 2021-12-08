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

using SignalSet = std::set<Signal>;

std::ostream& operator<<(std::ostream& o, const std::set<Signal>& s)
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

bool contains(const Signal& s, const Signal& o)
{
    for (const char c: s)
    {
        if (std::find(o.begin(), o.end(), c) == o.end())
        {
            return false;
        }
    }
    return true;
}

Signal determineContaining(const std::set<Signal>& poss, const Signal& s)
{
    for (const Signal& p : poss)
    {
        if (contains(s, p))
        {
            return p;
        }
    }
    return "NOT FOUND";    
}

Signal determineContained(const std::set<Signal>& poss, const Signal& s)
{
    SignalSet result;
    for (const Signal& p : poss)
    {
        if (contains(p, s))
        {
            return p;
        }
    }
    return "NOT FOUND";
}

Signal determineOther(const std::set<Signal>& poss, const Signal& s, const Signal& t)
{
    for (const Signal& p : poss)
    {
        if (p != s && p != t)
        {
            return p;
        }
    }
    return "NOT FOUND";
}


void sort(Signal& s)
{
    std::sort(s.begin(), s.end());
}

void sort(Signals& s)
{
    for(Signal& e: s)
    {
        sort(e);
    }
}

void sort(Entry& e)
{
    sort(e.uniqueSignals);
    sort(e.output);
}

void removeSignal(const Signal& s, std::map<int, std::set<Signal>>& signalsPerSize)
{
    auto finder = signalsPerSize.find(s.size());
    if (finder != signalsPerSize.end())
    {
        auto iter = finder->second.find(s);
        if (iter != finder->second.end())
        {
            finder->second.erase(iter);
        }
        if (finder->second.empty())
        {
            signalsPerSize.erase(finder);
        }
    }
}

std::map<Signal, int> determineSignals(const Signals& uniques)
{
    std::map<int, std::set<Signal>> signalsPerSize;
    
    for (const Signal& s: uniques)
    {
        signalsPerSize[s.size()].emplace(s);
    }
    
    Signal seven = *signalsPerSize.at(3).begin();
    Signal one = *signalsPerSize.at(2).begin();
    Signal four = *signalsPerSize.at(4).begin();
    Signal eight = *signalsPerSize.at(7).begin();

    Signal three = determineContaining(signalsPerSize.at(5), one);
    Signal nine  = determineContaining(signalsPerSize.at(6), four);
    
    removeSignal(three, signalsPerSize);
    removeSignal(nine, signalsPerSize);
    
    Signal zero = determineContaining(signalsPerSize.at(6), one);
    Signal six =  determineOther(signalsPerSize.at(6), zero, nine);

    
    Signal five = determineContained(signalsPerSize.at(5), nine);
    Signal two = determineOther(signalsPerSize.at(5), five, three);
    
    return {{zero, 0},
            {one, 1},
            {two, 2},
            {three, 3},
            {four, 4},
            {five, 5},
            {six, 6},
            {seven, 7},
            {eight, 8},
            {nine, 9}};    
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
    
    
    int total = 0;
    while (infile.peek() != EOF)
    {
        Entry e;
        e.uniqueSignals = readValues(infile);
        e.output = readValues(infile);
        sort(e);
        
        std::cout << "Read " << e << std::endl;
        
        std::map<Signal, int> decoder = determineSignals(e.uniqueSignals);
        
        int nb{0};
        
        std::cout << "      " << e.output << std::endl;
        
        for (Signal& s: e.output)
        {
            sort(s);

            nb *= 10;
            
            auto finder = decoder.find(s);
            
            if (finder != decoder.end())
            {
                nb += finder->second;
            }
            else
            {
                std::cerr << "Exiting" << std::endl;
                return 1;
            }
        }
        std::cout << "     " << nb << std::endl;        
        total += nb;
    }
    
    std::cout << "Total: " << total << std::endl;
    
    
    return 0;
}
