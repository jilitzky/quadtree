template<typename T, size_t Capacity>
LinearQuadtree<T, Capacity>::LinearQuadtree(const AABB& bounds)
{
    mNodes.emplace_back(bounds);
}

template<typename T, size_t Capacity>
const AABB& LinearQuadtree<T, Capacity>::GetBounds() const
{
    return mNodes[0].bounds;
}

template<typename T, size_t Capacity>
size_t LinearQuadtree<T, Capacity>::GetSize() const
{
    return GetSize(0);
}

template<typename T, size_t Capacity>
size_t LinearQuadtree<T, Capacity>::GetHeight() const
{
    return GetHeight(0);
}

template<typename T, size_t Capacity>
bool LinearQuadtree<T, Capacity>::Insert(T data, const Vector2& position)
{
    return Insert(0, data, position);
}

template<typename T, size_t Capacity>
bool LinearQuadtree<T, Capacity>::Remove(T data, const Vector2& position)
{
    return Remove(0, data, position);
}

template<typename T, size_t Capacity>
size_t LinearQuadtree<T, Capacity>::GetSize(int nodeIndex) const
{
    const Node<T>& node = mNodes[nodeIndex];
    
    if (node.isLeaf)
    {
        return node.elements.size();
    }
    
    size_t size = 0;
    for (auto childIndex : node.children)
    {
        size += GetSize(childIndex);
    }
    
    return size;
}

template<typename T, size_t Capacity>
size_t LinearQuadtree<T, Capacity>::GetHeight(int nodeIndex) const
{
    const Node<T>& node = mNodes[nodeIndex];
    
    if (node.isLeaf)
    {
        return 1;
    }
    
    size_t height = 0;
    for (auto childIndex : node.children)
    {
        height = std::max(GetHeight(childIndex), height);
    }
    
    return height + 1;
}

template<typename T, size_t Capacity>
int LinearQuadtree<T, Capacity>::GetChildIndex(int nodeIndex, const Vector2& position) const
{
    const Node<T>& node = mNodes[nodeIndex];
    
    int offset = 0;
    Vector2 center = node.bounds.GetCenter();
    if (position.x > center.x)
    {
        offset += 1;
    }
    if (position.y < center.y)
    {
        offset += 2;
    }
    
    return mNodes[nodeIndex].children[offset];
}

template<typename T, size_t Capacity>
bool LinearQuadtree<T, Capacity>::Insert(int nodeIndex, T data, const Vector2& position)
{
    if (!mNodes[nodeIndex].bounds.Contains(position))
    {
        return false;
    }

    if (!mNodes[nodeIndex].isLeaf)
    {
        int childIndex = GetChildIndex(nodeIndex, position);
        return Insert(childIndex, data, position);
    }

    mNodes[nodeIndex].elements.push_back({ data, position });
    
    if (mNodes[nodeIndex].elements.size() > Capacity)
    {
        Subdivide(nodeIndex);
    }

    return true;
}

template<typename T, size_t Capacity>
bool LinearQuadtree<T, Capacity>::Remove(int nodeIndex, T data, const Vector2& position)
{
    if (!mNodes[nodeIndex].bounds.Contains(position))
    {
        return false;
    }

    if (mNodes[nodeIndex].isLeaf)
    {
        auto& elements = mNodes[nodeIndex].elements;
        auto it = std::find_if(elements.begin(), elements.end(), [&](const auto& element)
        {
            return element.data == data;
        });
        
        if (it != elements.end())
        {
            *it = elements.back();
            elements.pop_back();
            return true;
        }
        
        return false;
    }

    int childIndex = GetChildIndex(nodeIndex, position);
    if (Remove(childIndex, data, position))
    {
        TryMerge(nodeIndex);
        return true;
    }

    return false;
}

template<typename T, size_t Capacity>
std::optional<Element<T>> LinearQuadtree<T, Capacity>::FindNearest(const Vector2& position) const
{
    std::optional<Element<T>> nearest = std::nullopt;
    float bestDistanceSq = std::numeric_limits<float>::max(); // TODO: Allow a radius to be passed in as an optional parameter
    FindNearest(0, position, bestDistanceSq, nearest);
    return nearest;
}

