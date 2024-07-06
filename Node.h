#pragma once
#include <utility>
#include "Point.h"

class Node
{
public:
    Node(const Point& min, const Point& max);
    virtual ~Node();

    bool Add(const Point& point);
    Point Center() const { return (_min + _max) / 2; }
    bool Contains(const Point& point) const;
    bool Empty() const;
    void FindNearest(const Point& point, std::pair<double, const Point*>& nearest) const;
    int Height() const { return _max.y - _min.y; }
    bool Remove(const Point& point);
    int Width() const { return _max.x - _min.x; }

private:
    int GetChildIndex(const Point& point) const;
    Node* GetOrCreateChild(const Point& point);

    const Point _min;
    const Point _max;
    bool _leaf = true;
    Point* _point = nullptr;
    Node* _children[4] = {};
};
