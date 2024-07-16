#include "Quadtree.h"

bool Quadtree::Add(const Point& point)
{
    if (!_root.Contains(point))
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
    if (!_root.Contains(point))
    {
        return std::nullopt;
    }

    const double maxDistance = _root.Width() + _root.Height();
    NearestPoint nearest{ std::optional<Point>(), maxDistance};
    _root.FindNearest(point, nearest);
    return nearest.point;
}

bool Quadtree::Remove(const Point& point)
{
    if (!_root.Contains(point))
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
