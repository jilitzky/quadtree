#pragma once
#include <optional>
#include <vector>
#include "AABB.h"

class NewQuadtree
{
public:
    NewQuadtree(const AABB& bounds);
    NewQuadtree(const NewQuadtree& other) = delete;
    NewQuadtree(NewQuadtree&& other) = default;
    
    const AABB& GetBounds() const;
    size_t GetSize() const;
    size_t GetHeight() const;
    
    bool Insert(const Vector2& position);

    NewQuadtree& operator=(const NewQuadtree&) = delete;
    NewQuadtree& operator=(NewQuadtree&&) = default;
    
private:
    int GetChildIndex(const Vector2& position) const;
    
    void Subdivide();
    
    static constexpr int kNodeCapacity = 1;
    AABB mBounds;
    bool mIsLeaf = true;
    std::vector<Vector2> mElements;
    std::array<std::unique_ptr<NewQuadtree>, 4> mChildren;
};
