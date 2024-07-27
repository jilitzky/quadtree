#pragma once
#include "Point.h"

struct Bounds
{
    Point min;
    Point max;

    Point Center() const
    {
        return (min + max) / 2;
    }

    bool Contains(const Point& point) const
    {
        return point >= min && point <= max;
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
