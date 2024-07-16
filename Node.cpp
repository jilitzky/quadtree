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
    if (ChildCount() == 0)
    {
        // Set the point at this level if there isn't one yet
        if (!_point.has_value())
        {
            _point = point;
            return true;
        }

        // Fail the add request if the point already exists or if we can't subdivide further
        const Point& existingPoint = *_point;
        auto canSubdivide = [this] { return Width() / 2 > 0 || Height() / 2 > 0; };
        if (existingPoint == point || !canSubdivide())
        {
            return false;
        }

        // If the new point is different from what we have, transfer the existing point to a child
        Node* child = GetOrCreateChild(existingPoint);
        const bool added = child->Add(existingPoint);
        if (added)
        {
            RefreshDepth();
        }
        _point.reset();
    }

    Node* child = GetOrCreateChild(point);
    const bool added = child->Add(point);
    if (added)
    {
        RefreshDepth();
    }
    return added;
}

void Node::FindNearest(const Point& point, NearestPoint& nearest) const
{
    // Do an early out if the point is farther away on each axis than the nearest distance we've already found
    if (point.x < _min.x - nearest.distance || point.x > _max.x + nearest.distance || point.y < _min.y - nearest.distance || point.y > _max.y + nearest.distance)
    {
        return;
    }

    // If the node has a point, consider it as a candidate
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
    // If the node has children, explore them sorted by their proximity to the query point
    else if (ChildCount() > 0)
    {
        const Point center = Center();
        const int isRight = point.x >= center.x;
        const int isBottom = point.y < center.y;

        std::array<int, 4> sortedIndices{};
        sortedIndices[0] = isBottom * 2 + isRight;
        sortedIndices[1] = isBottom * 2 + (1 - isRight);
        sortedIndices[2] = (1 - isBottom) * 2 + isRight;
        sortedIndices[3] = (1 - isBottom) * 2 + (1 - isRight);
        for (int index : sortedIndices)
        {
            const Node* child = _children[index];
            if (child != nullptr)
            {
                child->FindNearest(point, nearest);
            }
        }
    }
}

bool Node::Remove(const Point& point)
{
    // Remove the point at this level if it matches the given value
    if (_point.has_value() && *_point == point)
    {
        _point.reset();
        return true;
    }

    // Attempt to remove the point from the child matching the coordinates
    const int index = ChildIndex(point);
    Node* child = _children[index];
    if (child != nullptr)
    {
        const bool removed = child->Remove(point);
        if (removed)
        {
            // Delete a child if it's completely empty after a removal
            auto childIsEmpty = [child] { return !child->_point.has_value() && child->ChildCount() == 0; };
            if (childIsEmpty())
            {
                delete child;
                _children[index] = nullptr;
            }

            // Attempt to merge the last remaining child with the parent
            if (ChildCount() == 1)
            {
                for (int i = 0; i < _children.size(); i++)
                {
                    const Node* remainingChild = _children[i];
                    if (remainingChild != nullptr && remainingChild->_point.has_value())
                    {
                        _point = remainingChild->_point;
                        delete remainingChild;
                        _children[i] = nullptr;
                    }
                }
            }

            RefreshDepth();
        }
        return removed;
    }
    return false;
}

int Node::ChildCount() const
{
    int count = 0;
    for (const Node* child : _children)
    {
        if (child != nullptr)
        {
            count++;
        }
    }
    return count;
}

int Node::ChildIndex(const Point& point) const
{
    // Child indices follow a Z-order curve
    // 0 = Top-Left
    // 1 = Top-Right
    // 2 = Bottom-Left
    // 3 = Bottom-Right
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
    const int index = ChildIndex(point);
    if (_children[index] == nullptr)
    {
        const Point center = Center();

        Point childMin = _min;
        Point childMax = _max;

        // Adjust child's position horizontally
        const int halfWidth = Width() / 2;
        if (point.x < center.x)
        {
            childMax.x -= halfWidth;
        }
        else
        {
            childMin.x += halfWidth;
        }

        // Adjust child's position vertically
        const int halfHeight = Height() / 2;
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

void Node::RefreshDepth()
{
    size_t maxChildDepth = 0;
    for (const Node* child : _children)
    {
        if (child != nullptr)
        {
            maxChildDepth = std::max(child->Depth(), maxChildDepth);
        }
    }
    _depth = maxChildDepth + 1;
}
