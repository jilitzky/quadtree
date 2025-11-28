#pragma once
#include <optional>
#include <vector>
#include "Node.h"

class Quadtree
{
public:
    Quadtree(const AABB& bounds, size_t nodeCapacity = 4, size_t maxDepth = 10);

    Quadtree(const Quadtree&) = delete;
    Quadtree& operator=(const Quadtree&) = delete;
    Quadtree(Quadtree&&) = default;
    Quadtree& operator=(Quadtree&&) = default;
    
    bool Insert(const Vector2& position);
    bool Remove(const Vector2& position);
    
    std::vector<Vector2> QueryRange(const AABB& range) const;
    std::optional<Vector2> FindNearest(const Vector2& position, float maxRadius = -1.0f) const;
    
    void Clear();
    
    size_t GetSize() const;
    size_t GetDepth() const;
    const AABB& GetBounds() const;

private:
    void Subdivide();

    AABB m_bounds;
    size_t m_nodeCapacity;
    size_t m_maxDepth;
    size_t m_currentDepth;

    std::vector<Vector2> m_elements;

    // NorthWest, NorthEast, SouthWest, SouthEast
    std::array<std::unique_ptr<Quadtree>, 4> m_children;
    bool m_isLeaf = true;
    
    // TODO: Review old fields
    Node m_root;
    size_t m_size = 0;
};