template<typename T, size_t Capacity>
void LinearQuadtree<T, Capacity>::Subdivide(int nodeIndex)
{
    AABB bounds = mNodes[nodeIndex].bounds;
    Vector2 min = bounds.min;
    Vector2 max = bounds.max;
    Vector2 center = bounds.GetCenter();

    AABB topLeft(Vector2(min.x, center.y), Vector2(center.x, max.y));
    AABB topRight(Vector2(center.x, center.y), Vector2(max.x, max.y));
    AABB bottomLeft(Vector2(min.x, min.y), Vector2(center.x, center.y));
    AABB bottomRight(Vector2(center.x, min.y), Vector2(max.x, center.y));

    int topLeftIndex = AllocateNode(topLeft);
    int topRightIndex = AllocateNode(topRight);
    int bottomLeftIndex = AllocateNode(bottomLeft);
    int bottomRightIndex = AllocateNode(bottomRight);
    
    mNodes[nodeIndex].children[0] = topLeftIndex;
    mNodes[nodeIndex].children[1] = topRightIndex;
    mNodes[nodeIndex].children[2] = bottomLeftIndex;
    mNodes[nodeIndex].children[3] = bottomRightIndex;
    mNodes[nodeIndex].isLeaf = false;
    
    for (const auto& element : mNodes[nodeIndex].elements)
    {
        int childIndex = GetChildIndex(nodeIndex, element.position);
        Insert(childIndex, element.data, element.position);
    }
    mNodes[nodeIndex].elements.clear();
}

template<typename T, size_t Capacity>
void LinearQuadtree<T, Capacity>::TryMerge(int nodeIndex)
{
    for (int childIndex : mNodes[nodeIndex].children)
    {
        if (!mNodes[childIndex].isLeaf)
        {
            return;
        }
    }

    size_t totalElements = 0;
    for (int childIndex : mNodes[nodeIndex].children)
    {
        totalElements += mNodes[childIndex].elements.size();
    }

    if (totalElements <= Capacity)
    {
        mNodes[nodeIndex].elements.reserve(totalElements);
        for (int childIndex : mNodes[nodeIndex].children)
        {
            for (auto& element : mNodes[childIndex].elements)
            {
                mNodes[nodeIndex].elements.push_back(std::move(element));
            }
        }

        for (int childIndex : mNodes[nodeIndex].children)
        {
            FreeNode(childIndex);
        }

        mNodes[nodeIndex].children.fill(-1);
        mNodes[nodeIndex].isLeaf = true;
    }
}

template<typename T, size_t Capacity>
void LinearQuadtree<T, Capacity>::FindNearest(int nodeIndex, const Vector2& position, float& bestDistanceSq, std::optional<Element<T>>& nearest) const
{
    const Node<T>& node = mNodes[nodeIndex];
    
    for (const auto& element : node.elements)
    {
        float distanceSq = element.position.DistanceSquared(position);
        if (distanceSq < bestDistanceSq)
        {
            bestDistanceSq = distanceSq;
            nearest = element;
        }
    }

    if (node.isLeaf)
    {
        return;
    }
    
    Vector2 center = node.bounds.GetCenter();
    int isRight = position.x >= center.x;
    int isBottom = position.y < center.y;

    std::array<int, 4> sortedIndices;
    sortedIndices[0] = isBottom * 2 + isRight;
    sortedIndices[1] = isBottom * 2 + (1 - isRight);
    sortedIndices[2] = (1 - isBottom) * 2 + isRight;
    sortedIndices[3] = (1 - isBottom) * 2 + (1 - isRight);

    for (int sortedIndex : sortedIndices)
    {
        int childIndex = node.children[sortedIndex];
        AABB childBounds = mNodes[childIndex].bounds;
        float distanceX = std::max({childBounds.min.x - position.x, 0.0f, position.x - childBounds.max.x});
        float distanceY = std::max({childBounds.min.y - position.y, 0.0f, position.y - childBounds.max.y});
        float distanceSq = (distanceX * distanceX) + (distanceY * distanceY);
        if (distanceSq < bestDistanceSq)
        {
            FindNearest(childIndex, position, bestDistanceSq, nearest);
        }
    }
}

template<typename T, size_t Capacity>
int LinearQuadtree<T, Capacity>::AllocateNode(const AABB& bounds)
{
    int index;

    if (mFreeHead != -1)
    {
        index = mFreeHead;
        mFreeHead = mNodes[index].nextFree;
        mNodes[index].bounds = bounds;
        mNodes[index].children.fill(-1);
        mNodes[index].isLeaf = true;
    }
    else
    {
        index = static_cast<int>(mNodes.size());
        mNodes.emplace_back(bounds);
    }

    return index;
}

template<typename T, size_t Capacity>
void LinearQuadtree<T, Capacity>::FreeNode(int nodeIndex)
{
    mNodes[nodeIndex].elements.clear();
    mNodes[nodeIndex].nextFree = mFreeHead;
    mFreeHead = nodeIndex;
}
