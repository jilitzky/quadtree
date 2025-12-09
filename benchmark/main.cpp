#include <fstream>
#include <iostream>
#include "Quadtree.h"

using Tree = Quadtree<size_t, 16>;

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

std::chrono::nanoseconds Insertion(Tree& tree, const std::vector<Vector2>& positions)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    bool success = true;
    for (size_t i = 0; i < positions.size(); ++i)
    {
        success &= tree.Insert(i + 1, positions[i]);
    }
    
    if (!success)
    {
        std::cout << "ERROR: Failed to insert positions" << std::endl;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds FindNearest(Tree& tree, const std::vector<Vector2>& positions)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& position : positions)
    {
        tree.FindNearest(position);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds SpatialQuery(Tree& tree, const std::vector<Vector2>& positions)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < positions.size(); i = ++i)
    {
        Vector2 position = positions[i];
        Vector2 min = { std::min(-position.x, position.x), std::min(-position.y, position.y) };
        Vector2 max = { std::max(-position.x, position.x), std::max(-position.y, position.y) };
        tree.SpatialQuery(AABB(min, max));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

std::chrono::nanoseconds Removal(Tree& tree, const std::vector<Vector2>& positions)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    bool success = true;
    size_t data = positions.size();
    for (auto it = positions.rbegin(); it != positions.rend(); ++it)
    {
        success &= tree.Remove(data--, *it);
    }
    
    if (!success)
    {
        std::cout << "ERROR: Failed to remove positions" << std::endl;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

int main()
{
    Tree tree = { AABB({ -1000, -1000 }, { 1000, 1000 }) };

    std::vector<Vector2> positions;
    if (!TryReadPositions(positions))
    {
        std::cout << "ERROR: Failed to read positions" << std::endl;
        return 1;
    }
    
    auto insertion = Insertion(tree, positions);
    auto findNearest = FindNearest(tree, positions);
    auto spatialQuery = SpatialQuery(tree, positions);
    auto removal = Removal(tree, positions);
    
    size_t numPositions = positions.size();
    std::cout << "Insertion: " << insertion.count() / numPositions << " ns" << std::endl;
    std::cout << "Removal: " << removal.count() / numPositions << " ns" << std::endl;
    std::cout << "Find Nearest: " << findNearest.count() / numPositions << " ns" << std::endl;
    std::cout << "Spatial Query: " << spatialQuery.count() / numPositions << " ns" << std::endl;
    
    return 0;
}
