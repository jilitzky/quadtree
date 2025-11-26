#pragma once
#include "Vector2.h"

struct Bounds
{
    Vector2 min;
    Vector2 max;

    Vector2 Center() const
    {
        return (min + max) / 2;
    }

    bool Contains(const Vector2& point) const
    {
        return point.x >= min.x && point.y >= min.y && point.x <= max.x && point.y <= max.y;
    }

    int Height() const
    {
        return max.y - min.y;
    }

    int Width() const
    {
        return max.x - min.x;
    }
};
