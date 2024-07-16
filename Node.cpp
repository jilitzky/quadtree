#include "Node.h"
#include <cmath>

Node::~Node()
{
    for (int i = 0; i < _children.size(); i++)
    {
        delete _children[i];
        _children[i] = nullptr;
    }
}

bool Node::Add(const Point& point)
{
    if (_leaf)
    {
        if (!_point.has_value())
        {
            _point = point;
            return true;
        }

        auto canSubdivide = [this] { return Width() / 2 > 0 || Height() / 2 > 0; };
        if (*_point == point || !canSubdivide())
        {
            return false;
        }

        Node* child = GetOrCreateChild(*_point);
        child->_point = _point;
        _point.reset();
        _leaf = false;
    }

    if (!_leaf)
    {
        Node* child = GetOrCreateChild(point);
        return child->Add(point);
    }
    return false;
}

void Node::FindNearest(const Point& point, NearestPoint& nearest) const
{
    if (point.x < _min.x - nearest.distance || point.x > _max.x + nearest.distance || point.y < _min.y - nearest.distance || point.y > _max.y + nearest.distance)
    {
        return;
    }

    if (_point.has_value())
    {
        const double deltaX = _point->x - point.x;
        const double deltaY = _point->y - point.y;
        const double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distance < nearest.distance)
        {
            nearest.distance = distance;
            nearest.point = _point;
        }
    }

    const Point center = Center();
    const int isRight = point.x >= center.x;
    const int isBottom = point.y < center.y;

    std::array<int, 4> nearestIndices{};
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
        if (_point.has_value() && *_point == point)
        {
            _point.reset();
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
        if (removed && child->IsEmpty())
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
    if (point.x >= center.x)
    {
        index += 1;
    }
    if (point.y < center.y)
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

        if (point.x < center.x)
        {
            childMax.x -= halfWidth;
        }
        else
        {
            childMin.x += halfWidth;
        }

        if (point.y < center.y)
        {
            childMax.y -= halfHeight;
        }
        else
        {
            childMin.y += halfHeight;
        }

        _children[index] = new Node(childMin, childMax);
    }
    return _children[index];
}

bool Node::IsEmpty() const
{
    if (_leaf && !_point.has_value())
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
