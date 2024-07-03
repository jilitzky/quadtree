#pragma once
#include "Entry.h"

class Region
{
public:
    Region(Point center, int halfExtent);
    virtual ~Region();

    void Add(int value, Point position);

private:
    void AddToChild(int value, Point position);

    bool _leaf = true;
    Point _center = {};
    int _halfExtent = 0;
    Entry* _entry = nullptr;
    Region* _children[4] = {};
};
