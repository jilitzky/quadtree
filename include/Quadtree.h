#pragma once
#include <optional>
#include <vector>
#include "AABB.h"

template<typename T>
class Quadtree
{
public:
    struct Element
    {
        Vector2 position;
        T data;
    };
    
    Quadtree(const AABB& bounds, int nodeCapacity);
    Quadtree(const Quadtree& other) = delete;
    Quadtree(Quadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(const Vector2& position, T data);
    bool Remove(const Vector2& position, T data);
    
    std::optional<Element> FindNearest(const Vector2& target) const;

    Quadtree& operator=(const Quadtree&) = delete;
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    void TryMerge();
    
    void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element>& nearest) const;
    
    AABB mBounds;
    int mNodeCapacity;
    bool mIsLeaf = true;
    std::vector<Element> mElements;
    std::array<std::unique_ptr<Quadtree>, 4> mChildren;
};

#include "Quadtree.inl"
