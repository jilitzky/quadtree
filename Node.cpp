#include "Node.h"

Node::Node(Point center, int halfExtent) : _center(center), _halfExtent(halfExtent)
{
}

Node::~Node()
{
    delete _point;
    _point = nullptr;

    for (int i = 0; i < 4; i++)
    {
        delete _children[i];
        _children[i] = nullptr;
    }
}

bool Node::Add(Point point)
{
    if (_leaf)
    {
        if (_point == nullptr)
        {
            _point = new Point(point);
            return true;
        }

        auto canSubdivide = [this] { return _halfExtent / 2 > 0; };
        if (*_point == point || !canSubdivide())
        {
            return false;
        }

        Node* child = GetOrCreateChild(*_point);
        child->_point = _point;
        _point = nullptr;
        _leaf = false;
    }

    if (!_leaf)
    {
        Node* child = GetOrCreateChild(point);
        return child->Add(point);
    }
    return false;
}

bool Node::Empty() const
{
    if (_leaf && _point == nullptr)
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

bool Node::Remove(Point point)
{
    if (_leaf)
    {
        if (_point != nullptr && *_point == point)
        {
            delete _point;
            _point = nullptr;
            _leaf = false;
            return true;
        }
        return false;
    }

    const int index = GetChildIndex(point);
    Node* child = _children[index];
    if (child != nullptr)
    {
        const bool removed = child->Remove(point);
        if (removed && child->Empty())
        {
            delete child;
            _children[index] = nullptr;
        }
        return removed;
    }
    return false;
}

int Node::GetChildIndex(Point point) const
{
    constexpr int topLeft = 0;
    constexpr int topRight = 1;
    constexpr int bottomLeft = 2;
    constexpr int bottomRight = 3;

    if (point.X < _center.X)
    {
        return point.Y < _center.Y ? bottomLeft : topLeft;
    }
    return point.Y < _center.Y ? bottomRight : topRight;
}

Node* Node::GetOrCreateChild(Point point)
{
    const int index = GetChildIndex(point);
    if (_children[index] == nullptr)
    {
        Point childCenter = _center;
        const int quarterExtent = _halfExtent / 2;
        childCenter.X += point.X < _center.X ? -quarterExtent : quarterExtent;
        childCenter.Y += point.Y < _center.Y ? -quarterExtent : quarterExtent;
        _children[index] = new Node(childCenter, quarterExtent);
    }
    return _children[index];
}
