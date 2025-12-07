template<typename T, size_t Capacity>
Quadtree<T, Capacity>::Quadtree(const AABB& bounds) : mBounds(bounds)
{
}

template<typename T, size_t Capacity>
const AABB& Quadtree<T, Capacity>::GetBounds() const
{
    return mBounds;
}

template<typename T, size_t Capacity>
size_t Quadtree<T, Capacity>::GetSize() const
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

template<typename T, size_t Capacity>
size_t Quadtree<T, Capacity>::GetHeight() const
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

template<typename T, size_t Capacity>
bool Quadtree<T, Capacity>::Insert(T data, const Vector2& position)
{
    if (!mBounds.Contains(position))
    {
        return false;
    }

    if (!mIsLeaf)
    {
        int index = GetChildIndex(position);
        return mChildren[index]->Insert(data, position);
    }

    mElements.push_back({data, position});

    bool canSubdivide = mBounds.GetWidth() >= 2.f && mBounds.GetHeight() >= 2.f;
    if (mElements.size() > Capacity && canSubdivide)
    {
        Subdivide();
    }

    return true;
}

template<typename T, size_t Capacity>
bool Quadtree<T, Capacity>::Remove(T data, const Vector2& position)
{
    if (!mBounds.Contains(position))
    {
        return false;
    }

    if (mIsLeaf)
    {
        auto it = std::find_if(mElements.begin(), mElements.end(), [&](const Element& element)
        {
            return element.data == data;
        });
        
        if (it != mElements.end())
        {
            *it = mElements.back();
            mElements.pop_back();
            return true;
        }
        
        return false;
    }

    int index = GetChildIndex(position);
    if (mChildren[index]->Remove(data, position))
    {
        TryMerge();
        return true;
    }

    return false;
}

template<typename T, size_t Capacity>
std::optional<typename Quadtree<T, Capacity>::Element> Quadtree<T, Capacity>::FindNearest(const Vector2& target) const
{
    std::optional<Element> nearest = std::nullopt;
    float bestDistanceSq = std::numeric_limits<float>::max();
    FindNearest(target, bestDistanceSq, nearest);
    return nearest;
}

template<typename T, size_t Capacity>
std::vector<typename Quadtree<T, Capacity>::Element> Quadtree<T, Capacity>::Query(const AABB& bounds) const
{
    std::vector<Element> elements;
    Query(bounds, elements);
    return elements;
}

template<typename T, size_t Capacity>
int Quadtree<T, Capacity>::GetChildIndex(const Vector2& position) const
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

template<typename T, size_t Capacity>
void Quadtree<T, Capacity>::Subdivide()
{
    Vector2 min = mBounds.min;
    Vector2 max = mBounds.max;
    Vector2 center = mBounds.GetCenter();

    AABB topLeft(Vector2(min.x, center.y), Vector2(center.x, max.y));
    AABB topRight(Vector2(center.x, center.y), Vector2(max.x, max.y));
    AABB bottomLeft(Vector2(min.x, min.y), Vector2(center.x, center.y));
    AABB bottomRight(Vector2(center.x, min.y), Vector2(max.x, center.y));

    mChildren[0] = std::make_unique<Quadtree>(topLeft);
    mChildren[1] = std::make_unique<Quadtree>(topRight);
    mChildren[2] = std::make_unique<Quadtree>(bottomLeft);
    mChildren[3] = std::make_unique<Quadtree>(bottomRight);

    for (const auto& element : mElements)
    {
        int index = GetChildIndex(element.position);
        mChildren[index]->Insert(element.data, element.position);
    }

    mIsLeaf = false;
    mElements.clear();
}

template<typename T, size_t Capacity>
void Quadtree<T, Capacity>::TryMerge()
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

    if (totalSize <= Capacity)
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

template<typename T, size_t Capacity>
void Quadtree<T, Capacity>::FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element>& nearest) const
{
    for (const auto& element : mElements)
    {
        float distanceSq = element.position.DistanceSquared(target);
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
        float childDistanceSq = (dx * dx) + (dy * dy);
        if (childDistanceSq < bestDistanceSq)
        {
            child->FindNearest(target, bestDistanceSq, nearest);
        }
    }
}

template<typename T, size_t Capacity>
void Quadtree<T, Capacity>::Query(const AABB& bounds, std::vector<Element>& elements) const
{
    if (!mBounds.Intersects(bounds))
    {
        return;
    }

    if (bounds.Contains(mBounds))
    {
        GatherElements(elements);
        return;
    }

    if (mIsLeaf)
    {
        for (const auto& element : mElements)
        {
            if (bounds.Contains(element.position))
            {
                elements.push_back(element);
            }
        }
        return;
    }

    for (const auto& child : mChildren)
    {
        child->Query(bounds, elements);
    }
}

template<typename T, size_t Capacity>
void Quadtree<T, Capacity>::GatherElements(std::vector<Element>& elements) const
{
    if (mIsLeaf)
    {
        elements.insert(elements.end(), mElements.begin(), mElements.end());
        return;
    }
    
    for (const auto& child : mChildren)
    {
        child->GatherElements(elements);
    }
}
