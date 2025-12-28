/// Copyright (c) 2025 Jose Ilitzky

#pragma once

#include <algorithm>
#include <cmath>

/// A two-dimensional vector used to represent positions, directions and offsets.
struct Vector2
{
    /// Value along the X-axis.
    float x;
    
    /// Value along the Y-axis.
    float y;
    
    /// Default constructor that initializes the vector to (0, 0).
    Vector2() : x(0.f), y(0.f) {}

    /// Constructor that initializes the vector to the given values.
    /// @param x Initial value for x.
    /// @param y Initial value for y.
    Vector2(float x, float y) : x(x), y(y) {}
    
    /// Calculates the magnitude of the vector.
    /// @return The length of the vector.
    float Length() const
    {
        return std::sqrt(LengthSquared());
    }
    
    /// Calculates the squared magnitude of the vector, avoiding the expensive square root operation needed for magnitude.
    /// @return The squared length of the vector.
    float LengthSquared() const
    {
        return (x * x) + (y * y);
    }
    
    /// Calculates the normalized (unit) version of this vector.
    /// @return A new vector with a length of one, preserving direction. Returns (0, 0) if the original length was zero.
    Vector2 Normalize() const
    {
        float length = Length();
        if (length == 0.f)
        {
            return Vector2(0.f, 0.f);
        }
        return *this / length;
    }
    
    /// Calculates the squared distance between this vector and the other vector.
    /// @param other The vector to measure the squared distance to.
    /// @return The squared distance between the two vectors.
    float DistanceSquared(const Vector2& other) const
    {
        float dx = other.x - x;
        float dy = other.y - y;
        return (dx * dx) + (dy * dy);
    }
    
    /// Calculates the scalar dot product with the other vector.
    /// @param other The other vector for the calculation.
    /// @return The scalar dot product.
    float Dot(const Vector2& other) const
    {
        return (x * other.x) + (y * other.y);
    }

    /// Calculates the 2D cross product with the other vector.
    /// @param other The other vector for the calculation.
    /// @return The scalar Z-component of the cross product.
    float Cross(const Vector2& other) const
    {
        return (x * other.y) - (y * other.x);
    }
    
    /// Calculates the signed angle in radians between this vector and the other.
    /// @param other The other vector used in the angle calculation.
    /// @return The signed angle in radians between the two vectors.
    float SignedAngle(const Vector2& other) const
    {
        float cross = Cross(other);
        float dot = Dot(other);
        return std::atan2(cross, dot);
    }
    
    /// Calculates the absolute angle in radians between this vector and the other.
    /// @param other The other vector used in the angle calculation.
    /// @return The absolute angle in radians between the two vectors.
    float Angle(const Vector2& other) const
    {
        return std::abs(SignedAngle(other));
    }
    
    /// Rotates the vector by a given angle in radians.
    /// @param angle The angle in radians to rotate by.
    /// @return A new vector rotated by the given angle.
    Vector2 Rotate(float angle) const
    {
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);
        return { x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle };
    }
    
    /// Checks if this vector is equal to the other vector.
    /// @param other The vector to compare against.
    /// @return True if both x and y components are equal, false otherwise.
    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }
    
    /// Checks if this vector is different from the other vector.
    /// @param other The vector to compare against.
    /// @return True if any component is different, false otherwise.
    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
    
    /// Adds this vector to the other vector and returns a new vector.
    /// @param other The vector to add.
    /// @return A new vector representing the sum of the two vectors.
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
    
    /// Subtracts the other vector from this vector and returns a new vector.
    /// @param other The vector to substract.
    /// @return A new vector representing the difference between the two vectors.
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }
    
    /// Multiplies this vector by a scalar and returns a new vector.
    /// @param scalar The value to multiply each component by.
    /// @return A new vector multiplied by the scalar.
    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }
    
    /// Divides this vector by a scalar and returns a new vector.
    /// @param scalar The value to divide each component by.
    /// @return A new vector divided by the scalar.
    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }
    
    /// Adds another vector to this one and returns the modified vector.
    /// @param other The vector to add.
    /// @return A reference to the modified vector.
    Vector2& operator+=(const Vector2 other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    /// Subtracts another vector from this one and returns the modified vector.
    /// @param other The vector to subtract.
    /// @return A reference to the modified vector.
    Vector2& operator-=(const Vector2 other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};
