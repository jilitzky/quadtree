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
    bool isLeaf = true;
    std::vector<Element> elements;
    std::array<int, 4> children = { -1, -1, -1, -1 };
    
    Node(const AABB& bounds) : bounds(bounds) {}
};
