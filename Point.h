#pragma once
#include <compare>

struct Point
{
    int X = 0;
    int Y = 0;

    auto operator<=>(const Point&) const = default;

    Point& operator+=(const Point& rhs)
    {
        X += rhs.X;
        Y += rhs.Y;
        return *this;
    }

    Point& operator-=(const Point& rhs)
    {
        X -= rhs.X;
        Y -= rhs.Y;
        return *this;
    }

    Point& operator*=(int rhs)
    {
        X *= rhs;
        Y *= rhs;
        return *this;
    }

    Point& operator/=(int rhs)
    {
        X /= rhs;
        Y /= rhs;
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
