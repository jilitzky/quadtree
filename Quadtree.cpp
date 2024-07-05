#include "Quadtree.h"

Quadtree::Quadtree(const Point& min, const Point& max) : _root(min, max)
{
}

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

const Point* Quadtree::FindNearest(const Point& point) const
{
    if (!_root.Contains(point))
    {
        return nullptr;
    }

    const double maxDistance = _root.Width() + _root.Height();
    std::pair<double, const Point*> nearest(maxDistance, nullptr);
    _root.FindNearest(point, nearest);
    return nearest.second;
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
