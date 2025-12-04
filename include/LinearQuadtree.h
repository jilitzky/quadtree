#pragma once
#include "AABB.h"
#include "Node.h"

template<typename T>
class LinearQuadtree
{
public:
    LinearQuadtree(const AABB& bounds, size_t nodeCapacity);
    LinearQuadtree(const LinearQuadtree& other) = delete;
    LinearQuadtree(LinearQuadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(T data, const Vector2& position);
    bool Remove(T data, const Vector2& position);
    
    LinearQuadtree& operator=(const LinearQuadtree&) = delete;
    LinearQuadtree& operator=(LinearQuadtree&&) = default;
    
private:
    size_t GetSize(int nodeIndex) const;
    size_t GetHeight(int nodeIndex) const;
    int GetChildIndex(int nodeIndex, const Vector2& position) const;
    
    bool Insert(int nodeIndex, T data, const Vector2& position);
    bool Remove(int nodeIndex, T data, const Vector2& position);
    
    void Subdivide(int nodeIndex);
    void TryMerge(int nodeIndex);
    
    int AllocateNode(const AABB& bounds);
    void FreeNode(int nodeIndex);
    
    std::vector<Node<T>> mNodes;
    size_t mNodeCapacity;
    int mFreeHead = -1;
};

#include "LinearQuadtree.inl"
