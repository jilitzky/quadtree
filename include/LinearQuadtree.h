#pragma once
#include "AABB.h"
#include "Node.h"

class LinearQuadtree
{
public:
    LinearQuadtree(const AABB& bounds);
    LinearQuadtree(const LinearQuadtree& other) = delete;
    LinearQuadtree(LinearQuadtree&& other) = default;
    
    const AABB& GetBounds() const
    {
        return mNodes[0].bounds;
    }
    
    size_t GetSize() const
    {
        return GetNodeSize(mNodes[0]);
    }
    
    size_t GetHeight() const
    {
        return GetNodeHeight(mNodes[0]);
    }
    
    LinearQuadtree& operator=(const LinearQuadtree&) = delete;
    LinearQuadtree& operator=(LinearQuadtree&&) = default;
    
private:
    size_t GetNodeSize(const Node& node) const
    {
        if (node.isLeaf)
        {
            return node.elements.size();
        }
        
        size_t size = 0;
        for (auto childIndex : node.children)
        {
            const Node& child = mNodes[childIndex];
            size += GetNodeSize(child);
        }
        
        return size;
    }
    
    size_t GetNodeHeight(const Node& node) const
    {
        if (node.isLeaf)
        {
            return 1;
        }
        
        size_t height = 0;
        for (auto childIndex : node.children)
        {
            const Node& child = mNodes[childIndex];
            height = std::max(GetNodeHeight(child), height);
        }
        
        return height + 1;
    }
    
    std::vector<Node> mNodes;
};
