#pragma once
#include "Vector2.h"

struct AABB
{
    Vector2 min;
    Vector2 max;
    
    AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}
    
    bool Contains(const Vector2& point) const
    {
        return point.x >= min.x && point.y >= min.y && point.x <= max.x && point.y <= max.y;
    }
    
    int GetWidth() const
    {
        return max.x - min.x;
    }
    
    int GetHeight() const
    {
        return max.y - min.y;
    }
    
    Vector2 GetCenter() const
    {
        return (min + max) / 2;
    }
};
