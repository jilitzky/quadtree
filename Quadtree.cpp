#include "Quadtree.h"

Quadtree::Quadtree(Point center, int extent) : _root(center, extent / 2)
{
    const int halfExtent = extent / 2;
    _min = Point{ center.X - halfExtent, center.Y - halfExtent };
    _max = Point{ center.X + halfExtent, center.Y + halfExtent };
}

bool Quadtree::Add(int value, Point position)
{
    if (Contains(position))
    {
        _root.Add(value, position);
        _size++;
        return true;
    }
    return false;
}

bool Quadtree::Contains(Point position) const
{
    return position.X >= _min.X && position.X <= _max.X && position.Y >= _min.Y && position.Y <= _max.Y;
}
