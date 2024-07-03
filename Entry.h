#pragma once
#include "Point.h"

struct Entry
{
    int Value = -1;
    Point Position = {};
    Entry* Next = nullptr;
};
