#pragma once
#include "Node.h"

class Quadtree
{
public:
    Quadtree(const Point& min, const Point& max) : _root(min, max) {}

    size_t Size() const { return _size; }

    bool Add(const Point& point);
    bool Remove(const Point& point);
    const Point* FindNearest(const Point& point) const;

private:
    Node _root;
    size_t _size = 0;
};
