#pragma once

#include "Vector2.h"

/// An Axis-Aligned Bounding Box (AABB) defined by its minimum and maximum points.
struct AABB
{
    /// The bottom-left corner of the bounding box.
    Vector2 min;
    /// The top-right corner of the bounding box.
    Vector2 max;
    
    /// Constructor that initializes the bounding box to the given minimum and maximum points.
    /// @param min The minimum point (bottom-left corner).
    /// @param max The maximum point (top-right corner).
    AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}

    /// Calculates the width of the bounding box.
    /// @return The measured width.
    float GetWidth() const
    {
        return max.x - min.x;
    }
    
    /// Calculates the height of the bounding box.
    /// @return The measured height.
    float GetHeight() const
    {
        return max.y - min.y;
    }
    
    /// Calculates the center point of the bounding box.
    /// @return The position representing the center point.
    Vector2 GetCenter() const
    {
        return (min + max) * 0.5f;
    }
    
    /// Returns true if this bounding box completely contains the other box.
    /// @param other The other box to check.
    /// @return True if the other box is entirely within this box, false otherwise.
    bool Contains(const AABB& other) const
    {
        return other.min.x >= min.x && other.max.x <= max.x && other.min.y >= min.y && other.max.y <= max.y;
    }
    
    /// Returns true if this bounding box contains the given position.
    /// @param position The position to check.
    /// @return True if the position is inside or on the boundary of this box, false otherwise.
    bool Contains(const Vector2& position) const
    {
        return position.x >= min.x && position.y >= min.y && position.x <= max.x && position.y <= max.y;
    }
    
    /// Returns true if this bounding box overlaps the other box.
    /// @param other The other box to check against.
    /// @return True if the two boxes overlap, false otherwise.
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
