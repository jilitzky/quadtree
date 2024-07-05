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

        auto canSubdivide = [this] { return Width() / 2 > 0 || Height() / 2 > 0; };
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

void Node::FindNearest(const Point& point, std::pair<double, const Point*>& nearest) const
{
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

    const Point center = Center();
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

        const Point center = Center();
        const int halfWidth = Width() / 2;
        const int halfHeight = Height() / 2;

        if (point.X < center.X)
        {
            childMax.X -= halfWidth;
        }
        else
        {
            childMin.X += halfWidth;
        }

        if (point.Y < center.Y)
        {
            childMax.Y -= halfHeight;
        }
        else
        {
            childMin.Y += halfHeight;
        }

        _children[index] = new Node(childMin, childMax);
    }
    return _children[index];
}
