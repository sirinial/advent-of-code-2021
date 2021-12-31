#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using int64 = std::int64_t;
struct Coordinate
{
    Coordinate() = default;
    Coordinate(int64 ax, int64 ay, int64 az)
        : x(ax)
        , y(ay)
        , z(az)
    {}

    int64 x{0};
    int64 y{0};
    int64 z{0};

    bool operator<(const Coordinate& other) const
    {
        return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
    }
    bool operator==(const Coordinate& other) const
    {
        return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
    }
    bool operator!=(const Coordinate& other) const
    {
        return std::tie(x, y, z) != std::tie(other.x, other.y, other.z);
    }

    int64 distanceSquared(const Coordinate& other) const
    {
        int64 dx = x - other.x;
        int64 dy = y - other.y;
        int64 dz = z - other.z;
        return (dx * dx) + (dy * dy) + (dz * dz);
    }

    int64 manhattanDistance(const Coordinate& other) const
    {
        int64 dx = x - other.x;
        int64 dy = y - other.y;
        int64 dz = z - other.z;
        return std::abs(dx) + std::abs(dy) + std::abs(dz);
    }
};

std::ostream& operator<<(std::ostream& o, const Coordinate& co)
{
    o << "(" << co.x << "," << co.y << "," << co.z << ")";
    return o;
}
Coordinate operator-(const Coordinate& lhs, const Coordinate& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Coordinate operator+(const Coordinate& lhs, const Coordinate& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

int64 innerProduct(const Coordinate& lhs, const Coordinate& rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

using CoordinateSet = std::set<Coordinate>;

std::ostream& operator<<(std::ostream& o, const std::set<Coordinate>& l)
{
    o << "{";
    for(const auto& a : l)
    {
        o << " " << a;
    }
    o << " }";
    return o;
}

struct CoordinateMap
{
    Coordinate ax;
    Coordinate ay;
    Coordinate az;
    Coordinate b;

    Coordinate map(const Coordinate& co) const
    {
        return Coordinate(innerProduct(ax, co), innerProduct(ay, co), innerProduct(az, co)) + b;
    }

    CoordinateSet map(const CoordinateSet& elements) const
    {
        CoordinateSet result;
        for(const Coordinate& co : elements)
        {
            result.emplace(map(co));
        }
        return result;
    }

    bool operator<(const CoordinateMap& other) const
    {
        return std::tie(ax, ay, az, b) < std::tie(other.ax, other.ay, other.az, other.b);
    }
};
std::ostream& operator<<(std::ostream& o, const CoordinateMap& co)
{
    o << "(" << co.ax << "," << co.ay << "," << co.az << "," << co.b << ")";
    return o;
}

std::set<CoordinateMap> createPossibleAs()
{
    std::set<CoordinateMap> result;

    for(int64 x = -1; x <= 1; x += 2)
    {
        for(int64 y = -1; y <= 1; y += 2)
        {
            for(int64 z = -1; z <= 1; z += 2)
            {
                if(x * y * z == 1)
                {
                    {
                        CoordinateMap map;
                        map.ax = {x, 0, 0};
                        map.ay = {0, y, 0};
                        map.az = {0, 0, z};

                        result.emplace(map);
                    }
                    {
                        CoordinateMap map;
                        map.ax = {0, x, 0};
                        map.ay = {0, 0, y};
                        map.az = {z, 0, 0};

                        result.emplace(map);
                    }
                    {
                        CoordinateMap map;
                        map.ax = {0, 0, z};
                        map.ay = {x, 0, 0};
                        map.az = {0, y, 0};

                        result.emplace(map);
                    }
                }
                else
                {
                    {
                        CoordinateMap map;
                        map.ax = {0, x, 0};
                        map.ay = {y, 0, 0};
                        map.az = {0, 0, z};

                        result.emplace(map);
                    }
                    {
                        CoordinateMap map;
                        map.ax = {x, 0, 0};
                        map.ay = {0, 0, y};
                        map.az = {0, z, 0};

                        result.emplace(map);
                    }
                    {
                        CoordinateMap map;
                        map.ax = {0, 0, z};
                        map.ay = {0, y, 0};
                        map.az = {x, 0, 0};

                        result.emplace(map);
                    }
                }
            }
        }
    }

    return result;
}

const std::set<CoordinateMap> gPossibleAs = createPossibleAs();

CoordinateMap createMap(const CoordinateSet& origin, const CoordinateSet& target)
{
    CoordinateSet possibleBs;

    for(const Coordinate& ori : origin)
    {
        for(const Coordinate& tar : target)
        {

            for(const CoordinateMap& A : gPossibleAs)
            {
                possibleBs.emplace(tar - A.map(ori));
            }
        }
    }

    for(const CoordinateMap& A : gPossibleAs)
    {
        for(const Coordinate& b : possibleBs)
        {
            CoordinateMap m = A;
            m.b             = b;

            // std::cout << " * Considering: " << m << std::endl;

            const auto mappedOrigin = m.map(origin);
            // std::cout << "             -> " << mappedOrigin << std::endl;

            if(mappedOrigin == target)
            {
                return m;
            }
        }
    }
    std::cerr << "Was not able to calculate" << std::endl;
    return {};
}

struct Scanner
{
    int id;

    Coordinate base{0, 0, 0};

    CoordinateSet beacons;

    std::multiset<int64> distanceSquared;

    void normalize(const Scanner& scan);
};

std::multiset<int64> intersection(const std::multiset<int64>& lhs, const std::multiset<int64>& rhs)
{
    std::multiset<int64> result;

    auto lIter = lhs.begin();
    auto rIter = rhs.begin();

    while(lIter != lhs.end() && rIter != rhs.end())
    {
        if(*lIter == *rIter)
        {
            result.emplace(*lIter);
            ++lIter;
            ++rIter;
        }
        else if(*lIter < *rIter)
        {
            ++lIter;
        }
        else if(*lIter > *rIter)
        {
            ++rIter;
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream& o, const std::multiset<int64>& l)
{
    o << "{";
    for(const int64 a : l)
    {
        o << " " << a;
    }
    o << " }";
    return o;
}
std::ostream& operator<<(std::ostream& o, const std::multiset<int>& l)
{
    o << "{";
    for(const int a : l)
    {
        o << " " << a;
    }
    o << " }";
    return o;
}
std::multiset<int64> distanceSquared(const std::set<Coordinate>& beacons)
{
    std::multiset<int64> result;
    for(const auto& lhs : beacons)
    {
        for(const auto& rhs : beacons)
        {
            if(lhs < rhs)
            {
                result.emplace(lhs.distanceSquared(rhs));
            }
        }
    }
    return result;
}

std::set<std::set<Coordinate>> subsetsOfSize(const std::set<Coordinate>& beacons, const std::size_t& s)
{
    if(beacons.size() < s)
    {
        return {};
    }
    if(beacons.size() == s)
    {
        return {beacons};
    }

    std::set<std::set<Coordinate>> result;
    for(const Coordinate& co : beacons)
    {
        std::set<Coordinate> smallerBeacons = beacons;
        smallerBeacons.erase(smallerBeacons.find(co));

        for(const auto& el : subsetsOfSize(smallerBeacons, s))
        {
            result.emplace(el);
        }
    }
    return result;
}

std::set<Coordinate> makeSmaller(const std::set<Coordinate>& beaconsWithRightDistance, const std::multiset<int64>& distances)
{
    if(beaconsWithRightDistance.size() == 12)
    {
        return beaconsWithRightDistance;
    }
    {
        std::set<Coordinate> newBeaconsWithRightDistance;
        for(const auto& beacon : beaconsWithRightDistance)
        {
            bool takeBeacon = false;
            for(const auto& otherBeacon : beaconsWithRightDistance)
            {

                if(distances.find(beacon.distanceSquared(otherBeacon)) != distances.end())
                {
                    takeBeacon = true;
                }
            }
            if(takeBeacon)
            {
                newBeaconsWithRightDistance.emplace(beacon);
            }
        }
        if(newBeaconsWithRightDistance.size() < beaconsWithRightDistance.size())
        {
            return newBeaconsWithRightDistance;
        }
    }

    auto subsets = subsetsOfSize(beaconsWithRightDistance, 12);

    for(const std::set<Coordinate> newBeaconsWithRightDistance : subsets)
    {
        std::multiset<int64> dista = distanceSquared(newBeaconsWithRightDistance);
        if(dista == distances)
        {
            return newBeaconsWithRightDistance;
        }
    }

    std::set<Coordinate> newBeaconsWithRightDistance = beaconsWithRightDistance;

    return newBeaconsWithRightDistance;
}

CoordinateSet getCoordinatesOf(const CoordinateSet& coordinates, const std::multiset<int64>& distances)
{
    std::set<Coordinate> beaconsWithRightDistance = coordinates;

    bool tryAgain = true;

    while(tryAgain)
    {
        std::set<Coordinate> newBeaconsWithRightDistance = makeSmaller(beaconsWithRightDistance, distances);
        tryAgain                                         = newBeaconsWithRightDistance.size() < beaconsWithRightDistance.size();

        beaconsWithRightDistance = std::move(newBeaconsWithRightDistance);
    }

    return beaconsWithRightDistance;
}

void Scanner::normalize(const Scanner& origin)
{
    std::multiset<int64> distances = intersection(distanceSquared, origin.distanceSquared);

    CoordinateSet originCoordinates = getCoordinatesOf(origin.beacons, distances);
    CoordinateSet targetCoordinates = getCoordinatesOf(beacons, distances);

    if(originCoordinates.size() != targetCoordinates.size())
    {
        std::cerr << "Problem at normalizing" << std::endl;
    }

    CoordinateMap map = createMap(targetCoordinates, originCoordinates);

    base    = map.map(base);
    beacons = map.map(beacons);

    std::cout << "    - Found " << map << std::endl;
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

    int id = 0;

    std::map<int, Scanner> scanners;

    while(infile.peek() != EOF)
    {
        std::string line;

        Scanner scanner;
        scanner.id = id;

        getline(infile, line);
        getline(infile, line);

        bool succesAtReading = true;

        while(line.empty() == false && succesAtReading)
        {
            std::stringstream ss(line);
            Coordinate        co;
            char              c;

            ss >> co.x >> c >> co.y >> c >> co.z;

            scanner.beacons.emplace(co);

            if(infile.peek() == EOF)
            {
                succesAtReading = false;
            }
            else
            {
                getline(infile, line);
            }
        }

        while(infile.peek() == '\n')
        {
            infile.get();
        }
        std::cout << "Read Scanner " << scanner.id << " (" << scanner.beacons.size() << " elements)" << std::endl;

        scanners.emplace(scanner.id, scanner);
        ++id;
    }

    for(auto& entry : scanners)
    {
        Scanner& scan = entry.second;
        for(const auto& lhs : scan.beacons)
        {
            for(const auto& rhs : scan.beacons)
            {
                if(lhs < rhs)
                {
                    scan.distanceSquared.emplace(lhs.distanceSquared(rhs));
                }
            }
        }
        std::cout << "Scanner " << scan.id << " has " << scan.distanceSquared.size() << std::endl;
    }

    std::multimap<int, int> connections;

    for(auto& lEntry : scanners)
    {
        Scanner& lhs = lEntry.second;
        for(auto& rEntry : scanners)
        {
            Scanner&             rhs    = rEntry.second;
            std::multiset<int64> result = intersection(lhs.distanceSquared, rhs.distanceSquared);

            if(result.size() >= 66 && lhs.id < rhs.id)
            {
                std::cout << "Connecting " << lhs.id << " <-> " << rhs.id << std::endl;
                connections.emplace(lhs.id, rhs.id);
                connections.emplace(rhs.id, lhs.id);
            }
        }
    }

    std::set<int> toDo{0};
    std::set<int> done;

    while(toDo.empty() == false)
    {
        const int id = *toDo.begin();
        toDo.erase(toDo.begin());
        done.emplace(id);

        std::cout << "Doing ID " << id << std::endl;

        auto r = connections.equal_range(id);
        for(auto iter = r.first; iter != r.second; ++iter)
        {
            if(done.find(iter->second) == done.end())
            {
                std::cout << "       Calculating " << iter->first << " ---> " << iter->second << std::endl;

                scanners.at(iter->second).normalize(scanners.at(iter->first));

                toDo.emplace(iter->second);
            }
        }
    }

    int max = 0;

    for(const auto& lEntry : scanners)
    {
        const Scanner& lhs = lEntry.second;
        for(const auto& rEntry : scanners)
        {
            const Scanner& rhs = rEntry.second;

            const int dist = rhs.base.manhattanDistance(lhs.base);
            if(dist > max)
            {
                max = dist;
            }
        }
    }
    std::cout << max << std::endl;

    return 0;
}
