/// Copyright (c) 2025 Jose Ilitzky

#pragma once

#include <array>
#include <optional>
#include <type_traits>
#include <vector>

/// Represents an item stored in the tree.
/// @tparam T The type of data representing the element.
/// @tparam Vec2 The type of 2D vector to use.
template<typename T, typename Vec2>
struct QuadtreeElement
{
    /// The data representing the element.
    T data;
    
    /// The position linked to the data.
    Vec2 position;
};

namespace QuadtreeDetail
{
    /// Used to consider all possible elements during searches.
    struct NoFilter
    {
        /// Returns true to optimize queries that don't require filtering.
        /// @tparam E The type of quadtree element.
        template<typename E>
        constexpr bool operator()(const E&) const
        {
            return true;
        }
    };

    /// An Axis-Aligned Bounding Box (AABB) defined by its minimum and maximum points.
    /// @tparam Vec2 The type of 2D vector to use.
    template<typename Vec2>
    struct Bounds
    {
        /// The bottom-left corner of the bounding box.
        Vec2 min;
        /// The top-right corner of the bounding box.
        Vec2 max;
        
        /// Constructor that initializes the bounding box to the given minimum and maximum points.
        /// @param min The minimum point (bottom-left corner).
        /// @param max The maximum point (top-right corner).
        Bounds(const Vec2& min, const Vec2& max) : min(min), max(max) {}
        
        /// Calculates the width of the bounding box.
        /// @return The measured width.
        float GetWidth() const
        {
            return max.x - min.x;
        }
        
        /// Calculates the height of the bounding box.
        /// @return The measured height.
        float GetHeight() const
        {
            return max.y - min.y;
        }
        
        /// Calculates the center point of the bounding box.
        /// @return The position representing the center point.
        Vec2 GetCenter() const
        {
            return (min + max) * 0.5f;
        }
        
        /// Returns true if this bounding box completely contains the other box.
        /// @param other The other box to check.
        /// @return True if the other box is entirely within this box, false otherwise.
        bool Contains(const Bounds& other) const
        {
            return other.min.x >= min.x && other.max.x <= max.x && other.min.y >= min.y && other.max.y <= max.y;
        }
        
        /// Returns true if this bounding box contains the given position.
        /// @param position The position to check.
        /// @return True if the position is inside or on the boundary of this box, false otherwise.
        bool Contains(const Vec2& position) const
        {
            return position.x >= min.x && position.y >= min.y && position.x <= max.x && position.y <= max.y;
        }
        
        /// Returns true if this bounding box overlaps the other box.
        /// @param other The other box to check against.
        /// @return True if the two boxes overlap, false otherwise.
        bool Intersects(const Bounds& other) const
        {
            if (max.x < other.min.x || min.x > other.max.x)
            {
                return false;
            }
            
            if (max.y < other.min.y || min.y > other.max.y)
            {
                return false;
            }
            
            return true;
        }
    };

    /// Represents a node in the Quadtree that may be a leaf or a branch.
    /// @tparam T The type of data representing elements in the node.
    /// @tparam Vec2 The type of 2D vector to use.
    template<typename T, typename Vec2>
    struct Node
    {
        using Element = QuadtreeElement<T, Vec2>;
        using Bounds = Bounds<Vec2>;
        
        /// Array containing the four child quadrants in Z-order: Top-Left, Top-Right, Bottom-Left, Bottom-Right.
        std::array<std::unique_ptr<Node>, 4> children;
        
        /// Defines the area covered by this node.
        Bounds bounds;
        
        /// How many levels down the node is from the root.
        int depth;
        
        /// Indicates if this node is an endpoint and can store elements or if it's a branch with children.
        bool isLeaf = true;
        
        /// Elements stored by this node when it's a leaf.
        std::vector<Element> elements;
        
