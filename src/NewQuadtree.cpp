#include "NewQuadtree.h"

NewQuadtree::NewQuadtree(const AABB& bounds) : mBounds(bounds)
{
}

const AABB& NewQuadtree::GetBounds() const
{
    return mBounds;
}

size_t NewQuadtree::GetSize() const
{
    if (mIsLeaf)
    {
        return mElements.size();
    }
    
    size_t size = 0;
    for (const auto& child : mChildren)
    {
        size += child->GetSize();
    }
    
    return size;
}

size_t NewQuadtree::GetHeight() const
{
    if (mIsLeaf)
    {
        return 1;
    }
    
    size_t height = 0;
    for (const auto& child : mChildren)
    {
        height = std::max(child->GetHeight(), height);
    }
    
    return height + 1;
}

bool NewQuadtree::Insert(const Vector2& position)
{
    if (!mBounds.Contains(position))
    {
        return false;
    }

    if (!mIsLeaf)
    {
        int index = GetChildIndex(position);
        return mChildren[index]->Insert(position);
    }

    mElements.push_back({ position });

    bool canSubdivide = mBounds.GetWidth() >= 2.f && mBounds.GetHeight() >= 2.f;
    if (mElements.size() > kNodeCapacity && canSubdivide)
    {
        Subdivide();
    }

    return true;
}

int NewQuadtree::GetChildIndex(const Vector2& position) const
{
    int index = 0;
 
    Vector2 center = mBounds.GetCenter();
    if (position.x > center.x)
    {
        index += 1;
    }
    if (position.y < center.y)
    {
        index += 2;
    }
    
    return index;
}

void NewQuadtree::Subdivide()
{
    Vector2 min = mBounds.min;
    Vector2 max = mBounds.max;
    Vector2 center = mBounds.GetCenter();

    AABB nwBounds(Vector2(min.x, center.y), Vector2(center.x, max.y));
    AABB neBounds(Vector2(center.x, center.y), Vector2(max.x, max.y));
    AABB swBounds(Vector2(min.x, min.y), Vector2(center.x, center.y));
    AABB seBounds(Vector2(center.x, min.y), Vector2(max.x, center.y));

    mChildren[0] = std::make_unique<NewQuadtree>(nwBounds);
    mChildren[1] = std::make_unique<NewQuadtree>(neBounds);
    mChildren[2] = std::make_unique<NewQuadtree>(swBounds);
    mChildren[3] = std::make_unique<NewQuadtree>(seBounds);

    for (const auto& element : mElements)
    {
        int index = GetChildIndex(element);
        mChildren[index]->Insert(element);
    }

    mIsLeaf = false;
    mElements.clear();
}
