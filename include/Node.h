#pragma once
#include "Vector2.h"

struct Node
{
    struct Element
    {
        Vector2 position;
        int data;
    };
    
    AABB bounds;
    bool isLeaf = true;
    std::vector<Element> elements;
    std::array<int, 4> children = { -1, -1, -1, -1 };
};
