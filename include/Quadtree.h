#pragma once
#include <optional>
#include <vector>
#include "AABB.h"
#include "Element.h"
#include "Vector2.h"

template<typename T, size_t Capacity>
class Quadtree
{
public:
    Quadtree(const AABB& bounds);
    Quadtree(const Quadtree& other) = delete;
    Quadtree(Quadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(T data, const Vector2& position);
    bool Remove(T data, const Vector2& position);
    
    std::optional<Element<T>> FindNearest(const Vector2& target) const;

    Quadtree& operator=(const Quadtree&) = delete;
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    void TryMerge();
    
    void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element<T>>& nearest) const;
    
    AABB mBounds;
    bool mIsLeaf = true;
    std::vector<Element<T>> mElements;
    std::array<std::unique_ptr<Quadtree>, 4> mChildren;
};

#include "Quadtree.inl"
