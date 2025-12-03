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
        return mRoot.bounds;
    }
    
    size_t GetSize() const
    {
        return GetSize(mRoot);
    }
    
    size_t GetHeight() const
    {
        return GetHeight(mRoot);
    }
    
    LinearQuadtree& operator=(const LinearQuadtree&) = delete;
    LinearQuadtree& operator=(LinearQuadtree&&) = default;
    
private:
    size_t GetSize(const Node& node) const
    {
        if (node.isLeaf)
        {
            return node.elements.size();
        }
        
        size_t size = 0;
        for (auto childIndex : node.children)
        {
            // TODO: <JI> Get child from nodes list
            //size += GetSize(child);
        }
        
        return size;
    }
    
    size_t GetHeight(const Node& node) const
    {
        if (node.isLeaf)
        {
            return 1;
        }
        
        size_t height = 0;
        for (auto childIndex : node.children)
        {
            // TODO: <JI> Get child from nodes list
            //height = std::max(GetHeight(child), height);
        }
        
        return height + 1;
    }
    
    Node mRoot;
};
