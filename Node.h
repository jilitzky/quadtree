#pragma once
#include <array>
#include <memory>
#include <optional>
#include "Bounds.h"
#include "Point.h"

struct NearestPoint
{
    std::optional<Point> point;
    double distance;
};

class Node
{
public:
    Node(const Point& min, const Point& max) : _bounds(min, max) {}

    Bounds GetBounds() const { return _bounds; }
    size_t Depth() const { return _depth; }

    bool Add(const Point& point);
    void FindNearest(const Point& point, NearestPoint& nearest) const;
    bool Remove(const Point& point);

private:
    int ChildCount() const;
    int ChildIndex(const Point& point) const;
    std::unique_ptr<Node>& GetOrCreateChild(const Point& point);
    void RefreshDepth();

    const Bounds _bounds;
    size_t _depth = 1;
    std::optional<Point> _point = std::nullopt;
    std::array<std::unique_ptr<Node>, 4> _children = {};
};
