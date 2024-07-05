#include "Node.h"
#include <cmath>

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

    for (const Node* child : _children)
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
    const double nearestDistance = nearest.first;
    if (point.X < _min.X - nearestDistance || point.X > _max.X + nearestDistance || point.Y < _min.Y - nearestDistance || point.Y > _max.Y + nearestDistance)
    {
        return;
    }

    if (_point != nullptr)
    {
        const double deltaX = _point->X - point.X;
        const double deltaY = _point->Y - point.Y;
        const double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distance < nearestDistance)
        {
            nearest.first = distance;
            nearest.second = _point;
        }
    }

    const Point center = Center();
    const int isRight = point.X >= center.X;
    const int isBottom = point.Y < center.Y;

    int nearestIndices[4]{};
    nearestIndices[0] = isBottom * 2 + isRight;
    nearestIndices[1] = isBottom * 2 + (1 - isRight);
    nearestIndices[2] = (1 - isBottom) * 2 + isRight;
    nearestIndices[3] = (1 - isBottom) * 2 + (1 - isRight);
    for (int index : nearestIndices)
    {
        const Node* child = _children[index];
        if (child != nullptr)
        {
            child->FindNearest(point, nearest);
        }
    }
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
    int index = 0;
    const Point center = Center();
    if (point.X >= center.X)
    {
        index += 1;
    }
    if (point.Y < center.Y)
    {
        index += 2;
    }
    return index;
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
