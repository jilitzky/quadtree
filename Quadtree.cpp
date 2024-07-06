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

const Point* Quadtree::FindNearest(const Point& point) const
{
    if (!_root.Contains(point))
    {
        return nullptr;
    }

    const double nearestDistance = _root.Width() + _root.Height();
    std::pair<double, const Point*> nearest(nearestDistance, nullptr);
    _root.FindNearest(point, nearest);
    return nearest.second;
}
