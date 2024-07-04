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
    int GetChildIndex(Point position) const;
    Region* GetOrCreateChild(Point position);

    bool _leaf = true;
    Point _center = {};
    int _halfExtent = 0;
    std::forward_list<Entry> _entries;
    Region* _children[4] = {};
};
