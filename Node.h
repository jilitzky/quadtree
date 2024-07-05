#pragma once
#include <forward_list>
#include "Entry.h"

class Node
{
public:
    Node(Point center, int halfExtent);
    virtual ~Node();

    void Add(int value, Point position);
    bool Empty() const;
    bool Remove(int value, Point position);

private:
    int GetChildIndex(Point position) const;
    Node* GetOrCreateChild(Point position);

    bool _leaf = true;
    Point _center = {};
    int _halfExtent = 0;
    std::forward_list<Entry> _entries;
    Node* _children[4] = {};
};
