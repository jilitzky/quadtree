#include <fstream>
#include <iostream>
#include "Quadtree.h"

using Tree = Quadtree<size_t, 10>;

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

std::chrono::milliseconds Insert(Tree& tree, const std::vector<Vector2>& positions)
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
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::chrono::milliseconds FindNearest(Tree& tree, const std::vector<Vector2>& positions)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    for (const auto& position : positions)
    {
        tree.FindNearest(position);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::chrono::milliseconds Query(Tree& tree, const std::vector<Vector2>& positions)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < positions.size(); i = i + 10)
    {
        Vector2 position = positions[i];
        Vector2 min = { std::min(-position.x, position.x), std::min(-position.y, position.y) };
        Vector2 max = { std::max(-position.x, position.x), std::max(-position.y, position.y) };
        tree.Query(AABB(min, max));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

std::chrono::milliseconds Remove(Tree& tree, const std::vector<Vector2>& positions)
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
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
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
    
    auto insert = Insert(tree, positions);
    auto findNearest = FindNearest(tree, positions);
    auto query = Query(tree, positions);
    auto remove = Remove(tree, positions);
    
    std::cout << "Insert: " << insert.count() << " ms" << std::endl;
    std::cout << "FindNearest: " << findNearest.count() << " ms" << std::endl;
    std::cout << "Query: " << query.count() << " ms" << std::endl;
    std::cout << "Remove: " << remove.count() << " ms" << std::endl;
    
    return 0;
}
