#include <fstream>
#include <iostream>
#include "LinearQuadtree.h"

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

bool TryReadPositions(std::vector<Vector2>& positions)
{
    std::ifstream stream("benchmark/data/Positions.txt");
    if (!stream.is_open())
    {
        return false;
    }
    
    Vector2 position;
    while (stream >> position)
    {
        positions.push_back(position);
    }
    
    stream.close();
    return true;
}

bool InsertPositions(LinearQuadtree<size_t>& tree, const std::vector<Vector2>& positions)
{
    bool success = true;
    for (size_t i = 0; i < positions.size(); ++i)
    {
        success &= tree.Insert(i + 1, positions[i]);
    }
    return success;
}

void FindNearest(LinearQuadtree<size_t>& tree, const std::vector<Vector2>& positions)
{
//    for (const auto& position : positions)
//    {
//        tree.FindNearest(position);
//    }
}

bool RemovePositions(LinearQuadtree<size_t>& tree, const std::vector<Vector2>& positions)
{
    bool success = true;
    size_t data = positions.size();
    for (auto it = positions.rbegin(); it != positions.rend(); ++it)
    {
        success &= tree.Remove(data--, *it);
    }
    return success;
}

int main()
{
    LinearQuadtree<size_t> tree = { AABB({ -1000, -1000 }, { 1000, 1000 }), 4 };

    std::vector<Vector2> positions;
    if (!TryReadPositions(positions))
    {
        std::cout << "ERROR: Failed to read positions" << std::endl;
        return 1;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!InsertPositions(tree, positions))
    {
        std::cout << "ERROR: Failed to insert positions" << std::endl;
        return 1;
    }
    
    FindNearest(tree, positions);
    
    if (!RemovePositions(tree, positions))
    {
        std::cout << "ERROR: Failed to remove positions" << std::endl;
        return 1;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Benchmark Time: " << duration << " ms" << std::endl;
    
    return 0;
}