        /// Construct a node with the given bounds
        /// @param bounds The area covered by the node.
        /// @param depth How many levels down the node is from the root.
        Node(const Bounds& bounds, int depth) : bounds(bounds), depth(depth)
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
        bool Insert(T data, const Vec2& position, size_t capacity, int maxDepth)
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
        bool Remove(T data, const Vec2& position, size_t capacity)
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
        /// @tparam Filter A function that takes in an element and returns true if it qualifies for the search.
        /// @param target The search position.
        /// @param filter The filter to pass for an element to qualify.
        /// @param bestDistanceSq The best squared distance found so far.
        /// @param nearest The closest element if found, or empty.
        template<typename Filter>
        void FindNearest(const Vec2& target, Filter filter, float& bestDistanceSq, std::optional<Element>& nearest) const
        {
            if (isLeaf)
            {
                for (const auto& element : elements)
                {
                    float distanceX = target.x - element.position.x;
                    float distanceY = target.y - element.position.y;
                    float distanceSq = (distanceX * distanceX) + (distanceY * distanceY);
                    if (distanceSq < bestDistanceSq && filter(element))
                    {
                        bestDistanceSq = distanceSq;
                        nearest = element;
                    }
                }
                return;
            }
            
            Vec2 center = bounds.GetCenter();
            int isRight = target.x >= center.x;
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
                    child->FindNearest(target, filter, bestDistanceSq, nearest);
                }
            }
        }
        
        /// Recursive helper for finding all elements within a search area.
        /// @tparam Filter A function that takes in an element and returns true if it qualifies for the search.
        /// @param searchArea The area to search within.
        /// @param filter The filter to pass for an element to qualify.
        /// @param foundElements The collection of elements found by the search.
        template<typename Filter>
        void FindAll(const Bounds& searchArea, Filter filter, std::vector<Element>& foundElements) const
        {
            if (searchArea.Contains(bounds))
            {
                GetAllElements(filter, foundElements);
                return;
            }
            
            if (isLeaf)
            {
                for (const auto& element : elements)
                {
                    if (searchArea.Contains(element.position) && filter(element))
                    {
                        foundElements.push_back(element);
                    }
                }
                return;
            }
            
            for (const auto& child : children)
            {
                if (child->bounds.Intersects(searchArea))
                {
                    child->FindAll(searchArea, filter, foundElements);
                }
            }
        }
        
    private:
        /// Determines the index to the children array based on where the position belongs to.
        /// @param position The position to check.
        /// @return The index to the corresponding child.
        int GetChildIndex(const Vec2& position) const
        {
            // Use a Z-order curve to map the children into a one-dimensional sequence.
            // 0: Left-Top
            // 1: Right-Top
            // 2: Left-Bottom
            // 3: Right-Bottom
            Vec2 center = bounds.GetCenter();
            return (position.x >= center.x) + ((position.y < center.y) * 2);
        }
        
        /// Recursively collect all elements in this node and its children.
        /// @tparam Filter A function that takes in an element and returns true if it qualifies for the search.
        /// @param filter The filter to pass for an element to qualify.
        /// @param allElements The collection where elements are accumulated.
        template<typename Filter>
        void GetAllElements(Filter filter, std::vector<Element>& allElements) const
        {
            if (isLeaf)
            {
                if constexpr (std::is_same_v<Filter, NoFilter>)
                {
                    allElements.insert(allElements.end(), elements.begin(), elements.end());
                }
                else
                {
                    for (const auto& element : elements)
                    {
                        if (filter(element))
                        {
                            allElements.push_back(element);
                        }
                    }
                }
                return;
            }
            
            for (const auto& child : children)
            {
                child->GetAllElements(filter, allElements);
            }
        }
        
        /// Divides this node into a branch by passing its elements into its children.
        /// @param capacity The maximum number of elements a node can hold.
        /// @param maxDepth The maximum depth a node can be from the root.
        void Subdivide(size_t capacity, int maxDepth)
        {
            Vec2 min = bounds.min;
            Vec2 max = bounds.max;
            Vec2 center = bounds.GetCenter();
            
            Bounds topLeft({min.x, center.y}, {center.x, max.y});
            Bounds topRight(center, {max.x, max.y});
            Bounds bottomLeft({min.x, min.y}, center);
            Bounds bottomRight({center.x, min.y}, {max.x, center.y});
            
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
}

/// A data structure that partitions a two-dimensional space into quadrants and provides efficient spatial queries.
/// @tparam T The type of data representing elements in the tree.
/// @tparam Vec2 The type of 2D vector to use.
template<typename T, typename Vec2>
class Quadtree
{
public:
    using Element = QuadtreeElement<T, Vec2>;
    
