#pragma once

struct Point
{
    int X = 0;
    int Y = 0;

    bool operator==(Point rhs) const
    {
        return X == rhs.X && Y == rhs.Y;
    }
};
