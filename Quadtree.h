#pragma once
#include "Node.h"

class Quadtree
{
public:
    Quadtree(Point center, int extent);

    bool Add(int value, Point position);
    bool Remove(int value, Point position);
    const Node& Root() const { return _root; }
    size_t Size() const { return _size; }

private:
    bool Contains(Point position) const;

    Node _root;
    Point _min = {};
    Point _max = {};
    size_t _size = 0;
};
