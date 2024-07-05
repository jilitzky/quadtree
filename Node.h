#pragma once
#include "Point.h"

class Node
{
public:
    Node(const Point& min, const Point& max);
    virtual ~Node();

    bool Add(const Point& point);
    bool Contains(const Point& point) const;
    bool Empty() const;
    bool Remove(const Point& point);

private:
    Point GetCenter() const { return (_min + _max) / 2; }
    int GetChildIndex(const Point& point) const;
    int GetExtentX() const { return _max.X - _min.X; }
    int GetExtentY() const { return _max.Y - _min.Y; }
    Node* GetOrCreateChild(const Point& point);

    const Point _min;
    const Point _max;
    bool _leaf = true;
    Point* _point = nullptr;
    Node* _children[4] = {};
};
