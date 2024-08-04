#include "Node.h"
#include <cmath>

Node::Node(const Vector2& min, const Vector2& max)
{
    _bounds = { min, max };
}

bool Node::Add(const Vector2& point)
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
        const Vector2& existingPoint = *_point;
        auto canSubdivide = [this] { return _bounds.Width() / 2 > 0 || _bounds.Height() / 2 > 0; };
        if (existingPoint == point || !canSubdivide())
        {
            return false;
        }

        // If the new point is different from what we have, transfer the existing point to a child
        std::unique_ptr<Node>& child = GetOrCreateChild(existingPoint);
        const bool added = child->Add(existingPoint);
        if (added)
        {
            RefreshDepth();
        }
        _point.reset();
    }

    std::unique_ptr<Node>& child = GetOrCreateChild(point);
    const bool added = child->Add(point);
    if (added)
    {
        RefreshDepth();
    }
    return added;
}

void Node::FindNearest(const Vector2& point, NearestPoint& nearest) const
{
    // Do an early out if the point is farther away on each axis than the nearest distance we've already found
    const Vector2& min = _bounds.min;
    const Vector2& max = _bounds.max;
    if (point.x < min.x - nearest.distance || point.x > max.x + nearest.distance || point.y < min.y - nearest.distance || point.y > max.y + nearest.distance)
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
        const Vector2 center = _bounds.Center();
        const int isRight = point.x >= center.x;
        const int isBottom = point.y < center.y;

        std::array<int, 4> sortedIndices{};
        sortedIndices[0] = isBottom * 2 + isRight;
        sortedIndices[1] = isBottom * 2 + (1 - isRight);
        sortedIndices[2] = (1 - isBottom) * 2 + isRight;
        sortedIndices[3] = (1 - isBottom) * 2 + (1 - isRight);
        for (int index : sortedIndices)
        {
            const std::unique_ptr<Node>& child = _children[index];
            if (child != nullptr)
            {
                child->FindNearest(point, nearest);
            }
        }
    }
}

bool Node::Remove(const Vector2& point)
{
    // Remove the point at this level if it matches the given value
    if (_point.has_value() && *_point == point)
    {
        _point.reset();
        return true;
    }

    // Attempt to remove the point from the child matching the coordinates
    const int index = ChildIndex(point);
    const std::unique_ptr<Node>& child = _children[index];
    if (child != nullptr)
    {
        const bool removed = child->Remove(point);
        if (removed)
        {
            // Invalidate a child if it's completely empty after a removal
            auto nodeIsEmpty = [](const std::unique_ptr<Node>& node) { return !node->_point.has_value() && node->ChildCount() == 0; };
            if (nodeIsEmpty(child))
            {
                _children[index] = nullptr;
            }

            // Attempt to merge the last remaining child with the parent
            if (ChildCount() == 1)
            {
                for (int i = 0; i < _children.size(); i++)
                {
                    const std::unique_ptr<Node>& remainingChild = _children[i];
                    if (remainingChild != nullptr && remainingChild->ChildCount() == 0)
                    {
                        _point = remainingChild->_point;
                        _children[i] = nullptr;
                        break;
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
    for (auto&& child : _children)
    {
        if (child != nullptr)
        {
            count++;
        }
    }
    return count;
}

int Node::ChildIndex(const Vector2& point) const
{
    // Child indices follow a Z-order curve
    // 0 = Top-Left
    // 1 = Top-Right
    // 2 = Bottom-Left
    // 3 = Bottom-Right
    int index = 0;
    const Vector2 center = _bounds.Center();
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

std::unique_ptr<Node>& Node::GetOrCreateChild(const Vector2& point)
{
    const int index = ChildIndex(point);
    if (_children[index] == nullptr)
    {
        const Vector2 center = _bounds.Center();

        Vector2 childMin = _bounds.min;
        Vector2 childMax = _bounds.max;

        // Adjust child's position horizontally
        const int halfWidth = _bounds.Width() / 2;
        if (point.x < center.x)
        {
            childMax.x -= halfWidth;
        }
        else
        {
            childMin.x += halfWidth;
        }

        // Adjust child's position vertically
        const int halfHeight = _bounds.Height() / 2;
        if (point.y < center.y)
        {
            childMax.y -= halfHeight;
        }
        else
        {
            childMin.y += halfHeight;
        }

        _children[index] = std::make_unique<Node>(childMin, childMax);
    }
    return _children[index];
}

void Node::RefreshDepth()
{
    size_t maxChildDepth = 0;
    for (auto&& child : _children)
    {
        if (child != nullptr)
        {
            maxChildDepth = std::max(child->Depth(), maxChildDepth);
        }
    }
    _depth = maxChildDepth + 1;
}
