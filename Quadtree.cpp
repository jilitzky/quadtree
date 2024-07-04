#include "Quadtree.h"

Quadtree::Quadtree(Point center, int extent) : _root(center, extent / 2)
{
    const int halfExtent = extent / 2;
    _min = Point{ center.X - halfExtent, center.Y - halfExtent };
    _max = Point{ center.X + halfExtent, center.Y + halfExtent };
}

bool Quadtree::Add(int value, Point position)
{
    if (!Contains(position))
    {
        return false;
    }

    _root.Add(value, position);
    _size++;
    return true;
}

bool Quadtree::Remove(int value, Point position)
{
    if (!Contains(position))
    {
        return false;
    }

    const bool removed = _root.Remove(value, position);
    if (removed)
    {
        _size--;
    }
    return removed;
}

bool Quadtree::Contains(Point position) const
{
    return position.X >= _min.X && position.X <= _max.X && position.Y >= _min.Y && position.Y <= _max.Y;
}
