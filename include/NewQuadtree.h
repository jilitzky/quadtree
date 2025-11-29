#pragma once
#include <optional>
#include <vector>
#include "AABB.h"

class NewQuadtree
{
public:
    NewQuadtree(const AABB& bounds, int nodeCapacity);
    NewQuadtree(const NewQuadtree& other) = delete;
    NewQuadtree(NewQuadtree&& other) = default;
    
    const AABB& GetBounds() const;
    int GetSize() const;
    int GetDepth() const;
    
    bool Insert(const Vector2& position);

    NewQuadtree& operator=(const NewQuadtree&) = delete;
    NewQuadtree& operator=(NewQuadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    
    AABB m_bounds;
    int m_nodeCapacity;
    std::vector<Vector2> m_elements;
    std::array<std::unique_ptr<NewQuadtree>, 4> m_children;
    bool m_isLeaf = true;
};
