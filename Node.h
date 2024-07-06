#pragma once
#include <utility>
#include "Point.h"

class Node
{
public:
    Node(const Point& min, const Point& max);
    virtual ~Node();

    int Width() const { return _max.x - _min.x; }
    int Height() const { return _max.y - _min.y; }
    Point Center() const { return (_min + _max) / 2; }

    bool Add(const Point& point);
    bool Remove(const Point& point);
    bool Contains(const Point& point) const;
    bool IsEmpty() const;

    void FindNearest(const Point& point, std::pair<double, const Point*>& nearest) const;

private:
    int GetChildIndex(const Point& point) const;
    Node* GetOrCreateChild(const Point& point);

    const Point _min;
    const Point _max;
    bool _leaf = true;
    Point* _point = nullptr;
    Node* _children[4] = {};
};
