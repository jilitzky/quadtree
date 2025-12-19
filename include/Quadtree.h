/// Copyright (c) 2025 Jose Ilitzky

#pragma once

#include <optional>
#include <vector>
#include "AABB.h"
#include "Vector2.h"

/// A data structure that partitions a two-dimensional space into quadrants and provides efficient spatial queries.
/// @tparam T The type of data representing elements in the tree.
template<typename T>
class Quadtree
{
public:
    /// Represents an item stored in the tree.
    struct Element
    {
        /// The data representing the element.
        T data;
        
        /// The position linked to the data.
        Vector2 position;
    };

    /// Construct a Quadtree that covers the given bounds.
    /// @param bounds The area covered by the tree.
    /// @param nodeCapacity The maximum number of elements that a node within the tree can store before subdividing.
    /// @param maxDepth The maximum depth the tree can have from its root to the furthest leaf.
    Quadtree(const AABB& bounds, size_t nodeCapacity = 8, int maxDepth = 4) : mRoot(bounds, 0), mNodeCapacity(nodeCapacity), mMaxDepth(maxDepth)
    {
    }
    
    /// Copy constructor is deleted to avoid accidental copies.
    Quadtree(const Quadtree& other) = delete;
    
    /// Move constructor is defaulted to allow efficient transfer of ownership.
    Quadtree(Quadtree&& other) = default;
    
    /// Returns the bounding box representing the area covered by the tree.
    /// @return A constant reference to the bounding box.
    const AABB& GetBounds() const
    {
       return mRoot.bounds;
    }

    /// Calculates the height of the tree from its deepest branch.
    /// @return The height of the tree.
    size_t GetHeight() const
    {
        return mRoot.GetHeight();
    }
    
    /// Gets all elements contained by the tree.
    /// @return A collection with all the elements.
    std::vector<T> GetAllElements() const
    {
        std::vector<T> allElements;
        mRoot.GetAllElements(allElements);
        return allElements;
    }
    
    /// Counts the total number of elements in the tree.
    /// @return The total number of elements.
    size_t CountElements() const
    {
        return mRoot.CountElements();
    }
    
    /// Inserts a new element with the given data and position.
    /// @param data The data representing the element.
    /// @param position The position where the element is.
    /// @return True if the element was successfully inserted.
    bool Insert(T data, const Vector2& position)
    {
        if (!GetBounds().Contains(position))
        {
            return false;
        }

        return mRoot.Insert(data, position, mNodeCapacity, mMaxDepth);
    }
    
    /// Removes an element matching the given data and position.
    /// @param data The data representing the element.
    /// @param position The position where the element is.
    /// @return True if the element was successfully removed.
    bool Remove(T data, const Vector2& position)
    {
        if (!GetBounds().Contains(position))
        {
            return false;
        }

        return mRoot.Remove(data, position, mNodeCapacity);
    }
    
    /// Finds the closest element to the given target position within a maximum radius.
    /// @param target The position to search around.
    /// @param maxRadius The maximum distance from the target to consider.
    /// @return The closest element if found, or empty.
    std::optional<Element> FindNearest(const Vector2& target, float maxRadius = std::numeric_limits<float>::max()) const
    {
        std::optional<Element> nearest = std::nullopt;
        float bestDistanceSq = maxRadius * maxRadius;
        mRoot.FindNearest(target, bestDistanceSq, nearest);
        return nearest;
    }
    
    /// Gathers elements found within the given bounds.
    /// @param queryBounds The search area.
    /// @return The collection of elements found within the search area.
    std::vector<Element> SpatialQuery(const AABB& queryBounds) const
    {
        std::vector<Element> foundElements;
        if (GetBounds().Intersects(queryBounds))
        {
            mRoot.SpatialQuery(queryBounds, foundElements);
        }
        
        return foundElements;
    }
    
    /// Copy assignment is deleted to avoid accidental copies.
    Quadtree& operator=(const Quadtree&) = delete;

    /// Move assignment is defaulted to allow efficient transfer of ownership.
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    /// Represents a node in the Quadtree that may be a leaf or a branch.
    struct Node
    {
        /// Array containing the four child quadrants in Z-order: Top-Left, Top-Right, Bottom-Left, Bottom-Right.
        std::array<std::unique_ptr<Node>, 4> children;
        
