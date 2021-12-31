#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class Point;
using PointVector = std::vector<Point>;
using PointSet    = std::set<Point>;
using int64       = std::int64_t;

class Point
{
public:
    Point(const int64 x, const int64 y, const int64 z)
        : _x(x)
        , _y(y)
        , _z(z)
    {}

    Point(const Point&) = default;
    Point(Point&&)      = default;
    Point& operator=(const Point&) = default;
    Point& operator=(Point&&) = default;

    bool operator<(const Point& other) const
    {
        return std::tie(_x, _y, _z) < std::tie(other._x, other._y, other._z);
    }

    bool operator==(const Point& other) const
    {
        return std::tie(_x, _y, _z) == std::tie(other._x, other._y, other._z);
    }

    bool operator!=(const Point& other) const
    {
        return std::tie(_x, _y, _z) != std::tie(other._x, other._y, other._z);
    }

    int64 x() const
    {
        return _x;
    }

    int64 y() const
    {
        return _y;
    }
    int64 z() const
    {
        return _z;
    }

private:
    int64 _x{0};
    int64 _y{0};
    int64 _z{0};
};
inline std::ostream& operator<<(std::ostream& os, const Point& t)
{
    os << "(" << t.x() << "," << t.y() << "," << t.z() << ")";
    return os;
}

using BoxVector = std::vector<class Box>;

class Box
{
public:
    Box(const Point& smallest, const Point& biggest)
        : mSmallest(smallest)
        , mBiggest(biggest)
    {
        if(smallest.x() >= biggest.x())
        {
            std::cerr << "Problem at constructing box of " << smallest << "<<" << biggest << std::endl;
        }
        if(smallest.y() >= biggest.y())
        {
            std::cerr << "Problem at constructing box of " << smallest << "<<" << biggest << std::endl;
        }
        if(smallest.z() >= biggest.z())
        {
            std::cerr << "Problem at constructing box of " << smallest << "<<" << biggest << std::endl;
        }
    }

    Box()           = delete;
    Box(const Box&) = default;
    Box(Box&&)      = default;
    Box& operator=(const Box&) = default;
    Box& operator=(Box&&) = default;
    ~Box()                = default;

    const Point& smallest() const
    {
        return mSmallest;
    }

    const Point& biggest() const
    {
        return mBiggest;
    }

    int64 size() const
    {
        return (mBiggest.x() - mSmallest.x()) * (mBiggest.y() - mSmallest.y()) * (mBiggest.z() - mSmallest.z());
    }

    bool contains(const Point& t) const
    {
        return mSmallest.x() <= t.x() && t.x() < mBiggest.x() && mSmallest.y() <= t.y() && t.y() < mBiggest.y() && mSmallest.z() <= t.z() && t.z() < mBiggest.z();
    }

    std::unique_ptr<Box> intersection(const Box& other) const
    {
        std::unique_ptr<Box> result;

        Point small(std::max(smallest().x(), other.smallest().x()), std::max(smallest().y(), other.smallest().y()), std::max(smallest().z(), other.smallest().z()));
        Point bigg(std::min(biggest().x(), other.biggest().x()), std::min(biggest().y(), other.biggest().y()), std::min(biggest().z(), other.biggest().z()));
        if(small.x() < bigg.x() && small.y() < bigg.y())
        {
            result = std::make_unique<Box>(small, bigg);
        }
        return result;
    }

    bool operator==(const Box& other) const
    {
        return std::tie(mSmallest, mBiggest) == std::tie(other.mSmallest, other.mBiggest);
    }

    bool operator!=(const Box& other) const
    {
        return std::tie(mSmallest, mBiggest) != std::tie(other.mSmallest, other.mBiggest);
    }

private:
    Point mSmallest;
    Point mBiggest;
};

inline std::ostream& operator<<(std::ostream& os, const Box& t)
{
    os << "[" << t.smallest() << "<<" << t.biggest() << "]";
    return os;
}

void normalize(int64& el)
{
    if(el < -50)
    {
        el = -50;
    }
    else if(el > 51)
    {
        el = 51;
    }
}

struct Values
{
    std::set<int64> x, y, z;
};

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

    std::vector<std::pair<Box, bool>> boxes;

    Values values;

    while(infile.peek() != EOF)
    {
        std::string command;
        infile >> command;

        int64 xMin, xMax, yMin, yMax, zMin, zMax;

        infile.get();
        infile.get();
        infile.get();
        infile >> xMin;
        infile.get();
        infile.get();

        infile >> xMax;

        infile.get();
        infile.get();
        infile.get();

        infile >> yMin;

        infile.get();
        infile.get();

        infile >> yMax;

        infile.get();

        infile.get();
        infile.get();

        infile >> zMin;

        infile.get();
        infile.get();

        infile >> zMax;

        infile.get();

        xMax += 1;
        yMax += 1;
        zMax += 1;

        if(xMin < xMax && yMin < yMax && zMin < zMax)
        {
            Point smallest(xMin, yMin, zMin);
            Point biggest(xMax, yMax, zMax);

            values.x.emplace(xMin);
            values.x.emplace(xMax);

            values.y.emplace(yMin);
            values.y.emplace(yMax);

            values.z.emplace(zMin);
            values.z.emplace(zMax);

            Box b = Box(smallest, biggest);

            boxes.emplace_back(b, command == "on");
        }
    }

    std::set<Point> points;

    std::cout << values.x.size() << std::flush << " ";
    std::cout << values.y.size() << std::flush << " ";
    std::cout << values.z.size() << std::endl;

    int counter = 0;

    for(const auto& entry : boxes)
    {
        const Box& b = entry.first;
        std::cout << (++counter) << "  ";
        if(entry.second)
        {
            std::cout << "-> Adding   " << b << std::endl;
        }
        else
        {
            std::cout << "-> Removing " << b << std::endl;
        }

        const auto xBegin = values.x.find(b.smallest().x());
        const auto yBegin = values.y.find(b.smallest().y());
        const auto zBegin = values.z.find(b.smallest().z());

        const auto xEnd = values.x.find(b.biggest().x());
        const auto yEnd = values.y.find(b.biggest().y());
        const auto zEnd = values.z.find(b.biggest().z());

        for(auto xIter = xBegin; xIter != xEnd; ++xIter)
        {
            for(auto yIter = yBegin; yIter != yEnd; ++yIter)
            {
                for(auto zIter = zBegin; zIter != zEnd; ++zIter)
                {
                    if(entry.second)
                    {
                        points.emplace(Point{*xIter, *yIter, *zIter});
                    }
                    else
                    {
                        auto finder = points.find(Point{*xIter, *yIter, *zIter});
                        if(finder != points.end())
                        {
                            points.erase(finder);
                        }
                    }
                }
            }
        }
    }

    int64 result = 0;

    std::cout << points.size() << std::endl;
    for(const Point p : points)
    {
        auto xIter     = values.x.find(p.x());
        auto nextXIter = std::next(xIter);

        auto yIter     = values.y.find(p.y());
        auto nextYIter = std::next(yIter);

        auto zIter     = values.z.find(p.z());
        auto nextZIter = std::next(zIter);

        result += (*nextXIter - *xIter) * (*nextYIter - *yIter) * (*nextZIter - *zIter);
    }

    std::cout << result << std::endl;

    return 0;
}
