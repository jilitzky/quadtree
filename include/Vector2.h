#pragma once

struct Vector2
{
    float x;
    float y;
    
    Vector2() : x(0.f), y(0.f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    
    float DistanceSquared(const Vector2& other) const
    {
        float dx = other.x - x;
        float dy = other.y - y;
        return (dx * dx) + (dy * dy);
    }
    
    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }
    
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }
    
    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }
    
    Vector2& operator+=(const Vector2 other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2& operator-=(const Vector2 other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};
