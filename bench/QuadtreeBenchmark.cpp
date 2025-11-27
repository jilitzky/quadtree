#include <fstream>
#include <gtest/gtest.h>
#include "Quadtree.h"

namespace
{
    std::istream& operator>>(std::istream& is, Vector2& v);
}

class QuadtreeBenchmark : public ::testing::Test
{
protected:
    Quadtree _tree = { { 0, 0 }, { 1000, 1000 } };
};

TEST_F(QuadtreeBenchmark, Benchmark)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream stream("bench/data/Points.txt");
    ASSERT_TRUE(stream.is_open());
    
    std::vector<Vector2> points;
    Vector2 point;
    while (stream >> point)
    {
        points.push_back(point);
    }

    stream.close();

    // Verification: Print the data back out
    std::cout << "Successfully loaded " << points.size() << " points:\n";
    for (const auto& p : points) {
        std::cout << "X: " << p.x << " | Y: " << p.y << "\n";
    }
    
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Benchmark time: " << duration << " ms" << std::endl;

    SUCCEED();
}

namespace
{
    std::istream& operator>>(std::istream& is, Vector2& v)
    {
        char separator;
        if (is >> v.x >> separator >> v.y)
        {
            if (separator != ',')
            {
                is.setstate(std::ios::failbit);
            }
        }
        return is;
    }
}
