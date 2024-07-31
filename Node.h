#pragma once
#include <array>
#include <memory>
#include <optional>
#include "Bounds.h"
#include "Vector2.h"

struct NearestPoint
{
    std::optional<Vector2> point;
    double distance;
};

class Node
{
public:
    Node(const Vector2& min, const Vector2& max);

    const Bounds& GetBounds() const { return _bounds; }
    size_t Depth() const { return _depth; }

    bool Add(const Vector2& point);
    void FindNearest(const Vector2& point, NearestPoint& nearest) const;
    bool Remove(const Vector2& point);

private:
    int ChildCount() const;
    int ChildIndex(const Vector2& point) const;
    std::unique_ptr<Node>& GetOrCreateChild(const Vector2& point);
    void RefreshDepth();

    Bounds _bounds;
    size_t _depth = 1;
    std::optional<Vector2> _point = std::nullopt;
    std::array<std::unique_ptr<Node>, 4> _children = {};
};
