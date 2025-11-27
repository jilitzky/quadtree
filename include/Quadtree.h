#pragma once
#include <optional>
#include "Node.h"

class Quadtree
{
public:
    Quadtree(const Vector2& min, const Vector2& max) : _root(min, max) {}

    size_t Depth() const { return _root.Depth(); }
    size_t Size() const { return _size; }

    bool Add(const Vector2& point);
    std::optional<Vector2> FindNearest(const Vector2& point) const;
    bool Remove(const Vector2& point);

private:
    Node _root;
    size_t _size = 0;
};
