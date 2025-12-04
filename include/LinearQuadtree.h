#pragma once
#include "AABB.h"
#include "Node.h"

template<typename T, size_t Capacity>
class LinearQuadtree
{
public:
    LinearQuadtree(const AABB& bounds);
    LinearQuadtree(const LinearQuadtree& other) = delete;
    LinearQuadtree(LinearQuadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(int data, const Vector2& position);
    
    LinearQuadtree& operator=(const LinearQuadtree&) = delete;
    LinearQuadtree& operator=(LinearQuadtree&&) = default;
    
private:
    size_t GetSize(int nodeIndex) const;
    size_t GetHeight(int nodeIndex) const;
    int GetChildIndex(int nodeIndex, const Vector2& position) const;
    
    bool Insert(int nodeIndex, int data, const Vector2& position);
    void Subdivide(int nodeIndex);
    
    int AllocateNode(const AABB& bounds);
    
    std::vector<Node<T>> mNodes;
};

#include "LinearQuadtree.inl"
