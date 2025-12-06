#pragma once
#include "Vector2.h"

struct AABB
{
    Vector2 min;
    Vector2 max;
    
    AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}
    
    float GetWidth() const
    {
        return max.x - min.x;
    }
    
    float GetHeight() const
    {
        return max.y - min.y;
    }
    
    Vector2 GetCenter() const
    {
        return (min + max) * 0.5f;
    }
    
    bool Contains(const AABB& other) const
    {
        return other.min.x >= min.x && other.max.x <= max.x && other.min.y >= min.y && other.max.y <= max.y;
    }
    
    bool Contains(const Vector2& position) const
    {
        return position.x >= min.x && position.y >= min.y && position.x <= max.x && position.y <= max.y;
    }
    
    bool Intersects(const AABB& other) const
    {
        if (max.x < other.min.x || min.x > other.max.x)
        {
            return false;
        }
        
        if (max.y < other.min.y || min.y > other.max.y)
        {
            return false;
        }

        return true;
    }
};
