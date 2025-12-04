#pragma once
#include "Vector2.h"

template<typename T>
struct Node
{
    struct Element
    {
        T data;
        Vector2 position;
    };
    
    AABB bounds;
    union
    {
        std::array<int, 4> children = { -1, -1, -1, -1 };
        int nextFree;
    };
    std::vector<Element> elements;
    bool isLeaf = true;
    
    Node(const AABB& bounds) : bounds(bounds) {}
};
