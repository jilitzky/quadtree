#pragma once
#include <optional>
#include <vector>
#include "AABB.h"
#include "Vector2.h"

template<typename T, size_t Capacity>
class Quadtree
{
public:
    struct Element
    {
        T data;
        Vector2 position;
    };
    
    Quadtree(const AABB& bounds);
    Quadtree(const Quadtree& other) = delete;
    Quadtree(Quadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(T data, const Vector2& position);
    bool Remove(T data, const Vector2& position);
    
    std::optional<Element> FindNearest(const Vector2& target) const;
    std::vector<Element> Query(const AABB& bounds) const;

    Quadtree& operator=(const Quadtree&) = delete;
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    void TryMerge();
    
    void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element>& nearest) const;
    void Query(const AABB& bounds, std::vector<Element>& elements) const;
    void GatherElements(std::vector<Element>& elements) const;
    
    AABB mBounds;
    bool mIsLeaf = true;
    std::vector<Element> mElements;
    std::array<std::unique_ptr<Quadtree>, 4> mChildren;
};

#include "Quadtree.inl"
