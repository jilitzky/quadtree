const AABB& LinearQuadtree::GetBounds() const
{
    return GetRoot().bounds;
}

size_t LinearQuadtree::GetSize() const
{
    const Node& root = GetRoot();
    return GetNodeSize(root);
}

size_t LinearQuadtree::GetHeight() const
{
    const Node& root = GetRoot();
    return GetNodeHeight(root);
}

const Node& LinearQuadtree::GetRoot() const
{
    return mNodes[0];
}

size_t LinearQuadtree::GetNodeSize(const Node& node) const
{
    if (node.isLeaf)
    {
        return node.elements.size();
    }
    
    size_t size = 0;
    for (auto childIndex : node.children)
    {
        const Node& child = mNodes[childIndex];
        size += GetNodeSize(child);
    }
    
    return size;
}

size_t LinearQuadtree::GetNodeHeight(const Node& node) const
{
    if (node.isLeaf)
    {
        return 1;
    }
    
    size_t height = 0;
    for (auto childIndex : node.children)
    {
        const Node& child = mNodes[childIndex];
        height = std::max(GetNodeHeight(child), height);
    }
    
    return height + 1;
}
