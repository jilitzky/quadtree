#include "NewQuadtree.h"

NewQuadtree::NewQuadtree(const AABB& bounds, int nodeCapacity) :
    mBounds(bounds),
    mNodeCapacity(nodeCapacity)
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
    if (mElements.size() > mNodeCapacity && canSubdivide)
    {
        Subdivide();
    }

    return true;
}

bool NewQuadtree::Remove(const Vector2& position)
{
    if (!mBounds.Contains(position))
    {
        return false;
    }

    if (mIsLeaf)
    {
        auto it = std::find(mElements.begin(), mElements.end(), position);
        if (it != mElements.end())
        {
            *it = mElements.back();
            mElements.pop_back();
            return true;
        }
        
        return false;
    }

    int index = GetChildIndex(position);
    if (mChildren[index]->Remove(position))
    {
        TryMerge();
        return true;
    }

    return false;
}

std::optional<Vector2> NewQuadtree::FindNearest(const Vector2& target) const
{
    std::optional<Vector2> nearest = std::nullopt;
    float bestDistanceSq = std::numeric_limits<float>::max();
    FindNearest(target, bestDistanceSq, nearest);
    return nearest;
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

    AABB topLeft(Vector2(min.x, center.y), Vector2(center.x, max.y));
    AABB topRight(Vector2(center.x, center.y), Vector2(max.x, max.y));
    AABB bottomLeft(Vector2(min.x, min.y), Vector2(center.x, center.y));
    AABB bottomRight(Vector2(center.x, min.y), Vector2(max.x, center.y));

    mChildren[0] = std::make_unique<NewQuadtree>(topLeft, mNodeCapacity);
    mChildren[1] = std::make_unique<NewQuadtree>(topRight, mNodeCapacity);
    mChildren[2] = std::make_unique<NewQuadtree>(bottomLeft, mNodeCapacity);
    mChildren[3] = std::make_unique<NewQuadtree>(bottomRight, mNodeCapacity);

    for (const auto& element : mElements)
    {
        int index = GetChildIndex(element);
        mChildren[index]->Insert(element);
    }

    mIsLeaf = false;
    mElements.clear();
}

void NewQuadtree::TryMerge()
{
    for (const auto& child : mChildren)
    {
        if (!child->mIsLeaf)
        {
            return;
        }
    }

    size_t totalSize = 0;
    for (const auto& child : mChildren)
    {
        totalSize += child->mElements.size();
    }

    if (totalSize <= mNodeCapacity)
    {
        mElements.reserve(totalSize);
        for (auto& child : mChildren)
        {
            for (auto& element : child->mElements)
            {
                mElements.push_back(std::move(element));
            }
        }

        for (auto& child : mChildren)
        {
            child.reset();
        }

        mIsLeaf = true;
    }
}

void NewQuadtree::FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Vector2>& nearest) const
{
    for (const auto& element : mElements)
    {
        float distanceSq = element.DistanceSquared(target);
        if (distanceSq < bestDistanceSq)
        {
            bestDistanceSq = distanceSq;
            nearest = element;
        }
    }

    if (mIsLeaf)
    {
        return;
    }
    
    Vector2 center = mBounds.GetCenter();
    int isRight = target.x >= center.x;
    int isBottom = target.y < center.y;

    std::array<int, 4> sortedIndices;
    sortedIndices[0] = isBottom * 2 + isRight;
    sortedIndices[1] = isBottom * 2 + (1 - isRight);
    sortedIndices[2] = (1 - isBottom) * 2 + isRight;
    sortedIndices[3] = (1 - isBottom) * 2 + (1 - isRight);

    for (int index : sortedIndices)
    {
        const auto& child = mChildren[index];
        AABB childBounds = child->mBounds;
        float dx = std::max({childBounds.min.x - target.x, 0.0f, target.x - childBounds.max.x});
        float dy = std::max({childBounds.min.y - target.y, 0.0f, target.y - childBounds.max.y});
        float childDistSq = (dx * dx) + (dy * dy);
        if (childDistSq < bestDistanceSq)
        {
            child->FindNearest(target, bestDistanceSq, nearest);
        }
    }
}
