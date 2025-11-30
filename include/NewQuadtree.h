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
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(const Vector2& position);
    bool Remove(const Vector2& position);
    
    std::optional<Vector2> FindNearest(const Vector2& target) const;

    NewQuadtree& operator=(const NewQuadtree&) = delete;
    NewQuadtree& operator=(NewQuadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    void TryMerge();
    
    void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Vector2>& nearest) const;
    
    AABB mBounds;
    int mNodeCapacity;
    bool mIsLeaf = true;
    std::vector<Vector2> mElements;
    std::array<std::unique_ptr<NewQuadtree>, 4> mChildren;
};
