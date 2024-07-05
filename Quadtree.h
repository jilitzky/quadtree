#pragma once
#include "Node.h"

class Quadtree
{
public:
    Quadtree(Point center, int extent);

    bool Add(Point point);
    bool Remove(Point point);
    const Node& Root() const { return _root; }
    size_t Size() const { return _size; }

private:
    bool Contains(Point point) const;

    Node _root;
    Point _min = {};
    Point _max = {};
    size_t _size = 0;
};
