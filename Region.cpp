#include "Region.h"

Region::Region(Point center, int halfExtent) : _center(center), _halfExtent(halfExtent)
{
}

Region::~Region()
{
    Entry* current = _entry;
    while (current != nullptr)
    {
        Entry* next = current->Next;
        delete current;
        current = next;
    }
    _entry = nullptr;

    for (int i = 0; i < 4; i++)
    {
        delete _children[i];
        _children[i] = nullptr;
    }
}

void Region::Add(int value, Point position)
{
    if (_leaf)
    {
        // Hold the entry if the leaf doesn't have one yet
        if (_entry == nullptr)
        {
            _entry = new Entry{ value, position };
        }
        // Chain the entry if it's in the same position or if we can't subdivide further
        else if (position == _entry->Position || _halfExtent / 2 == 0)
        {
            Entry* tail = _entry;
            while (tail->Next != nullptr)
            {
                tail = tail->Next;
            }
            tail->Next = new Entry{ value, position };
        }
        // Stop being a leaf by passing the chain of entries onto a child
        else
        {
            _leaf = false;
            AddToChild(_entry->Value, _entry->Position);
            _entry = nullptr;
        }
    }

    // Non-leaf regions simply forward the request onto their children
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
        Point childCenter = _center;
        const int quarterExtent = _halfExtent / 2;
        childCenter.X += position.X < _center.X ? -quarterExtent : quarterExtent;
        childCenter.Y += position.Y < _center.Y ? -quarterExtent : quarterExtent;
        _children[childIndex] = new Region(childCenter, quarterExtent);
    }

    _children[childIndex]->Add(value, position);
}
