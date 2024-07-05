#pragma once
#include "Node.h"

class Quadtree
{
public:
    Quadtree(const Point& min, const Point& max);

    bool Add(const Point& point);
    const Point* FindNearest(const Point& point) const;
    bool Remove(const Point& point);
    const Node& Root() const { return _root; }
    size_t Size() const { return _size; }

private:
    Node _root;
    size_t _size = 0;
};
