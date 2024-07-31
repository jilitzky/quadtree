#pragma once
#include <compare>

struct Vector2
{
    int x;
    int y;

    auto operator<=>(const Vector2&) const = default;

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
