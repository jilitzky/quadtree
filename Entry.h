#pragma once
#include "Point.h"

struct Entry
{
    Entry(int value, Point position) : Value(value), Position(position) {}

    int Value;
    Point Position;
};
