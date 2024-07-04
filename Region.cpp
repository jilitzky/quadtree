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

bool Region::IsEmpty() const
{
    if (_entry != nullptr)
    {
        return false;
    }

    for (Region* child : _children)
    {
        if (child != nullptr)
        {
            return false;
        }
    }

    return true;
}

bool Region::Remove(int value, Point position)
{
    if (_leaf)
    {
        // Move the head of the chain to the next entry
        if (_entry->Value == value)
        {
            Entry* previousHead = _entry;
            _entry = _entry->Next;
            delete previousHead;
            _leaf = _entry != nullptr;
            return true;
        }
        // Search for the entry in the rest of the chain and unlink it
        else
        {
            Entry* current = _entry;
            while (current != nullptr)
            {
                Entry* next = current->Next;
                if (next->Value == value)
                {
                    current->Next = next->Next;
                    delete next;
                    return true;
                }
                current = next;
            }
            return false;
        }
    }
    return RemoveFromChild(value, position);
}

void Region::AddToChild(int value, Point position)
{
    const int index = GetChildIndex(position);

    // Create the child if it doesn't exist yet
    if (_children[index] == nullptr)
    {
        Point childCenter = _center;
        const int quarterExtent = _halfExtent / 2;
        childCenter.X += position.X < _center.X ? -quarterExtent : quarterExtent;
        childCenter.Y += position.Y < _center.Y ? -quarterExtent : quarterExtent;
        _children[index] = new Region(childCenter, quarterExtent);
    }

    _children[index]->Add(value, position);
}

int Region::GetChildIndex(Point position) const
{
    constexpr int bottomLeft = 0;
    constexpr int topLeft = 1;
    constexpr int bottomRight = 2;
    constexpr int topRight = 3;

    if (position.X < _center.X)
    {
        return position.Y < _center.Y ? bottomLeft : topLeft;
    }
    return position.Y < _center.Y ? bottomRight : topRight;
}

bool Region::RemoveFromChild(int value, Point position)
{
    const int index = GetChildIndex(position);
    Region* child = _children[index];
    if (child != nullptr)
    {
        const bool removed = child->Remove(value, position);
        if (removed && child->IsEmpty())
        {
            delete child;
            _children[index] = nullptr;
        }
        return removed;
    }
    return false;
}
