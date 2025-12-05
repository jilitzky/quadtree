#pragma once
#include "AABB.h"
#include "Element.h"

template<typename T>
struct Node
{
    AABB bounds;
    union
    {
        std::array<int, 4> children = { -1, -1, -1, -1 };
        int nextFree;
    };
    std::vector<Element<T>> elements;
    bool isLeaf = true;
    
    Node(const AABB& bounds) : bounds(bounds) {}
};
