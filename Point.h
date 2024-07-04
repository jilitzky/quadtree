#pragma once

struct Point
{
    bool operator==(Point rhs) const
    {
        return X == rhs.X && Y == rhs.Y;
    }

    int X = 0;
    int Y = 0;
};
