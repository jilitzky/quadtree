#pragma once
#include "Vector2.h"

/// An Axis-Aligned Bounding Box (AABB) defined by its minimum and maximum points.
struct AABB
{
    /// The minimum point of the bounding box (the bottom-left corner).
    Vector2 min;
    /// The maximum point of the bounding box (the top-right corner).
    Vector2 max;
    
    /// Constructor that initializes the AABB to the given minimum and maximum points.
    /// @param min Bottom-left corner of the box.
    /// @param max Top-right corner of the box.
    AABB(const Vector2& min, const Vector2& max) : min(min), max(max) {}

    /// Calculates and returns the width of the bounding box.
    /// @return The width of the bounding box.
    float GetWidth() const
    {
        return max.x - min.x;
    }
    
    /// Calculates and returns the height of the bounding box.
    /// @return The height of the bounding box.
    float GetHeight() const
    {
        return max.y - min.y;
    }
    
    /// Calculates and returns the center point of the bounding box.
    /// @return The vector representing the center point.
    Vector2 GetCenter() const
    {
        return (min + max) * 0.5f;
    }
    
    /// Returns true if this AABB completely contains the other AABB.
    /// @param other The other AABB to check for containment.
    /// @return True if the other AABB is entirely within this box, false otherwise.
    bool Contains(const AABB& other) const
    {
        return other.min.x >= min.x && other.max.x <= max.x && other.min.y >= min.y && other.max.y <= max.y;
    }
    
    /// Returns true if this AABB contains the given position.
    /// @param position The position to check for containment.
    /// @return True if the position is inside or on the boundary of this AABB, false otherwise.
    bool Contains(const Vector2& position) const
    {
        return position.x >= min.x && position.y >= min.y && position.x <= max.x && position.y <= max.y;
    }
    
    /// Returns true if this AABB overlaps the other AABB.
    /// @param other The other AABB to check for intersection against.
    /// @return True if the two AABBs overlap, false otherwise.
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
