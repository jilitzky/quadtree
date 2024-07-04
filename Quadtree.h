#pragma once
#include "Region.h"

class Quadtree
{
public:
    Quadtree(Point center, int extent);

    bool Add(int value, Point position);
    bool Remove(int value, Point position);
    const Region& Root() const { return _root; }
    size_t Size() const { return _size; }

private:
    bool Contains(Point position) const;

    Region _root;
    Point _min = {};
    Point _max = {};
    size_t _size = 0;
};
