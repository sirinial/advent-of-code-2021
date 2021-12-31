#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using Pos = std::pair<int, int>;

std::ostream& operator<<(std::ostream& o, const Pos& p)
{
    o << "(" << p.first << "," << p.second << ")";
    return o;
}
Pos operator+(const Pos& f, const Pos& o)
{
    Pos result = f;
    result.first += o.first;
    result.second += o.second;
    return result;
}

using Risk = std::map<Pos, int>;

const std::vector<Pos> directions{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct Grid
{
    bool hasPos(const Pos p) const
    {
        if(p.first < 0 || p.first >= static_cast<int>(_grid.size()))
        {
            return false;
        }
        std::vector<int> line = _grid.at(p.first);

        return p.second >= 0 && p.second < static_cast<int>(line.size());
    }

    std::vector<Pos> nbs(const Pos& p) const
    {
        std::vector<Pos> result;
        for(const Pos dir : directions)
        {
            Pos posEl = p + dir;
            if(hasPos(posEl))
            {
                result.emplace_back(posEl);
            }
        }
        return result;
    }

    Pos lastPos() const
    {
        int lastLine = static_cast<int>(_grid.size()) - 1;

        return {lastLine, _grid.at(lastLine).size() - 1};
    }

    Grid extend(const int dx, const int dy) const
    {
        std::vector<std::vector<int>> newBaseLines;

        for(const std::vector<int>& oldLine : _grid)
        {
            std::vector<int> line;
            for(int x = 0; x < dx; ++x)
            {
                for(const int e : oldLine)
                {
                    line.emplace_back(e + x);
                }
            }
            newBaseLines.emplace_back(std::move(line));
        }
        Grid result;

        for(int y = 0; y < dy; ++y)
        {
            for(const std::vector<int>& l : newBaseLines)
            {
                std::vector<int> line;

                for(const int e : l)
                {
                    line.emplace_back(e + y);
                }
                result._grid.emplace_back(std::move(line));
            }
        }
        for(auto& l : result._grid)
        {
            for(int& i : l)
            {
                while(i > 9)
                {
                    i -= 9;
                }
            }
        }
        return result;
    }

    Grid firstGuess() const
    {
        std::vector<int> lastLine;

        int total = -_grid.front().front();

        for(const int c : _grid.front())
        {
            total += c;

            lastLine.emplace_back(total);
        }

        auto iter = _grid.begin();
        ++iter;

        Grid result;
        result._grid.emplace_back(lastLine);

        while(iter != _grid.end())
        {
            auto lastLineIter = lastLine.begin();

            std::vector<int> line;
            for(const int c : *iter)
            {
                *lastLineIter += c;
                line.emplace_back(*lastLineIter);

                ++lastLineIter;
            }

            result._grid.emplace_back(std::move(line));

            ++iter;
        }
        return result;
    }

    int value(const Pos p) const
    {
        return _grid.at(p.first).at(p.second);
    }

    int reduceRisk(const Grid& g)
    {
        int result = 0;
        int i      = 0;
        for(std::vector<int>& line : _grid)
        {
            int j = 0;
            for(int& r : line)
            {
                const int c = g.value({i, j});
                for(const Pos p : g.nbs({i, j}))
                {
                    int posValue = value(p) + c;

                    if(posValue < r)
                    {
                        r = posValue;
                        ++result;
                    }
                }

                ++j;
            }

            ++i;
        }
        return result;
    }

    std::vector<std::vector<int>> _grid;
};

std::ostream& operator<<(std::ostream& o, const Grid& g)
{
    for(const std::vector<int>& line : g._grid)
    {
        for(const int i : line)
        {
            if(i < 100)
            {
                o << " ";
                if(i < 10)
                {
                    o << " ";
                }
            }
            o << i << " ";
        }
        o << std::endl;
    }
    return o;
}

std::istream& operator>>(std::istream& i, Grid& g)
{
    char             c;
    std::vector<int> line;
    while(i >> c)
    {
        line.emplace_back(c - '0');

        if(i.peek() == '\n')
        {
            g._grid.emplace_back(std::move(line));
            line = {};
        }
    }
    return i;
}

using Path = std::vector<Pos>;

struct PathWithTotalRisk
{
    PathWithTotalRisk() = default;
    PathWithTotalRisk(const Pos& start)
        : p({start})
    {}
    Path p;
    int  totalRisk{0};

    bool operator<(const PathWithTotalRisk& other) const
    {
        return std::tie(totalRisk, p) < std::tie(other.totalRisk, other.p);
    }
};

std::ostream& operator<<(std::ostream& o, const PathWithTotalRisk& g)
{
    for(const Pos& p : g.p)
    {
        o << "(" << p.first << "," << p.second << ") ";
    }
    o << g.totalRisk;
    return o;
}

using PathWithRisksSet = std::map<int, std::set<PathWithTotalRisk>>;

std::ostream& operator<<(std::ostream& o, const PathWithRisksSet& s)
{
    o << "{" << std::endl;
    for(const auto& p : s)
    {
        for(const auto& q : p.second)
        {
            o << "  " << q << std::endl;
        }
    }
    o << "}";
    return o;
}

std::pair<bool, int> reached(const Pos end, const PathWithRisksSet& elements)
{
    if(elements.empty())
    {
        return {false, 0};
    }

    for(const auto& p : elements)
    {
        for(const PathWithTotalRisk& pWithRisk : p.second)
        {
            if(pWithRisk.p.empty() == false && pWithRisk.p.back() == end)
            {
                return {true, pWithRisk.totalRisk};
            }
        }
    }
    return {false, 0};
}

void removeLowerTotalRisk(PathWithRisksSet& paths, int maxTotalRisk, const Risk& lowestRisk)
{
    PathWithRisksSet result;

    for(auto& entry : paths)
    {
        for(const auto& p : entry.second)
        {
            if(lowestRisk.at(p.p.back()) == p.totalRisk)
            {
                result[entry.first].emplace(p);
            }
        }
    }
    paths = std::move(result);
}

void extendshortedPaths(const Grid g, PathWithRisksSet& paths, Risk& lowestRiskMap)
{

    if(paths.empty())
    {
        return;
    }

    auto iter = paths.begin();

    const int lowestRisk = iter->first;

    std::cout << lowestRisk << ": " << iter->second.size() << std::endl;

    std::set<PathWithTotalRisk> toDo = std::move(iter->second);

    paths.erase(iter);

    for(const PathWithTotalRisk& pa : toDo)
    {
        for(const Pos nextPos : g.nbs(pa.p.back()))
        {
            PathWithTotalRisk newPa = pa;
            newPa.p.clear();
            newPa.p.emplace_back(nextPos);
            newPa.totalRisk += g._grid.at(nextPos.first).at(nextPos.second);

            auto finder = lowestRiskMap.find(nextPos);
            if(finder == lowestRiskMap.end() || finder->second < newPa.totalRisk)
            {
                lowestRiskMap[nextPos] = newPa.totalRisk;
                paths[newPa.totalRisk].emplace(newPa);
            }
        }
    }
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

    Grid g;

    infile >> g;

    int i = 5;

    g = g.extend(i, i);

    Grid lowestRsik = g.firstGuess();

    int count = 0;
    while((count = lowestRsik.reduceRisk(g)) != 0)
    {
        std::cout << count << std::endl;
    }
    std::cout << lowestRsik << std::endl;

    return 0;
}
