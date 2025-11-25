#pragma once
#include <compare>

struct Vector2
{
    int x;
    int y;

    bool operator==(const Vector2& other) const
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }

    bool operator<(const Vector2& other) const
    {
        if (x != other.x)
        {
            return x < other.x;
        }
        return y < other.y;
    }

    bool operator>(const Vector2& other) const
    {
        return other < *this;
    }

    bool operator<=(const Vector2& other) const
    {
        return !(*this > other);
    }

    bool operator>=(const Vector2& other) const
    {
        return !(*this < other);
    }

    Vector2& operator+=(const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2& operator*=(int rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vector2& operator/=(int rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    friend Vector2 operator+(Vector2 lhs, const Vector2& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Vector2 operator-(Vector2 lhs, const Vector2& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend Vector2 operator*(Vector2 lhs, int rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    friend Vector2 operator/(Vector2 lhs, int rhs)
    {
        lhs /= rhs;
        return lhs;
    }
};
