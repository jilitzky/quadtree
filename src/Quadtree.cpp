#include "Quadtree.h"

Quadtree::Quadtree(const AABB &bounds, size_t nodeCapacity, size_t maxDepth) :
    m_bounds(bounds),
    m_nodeCapacity(nodeCapacity),
    m_maxDepth(maxDepth),
    m_root(bounds)
{
}

bool Quadtree::Insert(const Vector2& position)
{
    if (!m_root.GetBounds().Contains(position))
    {
        return false;
    }

    const bool added = m_root.Add(position);
    if (added)
    {
        m_size++;
    }
    return added;
}

bool Quadtree::Remove(const Vector2& position)
{
    if (!m_root.GetBounds().Contains(position))
    {
        return false;
    }

    const bool removed = m_root.Remove(position);
    if (removed)
    {
        m_size--;
    }
    return removed;
}

std::vector<Vector2> Quadtree::QueryRange(const AABB& range) const
{
    // TODO: Implement this
    std::vector<Vector2> result;
    return result;
}

std::optional<Vector2> Quadtree::FindNearest(const Vector2& position, float maxRadiua) const
{
    if (!m_root.GetBounds().Contains(position))
    {
        return std::nullopt;
    }

    const AABB& rootBounds = m_root.GetBounds();
    const double maxDistance = rootBounds.GetWidth() + rootBounds.GetHeight();
    NearestPoint nearest{ std::nullopt, maxDistance };
    m_root.FindNearest(position, nearest);
    return nearest.point;
}

void Quadtree::Clear()
{
    // TODO: Implement this
}

size_t Quadtree::GetSize() const
{
    return m_size;
}

size_t Quadtree::GetDepth() const
{
    return m_root.Depth();
}

const AABB& Quadtree::GetBounds() const
{
    return m_bounds;
}

void Quadtree::Subdivide()
{
    // TODO: Implement me!
}
