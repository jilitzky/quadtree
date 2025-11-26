#pragma once

struct Vector2
{
    int x;
    int y;
    
    Vector2(int x = 0, int y = 0) : x(x), y(y) {}
    
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
