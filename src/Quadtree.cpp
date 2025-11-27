#include "Quadtree.h"

bool Quadtree::Add(const Vector2& point)
{
    if (!_root.GetBounds().Contains(point))
    {
        return false;
    }

    const bool added = _root.Add(point);
    if (added)
    {
        _size++;
    }
    return added;
}

std::optional<Vector2> Quadtree::FindNearest(const Vector2& point) const
{
    if (!_root.GetBounds().Contains(point))
    {
        return std::nullopt;
    }

    const AABB& rootBounds = _root.GetBounds();
    const double maxDistance = rootBounds.GetWidth() + rootBounds.GetHeight();
    NearestPoint nearest{ std::nullopt, maxDistance };
    _root.FindNearest(point, nearest);
    return nearest.point;
}

bool Quadtree::Remove(const Vector2& point)
{
    if (!_root.GetBounds().Contains(point))
    {
        return false;
    }

    const bool removed = _root.Remove(point);
    if (removed)
    {
        _size--;
    }
    return removed;
}
