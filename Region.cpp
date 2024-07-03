#include "Region.h"

Region::Region(Point center, int halfExtent) : _center(center), _halfExtent(halfExtent)
{
}

Region::~Region()
{
    delete _entry;
    _entry = nullptr;

    for (int i = 0; i < 4; i++)
    {
        delete _children[i];
        _children[i] = nullptr;
    }
}

void Region::Add(int value, Point position)
{
    // Leaf regions can hold an entry or stop being a leaf by passing their existing entry onto a child
    if (_leaf)
    {
        if (_entry == nullptr)
        {
            _entry = new Entry{ value, position };
        }
        else
        {
            _leaf = false;
            AddToChild(_entry->Value, _entry->Position);
            _entry = nullptr;
        }
    }

    // Branch regions simply forward the request onto their children
    if (!_leaf)
    {
        AddToChild(value, position);
    }
}

void Region::AddToChild(int value, Point position)
{
    constexpr int bottomLeft = 0;
    constexpr int topLeft = 1;
    constexpr int bottomRight = 2;
    constexpr int topRight = 3;

    // Figure out which child owns the position
    int childIndex;
    if (position.X < _center.X)
    {
        childIndex = position.Y < _center.Y ? bottomLeft : topLeft;
    }
    else
    {
        childIndex = position.Y < _center.Y ? bottomRight : topRight;
    }

    // Create the child if it doesn't exist yet
    if (_children[childIndex] == nullptr)
    {
        const int quarterExtent = _halfExtent / 2;
        Point childCenter = _center;
        childCenter.X += position.X < _center.X ? -quarterExtent : quarterExtent;
        childCenter.Y += position.Y < _center.Y ? -quarterExtent : quarterExtent;
        _children[childIndex] = new Region(childCenter, quarterExtent);
    }

    _children[childIndex]->Add(value, position);
}
