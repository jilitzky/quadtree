#include "Region.h"

Region::Region(Point center, int halfExtent) : _center(center), _halfExtent(halfExtent)
{
}

Region::~Region()
{
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
        if (_entries.empty() || _entries.front().Position == position || !CanSubdivide())
        {
            _entries.emplace_front(value, position);
        }
        else
        {
            // TODO: Look at doing a proper transfer of the entries, possibly via a move
            for (const auto& entry : _entries)
            {
                AddToChild(entry.Value, entry.Position);
            }
            _entries.clear();
            _leaf = false;
        }
    }

    if (!_leaf)
    {
        AddToChild(value, position);
    }
}

bool Region::Empty() const
{
    if (_leaf && _entries.empty())
    {
        return true;
    }

    for (const auto* child : _children)
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
        const size_t removed = _entries.remove_if(
            [value](const Entry& entry) { return entry.Value == value; });

        if (removed > 0)
        {
            _leaf = !_entries.empty();
            return true;
        }
        return false;
    }
    return RemoveFromChild(value, position);
}

void Region::AddToChild(int value, Point position)
{
    const int index = GetChildIndex(position);

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

bool Region::CanSubdivide() const
{
    return _halfExtent / 2 > 0;
}

int Region::GetChildIndex(Point position) const
{
    constexpr int topLeft = 0;
    constexpr int topRight = 1;
    constexpr int bottomLeft = 2;
    constexpr int bottomRight = 3;

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
        if (removed && child->Empty())
        {
            delete child;
            _children[index] = nullptr;
        }
        return removed;
    }
    return false;
}
