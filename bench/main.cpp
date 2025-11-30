#include <fstream>
#include <iostream>
#include "NewQuadtree.h"

const int POINTS_CAPACITY = 10000;

std::vector<Vector2> ReadPoints(std::istream& stream);

int main()
{
    NewQuadtree tree = { AABB({ -1000, -1000 }, { 1000, 1000 }), 4 };

    std::ifstream stream("bench/data/Points.txt");
    if (!stream.is_open())
    {
        std::cout << "ERROR: Failed to open input stream" << std::endl;
        return 1;
    }
    
    std::vector<Vector2> points = ReadPoints(stream);
    stream.close();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& point : points)
    {
        tree.Insert(point);
    }
    
    for (const auto& point : points)
    {
        tree.FindNearest(point);
    }
    
    for (auto it = points.rbegin(); it != points.rend(); ++it)
    {
        tree.Remove(*it);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Benchmark Time: " << duration << " ms" << std::endl;
    return 0;
}

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
    points.reserve(POINTS_CAPACITY);
    
    Vector2 point;
    while (stream >> point)
    {
        points.push_back(point);
    }
    
    return points;
}
