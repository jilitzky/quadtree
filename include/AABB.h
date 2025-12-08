#pragma once
#include "Vector2.h"

/// An Axis-Aligned Bounding Box defined by its minimum and maximum points.
struct AABB
{
    /// The minimum point of the bounding box.
    Vector2 min;
    
    /// The maximum point of the bounding box.
    Vector2 max;
    
    /// Constructor that initializes the bounding box to the specified minimum and maximum points.
    /// @param min Bottom-left corner of the box.
    /// @param max Top-right corner of the box.
    AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}

    /// Calculates and returns the width of the bounding box.
    float GetWidth() const
    {
        return max.x - min.x;
    }
    
    /// Calculates and returns the height of the bounding box.
    float GetHeight() const
    {
        return max.y - min.y;
    }
    
    /// Calculates and returns the center point of the bounding box.
    Vector2 GetCenter() const
    {
        return (min + max) * 0.5f;
    }
    
    /// Returns true if this bounding box completely contains the other bounding box.
    /// @param other The other bounding box to check for containment.
    bool Contains(const AABB& other) const
    {
        return other.min.x >= min.x && other.max.x <= max.x && other.min.y >= min.y && other.max.y <= max.y;
    }
    
    /// Returns true if this bounding box contains the given position.
    /// @param position The position to check for containment.
    bool Contains(const Vector2& position) const
    {
        return position.x >= min.x && position.y >= min.y && position.x <= max.x && position.y <= max.y;
    }
    
    /// Returns true if this bounding box overlaps the other bounding box.
    /// @param other The other bounding box to check for intersection against.
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
