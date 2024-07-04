#pragma once
#include <forward_list>
#include "Entry.h"

class Region
{
public:
    Region(Point center, int halfExtent);
    virtual ~Region();

    void Add(int value, Point position);
    bool Empty() const;
    bool Remove(int value, Point position);

private:
    void AddToChild(int value, Point position);
    bool CanSubdivide() const;
    int GetChildIndex(Point position) const;
    bool RemoveFromChild(int value, Point position);

    bool _leaf = true;
    Point _center = {};
    int _halfExtent = 0;
    std::forward_list<Entry> _entries;
    Region* _children[4] = {};
};
