#pragma once
#include <array>
#include <optional>
#include "Point.h"

struct NearestPoint
{
    std::optional<Point> point;
    double distance;
};

class Node
{
public:
    Node(const Point& min, const Point& max) : _min(min), _max(max) {}
    virtual ~Node();

    Point Center() const { return (_min + _max) / 2; }
    bool Contains(const Point& point) const { return point >= _min && point <= _max; }
    size_t Depth() const { return _depth; }
    int Height() const { return _max.y - _min.y; }
    int Width() const { return _max.x - _min.x; }

    bool Add(const Point& point);
    void FindNearest(const Point& point, NearestPoint& nearest) const;
    bool Remove(const Point& point);

private:
    int ChildCount() const;
    int ChildIndex(const Point& point) const;
    Node* GetOrCreateChild(const Point& point);
    void RefreshDepth();

    const Point _min;
    const Point _max;
    size_t _depth = 1;
    std::optional<Point> _point = std::nullopt;
    std::array<Node*, 4> _children = {};
};
