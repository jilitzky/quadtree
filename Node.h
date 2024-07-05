#pragma once
#include "Point.h"

class Node
{
public:
    Node(Point center, int halfExtent);
    virtual ~Node();

    bool Add(Point point);
    bool Empty() const;
    bool Remove(Point point);

private:
    int GetChildIndex(Point point) const;
    Node* GetOrCreateChild(Point point);

    bool _leaf = true;
    Point _center = {}; // TODO: This needs to change to _min and _max
    int _halfExtent = 0;
    Point* _point = nullptr;
    Node* _children[4] = {};
};