    /// Construct a Quadtree that covers the given bounds.
    /// @param min The minimum point describing the area covered by the tree.
    /// @param max The maximum point describing the area covered by the tree.
    /// @param nodeCapacity The maximum number of elements that a node within the tree can store before subdividing.
    /// @param maxDepth The maximum depth the tree can have from its root to the furthest leaf.
    Quadtree(const Vec2& min, const Vec2& max, size_t nodeCapacity = 8, int maxDepth = 4) : mRoot({min, max}, 0), mNodeCapacity(nodeCapacity), mMaxDepth(maxDepth)
    {
    }
    
    /// Copy constructor is deleted to avoid accidental copies.
    Quadtree(const Quadtree& other) = delete;
    
    /// Move constructor is defaulted to allow efficient transfer of ownership.
    Quadtree(Quadtree&& other) = default;
    
    /// Calculates the height of the tree from its deepest branch.
    /// @return The height of the tree.
    size_t GetHeight() const
    {
        return mRoot.GetHeight();
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
    bool Insert(T data, const Vec2& position)
    {
        if (!mRoot.bounds.Contains(position))
        {
            return false;
        }
        
        return mRoot.Insert(data, position, mNodeCapacity, mMaxDepth);
    }
    
    /// Removes an element matching the given data and position.
    /// @param data The data representing the element.
    /// @param position The position where the element is.
    /// @return True if the element was successfully removed.
    bool Remove(T data, const Vec2& position)
    {
        if (!mRoot.bounds.Contains(position))
        {
            return false;
        }
        
        return mRoot.Remove(data, position, mNodeCapacity);
    }
    
    /// Finds the closest element to the target position that passes a filter.
    /// @tparam Filter A function that takes in an element and returns true if it qualifies for the search.
    /// @param target The position to search around.
    /// @param filter The filter to pass for an element to qualify.
    /// @param maxRadius The maximum distance from the target to consider.
    /// @return The closest element if found, or empty.
    template<typename Filter>
    std::optional<Element> FindNearest(const Vec2& target, Filter filter, float maxRadius = std::numeric_limits<float>::max()) const
    {
        std::optional<Element> nearest = std::nullopt;
        float bestDistanceSq = maxRadius * maxRadius;
        mRoot.FindNearest(target, filter, bestDistanceSq, nearest);
        return nearest;
    }
    
    /// Finds the closest element to the target position.
    /// @param target The position to search around.
    /// @param maxRadius The maximum distance from the target to consider.
    /// @return The closest element if found, or empty.
    std::optional<Element> FindNearest(const Vec2& target, float maxRadius = std::numeric_limits<float>::max()) const
    {
        return FindNearest(target, QuadtreeDetail::NoFilter{}, maxRadius);
    }
    
    /// Finds elements within the region that pass a filter.
    /// @tparam Filter A function that takes in an element and returns true if it qualifies for the search.
    /// @param min The minimum point describing the search area.
    /// @param max The maximum point describing the search area.
    /// @param filter The filter to pass for an element to qualify.
    /// @return The collection of elements found within the region.
    template<typename Filter>
    std::vector<Element> FindAll(const Vec2& min, const Vec2& max, Filter filter) const
    {
        std::vector<Element> foundElements;
        
        QuadtreeDetail::Bounds searchArea(min, max);
        if (mRoot.bounds.Intersects(searchArea))
        {
            mRoot.FindAll(searchArea, filter, foundElements);
        }
        
        return foundElements;
    }
    
    /// Finds elements within the search area.
    /// @param min The minimum point describing the search area.
    /// @param max The maximum point describing the search area.
    /// @return The collection of elements found within the region.
    std::vector<Element> FindAll(const Vec2& min, const Vec2& max) const
    {
        return FindAll(min, max, QuadtreeDetail::NoFilter{});
    }
    
    /// Copy assignment is deleted to avoid accidental copies.
    Quadtree& operator=(const Quadtree&) = delete;
    
    /// Move assignment is defaulted to allow efficient transfer of ownership.
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    using Node = QuadtreeDetail::Node<T, Vec2>;
    
    /// Represents the tree's root node.
    Node mRoot;
    
    /// The maximum number of elements a node is allowed to have before attempting to subdivide.
    size_t mNodeCapacity;
    
    /// How many additional levels the tree can have (the root is at depth 0).
    int mMaxDepth;
};
