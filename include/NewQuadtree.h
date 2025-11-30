#pragma once
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

    NewQuadtree& operator=(const NewQuadtree&) = delete;
    NewQuadtree& operator=(NewQuadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    void TryMerge();
    
    AABB mBounds;
    int mNodeCapacity;
    bool mIsLeaf = true;
    std::vector<Vector2> mElements;
    std::array<std::unique_ptr<NewQuadtree>, 4> mChildren;
};
