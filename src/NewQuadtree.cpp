#include "NewQuadtree.h"

NewQuadtree::NewQuadtree(const AABB& bounds, int nodeCapacity) :
    m_bounds(bounds),
    m_nodeCapacity(nodeCapacity)
{
}

const AABB& NewQuadtree::GetBounds() const
{
    return m_bounds;
}

int NewQuadtree::GetSize() const
{
    // TODO: Implement me!
    return 0;
}

int NewQuadtree::GetDepth() const
{
    // TODO: Implement me!
    return 0;
}

bool NewQuadtree::Insert(const Vector2& position)
{
    if (!m_bounds.Contains(position))
    {
        return false;
    }

    if (!m_isLeaf)
    {
        int index = GetChildIndex(position);
        return m_children[index]->Insert(position);
    }

    m_elements.push_back({ position });

    bool canSubdivide = true; // TODO: Missing a min bounds check
    if (m_elements.size() > m_nodeCapacity && canSubdivide)
    {
        Subdivide();
    }

    return true;
}

int NewQuadtree::GetChildIndex(const Vector2& position) const
{
    int index = 0;
 
    Vector2 center = m_bounds.GetCenter();
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
    // TODO: Implement me!
}
