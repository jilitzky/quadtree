#include "Quadtree.h"

Quadtree::Quadtree(Point center, int extent) : _root(center, extent / 2)
{
    const int halfExtent = extent / 2;
    _min = Point{ center.X - halfExtent, center.Y - halfExtent };
    _max = Point{ center.X + halfExtent, center.Y + halfExtent };
}

bool Quadtree::Add(Point point)
{
    if (!Contains(point))
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

bool Quadtree::Remove(Point point)
{
    if (!Contains(point))
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

bool Quadtree::Contains(Point point) const
{
    return point.X >= _min.X && point.X <= _max.X && point.Y >= _min.Y && point.Y <= _max.Y;
}