        /// Defines the area covered by this node.
        AABB bounds;
        
        /// How many levels down the node is from the root.
        int depth;
        
        /// Indicates if this node is an endpoint and can store elements or if it's a branch with children.
        bool isLeaf = true;
        
        /// Elements stored by this node when it's a leaf.
        std::vector<Element> elements;
        
        /// Construct a node with the given bounds
        /// @param bounds The area covered by the node.
        /// @param depth How many levels down the node is from the root.
        Node(const AABB& bounds, int depth) : bounds(bounds), depth(depth)
        {
        }
        
        /// Calculates the height of this node from its deepest branch.
        /// @return The height of this node.
        size_t GetHeight() const
        {
            if (isLeaf)
            {
                return 1;
            }
            
            size_t height = 0;
            for (const auto& child : children)
            {
                height = std::max(child->GetHeight(), height);
            }
            
            return height + 1;
        }
        
        /// Recursively collect all elements in this node and its children.
        /// @param allElements The collection where elements are accumulated.
        void GetAllElements(std::vector<Element>& allElements) const
        {
            if (isLeaf)
            {
                allElements.insert(allElements.end(), elements.begin(), elements.end());
                return;
            }
            
            for (const auto& child : children)
            {
                child->GetAllElements(allElements);
            }
        }
        
        /// Counts the total number of elements in this node and all its children.
        /// @return The total number of elements.
        size_t CountElements() const
        {
            if (isLeaf)
            {
                return elements.size();
            }
            
            size_t size = 0;
            for (const auto& child : children)
            {
                size += child->CountElements();
            }
            
            return size;
        }
        
        /// Inserts a new element with the given data and position.
        /// @param data The data representing the element.
        /// @param position The position where the element is.
        /// @param capacity The maximum number of elements to hold before subdividing.
        /// @param maxDepth The maximum depth a node can be from the root.
        /// @return True if the element was successfully inserted.
        bool Insert(T data, const Vector2& position, size_t capacity, int maxDepth)
        {
            if (!isLeaf)
            {
                int index = GetChildIndex(position);
                return children[index]->Insert(data, position, capacity, maxDepth);
            }

            elements.push_back({data, position});

            if (elements.size() > capacity && depth < maxDepth)
            {
                Subdivide(capacity, maxDepth);
            }

            return true;
        }
        
        /// Removes an element matching the given data and position.
        /// @param data The data representing the element.
        /// @param position The position where the element is.
        /// @param capacity The maximum number of elements a node can hold.
        /// @return True if the element was successfully removed.
        bool Remove(T data, const Vector2& position, size_t capacity)
        {
            if (isLeaf)
            {
                auto it = std::find_if(elements.begin(), elements.end(), [&](const Element& element)
                {
                    return element.data == data && element.position == position;
                });
                
                if (it != elements.end())
                {
                    *it = std::move(elements.back());
                    elements.pop_back();
                    return true;
                }
                
                return false;
            }

            int index = GetChildIndex(position);
            if (children[index]->Remove(data, position, capacity))
            {
                TryMerge(capacity);
                return true;
            }

            return false;
        }
        
        /// Recursive helper for finding the nearest element.
        /// @param target The search position.
        /// @param bestDistanceSq The best squared distance found so far.
        /// @param nearest The closest element if found, or empty.
        void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element>& nearest) const
        {
            if (isLeaf)
            {
                for (const auto& element : elements)
                {
                    float distanceSq = element.position.DistanceSquared(target);
                    if (distanceSq < bestDistanceSq)
                    {
                        bestDistanceSq = distanceSq;
                        nearest = element;
                    }
                }
                return;
            }
            
            Vector2 center = bounds.GetCenter();
            int isRight = target.x > center.x;
            int isBottom = target.y < center.y;

            // Bias the search toward the quadrant that contains the target.
            std::array<int, 4> sortedIndices;
            sortedIndices[0] = isBottom * 2 + isRight;
            sortedIndices[1] = isBottom * 2 + (1 - isRight);
            sortedIndices[2] = (1 - isBottom) * 2 + isRight;
            sortedIndices[3] = (1 - isBottom) * 2 + (1 - isRight);
            
            for (int index : sortedIndices)
            {
                const auto& child = children[index];
                float distanceX = std::max({child->bounds.min.x - target.x, 0.0f, target.x - child->bounds.max.x});
                float distanceY = std::max({child->bounds.min.y - target.y, 0.0f, target.y - child->bounds.max.y});
                float distanceSq = (distanceX * distanceX) + (distanceY * distanceY);
                if (distanceSq < bestDistanceSq)
                {
                    child->FindNearest(target, bestDistanceSq, nearest);
                }
            }
        }
        
