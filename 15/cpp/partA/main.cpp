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
        if(p.first < 0 || p.first >= static_cast<int>(grid.size()))
        {
            return false;
        }
        std::vector<int> line = grid.at(p.first);

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
        int lastLine = static_cast<int>(grid.size()) - 1;

        return {lastLine, grid.at(lastLine).size() - 1};
    }

    std::vector<std::vector<int>> grid;
};

std::ostream& operator<<(std::ostream& o, const Grid& g)
{
    for(const std::vector<int>& line : g.grid)
    {
        for(const int i : line)
        {
            o << i;
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
            g.grid.emplace_back(std::move(line));
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

    for(const PathWithTotalRisk& pa : iter->second)
    {
        for(const Pos nextPos : g.nbs(pa.p.back()))
        {
            if(std::find(pa.p.begin(), pa.p.end(), nextPos) == pa.p.end())
            {
                PathWithTotalRisk newPa = pa;
                newPa.p.emplace_back(nextPos);
                newPa.totalRisk += g.grid.at(nextPos.first).at(nextPos.second);

                auto finder = lowestRiskMap.find(nextPos);
                if(finder == lowestRiskMap.end() || finder->second < newPa.totalRisk)
                {
                    lowestRiskMap[nextPos] = newPa.totalRisk;
                    paths[newPa.totalRisk].emplace(newPa);
                }
            }
        }
    }
    paths.erase(iter);
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

    std::cout << g;

    const Pos        end = g.lastPos();
    PathWithRisksSet paths{{0, {PathWithTotalRisk({0, 0})}}};
    Risk             lowestRisk{{Pos{0, 0}, 0}};

    bool shouldEnd = false;
    std::cout << "Extending to " << end << std::endl;

    while(shouldEnd == false)
    {
        extendshortedPaths(g, paths, lowestRisk);

        std::pair<bool, int> result = reached(end, paths);

        if(result.first)
        {
            if(paths.begin()->first == result.second)
            {
                shouldEnd = true;
                std::cout << result.second << std::endl;
            }

            removeLowerTotalRisk(paths, result.second, lowestRisk);
        }
    }

    return 0;
}
