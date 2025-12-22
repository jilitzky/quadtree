/// Copyright (c) 2025 Jose Ilitzky

#pragma once

#include <optional>
#include <vector>
#include "AABB.h"
#include "QuadtreeElement.h"
#include "QuadtreeNode.h"
#include "Vector2.h"

/// A data structure that partitions a two-dimensional space into quadrants and provides efficient spatial queries.
/// @tparam T The type of data representing elements in the tree.
template<typename T>
class Quadtree
{
public:
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
    std::optional<QuadtreeElement<T>> FindNearest(const Vector2& target, float maxRadius = std::numeric_limits<float>::max()) const
    {
        std::optional<QuadtreeElement<T>> nearest = std::nullopt;
        float bestDistanceSq = maxRadius * maxRadius;
        mRoot.FindNearest(target, bestDistanceSq, nearest);
        return nearest;
    }
    
    /// Gathers elements found within the given region.
    /// @param region The search area.
    /// @return The collection of elements found within the region.
    std::vector<QuadtreeElement<T>> FindAll(const AABB& region) const
    {
        std::vector<QuadtreeElement<T>> foundElements;
        if (GetBounds().Intersects(region))
        {
            mRoot.FindAll(region, foundElements);
        }
        
        return foundElements;
    }
    
    /// Copy assignment is deleted to avoid accidental copies.
    Quadtree& operator=(const Quadtree&) = delete;

    /// Move assignment is defaulted to allow efficient transfer of ownership.
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    /// Represents the tree's root node.
    QuadtreeNode<T> mRoot;
    
    /// The maximum number of elements a node is allowed to have before attempting to subdivide.
    size_t mNodeCapacity;
    
    /// How many additional levels the tree can have (the root is at depth 0).
    int mMaxDepth;
};