        /// Recursive helper for the spatial query.
        /// @param queryBounds The search area.
        /// @param foundElements The collection of elements found by the query.
        void SpatialQuery(const AABB& queryBounds, std::vector<Element>& foundElements) const
        {
            if (queryBounds.Contains(bounds))
            {
                GetAllElements(foundElements);
                return;
            }

            if (isLeaf)
            {
                for (const auto& element : elements)
                {
                    if (queryBounds.Contains(element.position))
                    {
                        foundElements.push_back(element);
                    }
                }
                return;
            }

            for (const auto& child : children)
            {
                if (child->bounds.Intersects(queryBounds))
                {
                    child->SpatialQuery(queryBounds, foundElements);
                }
            }
        }
        
    private:
        /// Determines the index to the children array based on where the position belongs to.
        /// @param position The position to check.
        /// @return The index to the corresponding child.
        int GetChildIndex(const Vector2& position) const
        {
            // Use a Z-order curve to map the children into a one-dimensional sequence.
            // 0: Left-Top
            // 1: Right-Top
            // 2: Left-Bottom
            // 3: Right-Bottom
            Vector2 center = bounds.GetCenter();
            return (position.x > center.x) + ((position.y < center.y) * 2);
        }
        
        /// Divides this node into a branch by passing its elements into its children.
        /// @param capacity The maximum number of elements a node can hold.
        /// @param maxDepth The maximum depth a node can be from the root.
        void Subdivide(size_t capacity, int maxDepth)
        {
            Vector2 min = bounds.min;
            Vector2 max = bounds.max;
            Vector2 center = bounds.GetCenter();

            AABB topLeft({min.x, center.y}, {center.x, max.y});
            AABB topRight(center, {max.x, max.y});
            AABB bottomLeft({min.x, min.y}, center);
            AABB bottomRight({center.x, min.y}, {max.x, center.y});

            int childDepth = depth + 1;
            children[0] = std::make_unique<Node>(topLeft, childDepth);
            children[1] = std::make_unique<Node>(topRight, childDepth);
            children[2] = std::make_unique<Node>(bottomLeft, childDepth);
            children[3] = std::make_unique<Node>(bottomRight, childDepth);

            for (auto& element : elements)
            {
                int index = GetChildIndex(element.position);
                children[index]->Insert(std::move(element.data), element.position, capacity, maxDepth);
            }

            isLeaf = false;
            elements.clear();
        }

        /// Attempts to merge the children back into this node if their elements fit within this node's capacity.
        /// @param capacity The maximum number of elements a node can hold.
        void TryMerge(size_t capacity)
        {
            for (const auto& child : children)
            {
                if (!child->isLeaf)
                {
                    return;
                }
            }

            size_t elementCount = 0;
            for (const auto& child : children)
            {
                elementCount += child->elements.size();
            }

            if (elementCount <= capacity)
            {
                elements.reserve(elementCount);
                for (auto& child : children)
                {
                    for (auto& element : child->elements)
                    {
                        elements.push_back(std::move(element));
                    }
                }

                for (auto& child : children)
                {
                    child.reset();
                }

                isLeaf = true;
            }
        }
    };
    
    /// Represents the tree's root node.
    Node mRoot;
    
    /// The maximum number of elements a node is allowed to have before attempting to subdivide.
    size_t mNodeCapacity;
    
    /// How many additional levels the tree can have (the root is at depth 0).
    int mMaxDepth;
};
