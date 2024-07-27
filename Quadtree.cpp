#include "Quadtree.h"

bool Quadtree::Add(const Point& point)
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

std::optional<Point> Quadtree::FindNearest(const Point& point) const
{
    if (!_root.GetBounds().Contains(point))
    {
        return std::nullopt;
    }

    const Bounds& rootBounds = _root.GetBounds();
    const double maxDistance = rootBounds.Width() + rootBounds.Height();
    NearestPoint nearest{ std::nullopt, maxDistance };
    _root.FindNearest(point, nearest);
    return nearest.point;
}

bool Quadtree::Remove(const Point& point)
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
