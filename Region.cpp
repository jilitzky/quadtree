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
        auto canSubdivide = [this] { return _halfExtent / 2 > 0; };
        if (_entries.empty() || _entries.front().Position == position || !canSubdivide())
        {
            _entries.emplace_front(value, position);
        }
        else
        {
            Region* child = GetOrCreateChild(_entries.front().Position);
            child->_entries.splice_after(child->_entries.cbefore_begin(), _entries);
            _leaf = false;
        }
    }

    if (!_leaf)
    {
        Region* child = GetOrCreateChild(position);
        child->Add(value, position);
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

Region* Region::GetOrCreateChild(Point position)
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
    return _children[index];
}
