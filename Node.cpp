#include "Node.h"

Node::Node(const Point& min, const Point& max) : _min(min), _max(max)
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

bool Node::Add(const Point& point)
{
    if (_leaf)
    {
        if (_point == nullptr)
        {
            _point = new Point(point);
            return true;
        }

        auto canSubdivide = [this] { return GetExtentX() / 2 > 0 || GetExtentY() / 2 > 0; };
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

bool Node::Contains(const Point& point) const
{
    return point.X >= _min.X && point.X <= _max.X && point.Y >= _min.Y && point.Y <= _max.Y;
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

bool Node::Remove(const Point& point)
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

int Node::GetChildIndex(const Point& point) const
{
    constexpr int topLeft = 0;
    constexpr int topRight = 1;
    constexpr int bottomLeft = 2;
    constexpr int bottomRight = 3;

    const Point center = GetCenter();
    if (point.X < center.X)
    {
        return point.Y < center.Y ? bottomLeft : topLeft;
    }
    return point.Y < center.Y ? bottomRight : topRight;
}

Node* Node::GetOrCreateChild(const Point& point)
{
    const int index = GetChildIndex(point);
    if (_children[index] == nullptr)
    {
        Point childMin = _min;
        Point childMax = _max;

        const Point center = GetCenter();
        const int halfExtentX = GetExtentX() / 2;
        const int halfExtentY = GetExtentY() / 2;

        if (point.X < center.X)
        {
            childMax.X -= halfExtentX;
        }
        else
        {
            childMin.X += halfExtentX;
        }

        if (point.Y < center.Y)
        {
            childMax.Y -= halfExtentY;
        }
        else
        {
            childMin.Y += halfExtentY;
        }

        _children[index] = new Node(childMin, childMax);
    }
    return _children[index];
}
