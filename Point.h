#pragma once
#include <compare>

struct Point
{
    int x = 0;
    int y = 0;

    auto operator<=>(const Point&) const = default;

    Point& operator+=(const Point& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Point& operator-=(const Point& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Point& operator*=(int rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Point& operator/=(int rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    friend Point operator+(Point lhs, const Point& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Point operator-(Point lhs, const Point& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    friend Point operator*(Point lhs, int rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    friend Point operator/(Point lhs, int rhs)
    {
        lhs /= rhs;
        return lhs;
    }
};
