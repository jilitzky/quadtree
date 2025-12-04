#pragma once
#include "AABB.h"
#include "Node.h"

class LinearQuadtree
{
public:
    LinearQuadtree(const AABB& bounds);
    LinearQuadtree(const LinearQuadtree& other) = delete;
    LinearQuadtree(LinearQuadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    LinearQuadtree& operator=(const LinearQuadtree&) = delete;
    LinearQuadtree& operator=(LinearQuadtree&&) = default;
    
private:
    const Node& GetRoot() const;
    size_t GetNodeSize(const Node& node) const;
    size_t GetNodeHeight(const Node& node) const;
    
    std::vector<Node> mNodes;
};

#include "LinearQuadtree.inl"
