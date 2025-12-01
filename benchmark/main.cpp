#include <fstream>
#include <iostream>
#include "Quadtree.h"

std::vector<Vector2> ReadPositions(std::istream& stream);

int main()
{
    Quadtree<size_t> tree = { AABB({ -1000, -1000 }, { 1000, 1000 }), 4 };

    std::ifstream stream("benchmark/data/Positions.txt");
    if (!stream.is_open())
    {
        std::cout << "ERROR: Failed to open input stream" << std::endl;
        return 1;
    }
    
    std::vector<Vector2> positions = ReadPositions(stream);
    stream.close();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < positions.size(); ++i)
    {
        tree.Insert(positions[i], i + 1);
    }
    
    for (const auto& position : positions)
    {
        tree.FindNearest(position);
    }
    
    size_t data = positions.size();
    for (auto it = positions.rbegin(); it != positions.rend(); ++it)
    {
        tree.Remove(*it, data--);
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

std::vector<Vector2> ReadPositions(std::istream& stream)
{
    std::vector<Vector2> positions;
    positions.reserve(10000);
    
    Vector2 position;
    while (stream >> position)
    {
        positions.push_back(position);
    }
    
    return positions;
}
