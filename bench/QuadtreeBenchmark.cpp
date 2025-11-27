#include <fstream>
#include <gtest/gtest.h>
#include "Quadtree.h"

namespace
{
std::vector<Vector2> ReadPoints(std::istream& stream);
}

class QuadtreeBenchmark : public ::testing::Test
{
protected:
    Quadtree _tree = { { -1000, -1000 }, { 1000, 1000 } };
};

TEST_F(QuadtreeBenchmark, Benchmark)
{
    std::ifstream stream("bench/data/Points.txt");
    ASSERT_TRUE(stream.is_open());
    std::vector<Vector2> points = ReadPoints(stream);
    stream.close();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& point : points)
    {
        _tree.Add(point);
    }
    
    for (const auto& point : points)
    {
        _tree.FindNearest(point);
    }
    
    for (auto it = points.rbegin(); it != points.rend(); ++it)
    {
        _tree.Remove(*it);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Benchmark took " << duration << " ms for " << points.size() << " points" << std::endl;
    SUCCEED();
}

namespace
{
    std::istream& operator>>(std::istream& stream, Vector2& vector)
    {
        char separator;
        if (stream >> vector.x >> separator >> vector.y)
        {
            if (separator != ',')
            {
                stream.setstate(std::ios::failbit);
            }
        }
        return stream;
    }

    std::vector<Vector2> ReadPoints(std::istream& stream)
    {
        std::vector<Vector2> points;
        points.reserve(10000);
        
        Vector2 point;
        while (stream >> point)
        {
            points.push_back(point);
        }
        
        return points;
    }
}
