#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>

struct BitCount
{
    int zeroes{0};
    int ones{0};
    
    char characterMostFound() const
    {
        if (zeroes < ones)
        {
            return '1';
        }
        else if (zeroes > ones)
        {
            return '0';
        }
        return 'a';
    }
    char characterLeastFound() const
    {
        if (zeroes < ones)
        {
            return '0';
        }
        else if (zeroes > ones)
        {
            return '1';
        }
        return 'a';
    }
};

struct Counter
{
    std::map<int, BitCount, std::greater<int>> map;
};

Counter count(const std::vector<std::string>& lines)
{
    Counter count;
    
    for(std::string word : lines)
    {
        int pos = 0;
        for (char c : word)
        {
            if (c == '0')
            {
                ++count.map[pos].zeroes;
            }
            else if (c == '1')
            {
                ++count.map[pos].ones;
            }        
            ++pos;
        }
    }
    return count;
}


int main(int argc, char *argv[])
{
    const std::string fileName = "../../input";

    std::ifstream infile(fileName);   
    std::cout << "Reading" << std::endl;
    
    std::vector<std::string> allLines;
    
    std::string word;
    while(infile >> word)
    {
        allLines.emplace_back(word);
    }
    
    
   {
       std::vector<std::string> oxygenLines = allLines; 
       int currentPos = 0;
       while (oxygenLines.size() > 1)
       {
           std::cout << currentPos << std::endl;
           std::cout << "   "  << oxygenLines.size() << " -- " << std::flush;
           Counter co = count(oxygenLines);
        
           char mostFound = co.map.at(currentPos).characterMostFound();
        
           if (mostFound == 'a')
           {
               mostFound = '1';
           }
        
           std::cout << mostFound << " -- " << std::flush;
        
           std::vector<std::string> newLines;
        
           for (std::string& line : oxygenLines)
           {
               if (line.at(currentPos) == mostFound)
               {
                  newLines.emplace_back(line);
               }
           }
           oxygenLines = std::move(newLines);
           std::cout << oxygenLines.size() << std::endl;
        
           ++currentPos;
        }
        std::string solution = oxygenLines.front();   
        std::cout << solution << std::endl;

        int sol = 0;
        for (char c : solution)
        {
            sol *= 2;
            if (c == '1')
            {
                sol+= 1;
            }
        }
        std::cout << sol << std::endl;
    }
    {
       std::vector<std::string> co2scrubberLines = allLines; 
       int currentPos = 0;
       while (co2scrubberLines.size() > 1)
       {
           std::cout << currentPos << std::endl;
           std::cout << "   "  << co2scrubberLines.size() << " -- " << std::flush;
           Counter co = count(co2scrubberLines);

           char leastFound = co.map.at(currentPos).characterLeastFound();

           if (leastFound == 'a')
           {
               leastFound = '0';
           }

           std::cout << leastFound << " -- " << std::flush;

           std::vector<std::string> newLines;

           for (std::string& line : co2scrubberLines)
           {
               if (line.at(currentPos) == leastFound)
               {
                  newLines.emplace_back(line);
               }
           }
           co2scrubberLines = std::move(newLines);
           std::cout << co2scrubberLines.size() << std::endl;

           ++currentPos;
        }
        std::string solution = co2scrubberLines.front();   
        std::cout << solution << std::endl;

        int sol = 0;
        for (char c : solution)
        {
            sol *= 2;
            if (c == '1')
            {
                sol+= 1;
            }
        }
        std::cout << sol << std::endl;
    }

}