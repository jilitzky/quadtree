#pragma once

/// A two-dimensional vector often used for points, directions, and displacement.
struct Vector2
{
    float x;
    float y;
    
    /// Default constructor that initializes the vector to (0, 0).
    Vector2() : x(0.f), y(0.f) {}

    /// Constructor that initializes the vector to the given components.
    /// @param x Initial value for the x-component.
    /// @param y Initial value for the y-component.
    Vector2(float x, float y) : x(x), y(y) {}
    
    /// Calculates the magnitude of the vector.
    float Length() const
    {
        return std::sqrt(LengthSquared());
    }
    
    /// Calculates the squared magnitude of the vector avoiding the expensive square root operation.
    float LengthSquared() const
    {
        return (x * x) + (y * y);
    }
    
    /// Returns the normalized (unit) version of this vector.
    Vector2 Normalize() const
    {
        float length = Length();
        if (length == 0.0f)
        {
            return Vector2(0.0f, 0.0f);
        }
        return *this / length;
    }
    
    /// Calculates the squared distance between this vector and the other vector.
    /// @param other The vector to measure the distance to.
    float DistanceSquared(const Vector2& other) const
    {
        float dx = other.x - x;
        float dy = other.y - y;
        return (dx * dx) + (dy * dy);
    }
    
    /// Calculates the scalar dot product with the other vector.
    /// @param other The other vector for the calculation.
    float Dot(const Vector2& other) const
    {
        return (x * other.x) + (y * other.y);
    }

    /// Calculates the 2D cross product with the other vector.
    /// @param other The other vector for the calculation.
    float Cross(const Vector2& other) const
    {
        return (x * other.y) - (y * other.x);
    }
    
    /// Checks if this vector is equal to the other vector.
    /// @param other The vector to compare against.
    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }
    
    /// Checks if this vector is different from the other vector.
    /// @param other The vector to compare against.
    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
    
    /// Adds this vector to the other vector and returns a new vector.
    /// @param other The vector to add.
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
    
    /// Subtracts the other vector from this vector and returns a new vector.
    /// @param other The vector to substract.
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }
    
    /// Multiplies this vector by a scalar and returns a new vector.
    /// @param scalar The value to multiply each component by.
    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }
    
    /// Divides this vector by a scalar and returns a new vector.
    /// @param scalar The value to divide each component by.
    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }
    
    /// Adds another vector to this one and returns the modified vector.
    /// @param other The vector to add.
    Vector2& operator+=(const Vector2 other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    /// Subtracts another vector from this one and returns the modified vector.
    /// @param other The vector to subtract.
    Vector2& operator-=(const Vector2 other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};
