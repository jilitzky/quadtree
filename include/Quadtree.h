#pragma once

#include <optional>
#include <vector>
#include "AABB.h"
#include "Vector2.h"

/// A data structure that partitions a two-dimensional space into quadrants and provides efficient spatial queries.
/// @tparam T The type of data representing elements in the tree.
/// @tparam NodeCapacity The maximum number of elements that a node can store before subdividing.
template<typename T, size_t NodeCapacity = 8>
class Quadtree
{
public:
    /// Represents an item stored in the tree.
    struct Element
    {
        /// The data payload for this element.
        T data;
        
        /// The position linked to the data.
        Vector2 position;
    };

    /// Constructor that initializes the Quadtree node to the given bounding box.
    /// @param bounds The bounding box that defines the area covered by this node.
    Quadtree(const AABB& bounds) : mBounds(bounds)
    {
    }
    
    /// Copy constructor is deleted to avoid accidental copies.
    Quadtree(const Quadtree& other) = delete;
    
    /// Move constructor is defaulted to allow efficient transfer of ownership.
    Quadtree(Quadtree&& other) = default;
    
    /// Returns the bounding box representing this node.
    /// @return A constant reference to the area covered by the node.
    const AABB& GetBounds() const
    {
       return mBounds;
    }
    
    /// Calculates the total number of elements in this node and all its children.
    /// @return The total number of elements in this node.
    size_t GetSize() const
    {
        if (mIsLeaf)
        {
            return mElements.size();
        }
        
        size_t size = 0;
        for (const auto& child : mChildren)
        {
            size += child->GetSize();
        }
        
        return size;
    }

    /// Calculates the height of this node from its deepest branch.
    /// @return The height of this node.
    size_t GetHeight() const
    {
        if (mIsLeaf)
        {
            return 1;
        }
        
        size_t height = 0;
        for (const auto& child : mChildren)
        {
            height = std::max(child->GetHeight(), height);
        }
        
        return height + 1;
    }
    
    /// Inserts a new element with the given data at a given position.
    /// @param data The data representing the element.
    /// @param position The position where the element is.
    /// @return True if the element was successfully inserted.
    bool Insert(T data, const Vector2& position)
    {
        if (!mBounds.Contains(position))
        {
            return false;
        }

        if (!mIsLeaf)
        {
            int index = GetChildIndex(position);
            return mChildren[index]->Insert(data, position);
        }

        mElements.push_back({data, position});

        if (mElements.size() > NodeCapacity)
        {
            Subdivide();
        }

        return true;
    }
    
    /// Removes an element matching the given data at a specified position.
    /// @param data The data of the element to remove.
    /// @param position The position where the element is.
    /// @return True if the element was successfully removed.
    bool Remove(T data, const Vector2& position)
    {
        if (!mBounds.Contains(position))
        {
            return false;
        }

        if (mIsLeaf)
        {
            auto it = std::find_if(mElements.begin(), mElements.end(), [&](const Element& element)
            {
                return element.data == data;
            });
            
            if (it != mElements.end())
            {
                *it = mElements.back();
                mElements.pop_back();
                return true;
            }
            
            return false;
        }

        int index = GetChildIndex(position);
        if (mChildren[index]->Remove(data, position))
        {
            TryMerge();
            return true;
        }

        return false;
    }
    
    /// Finds the closest element to the given target position within a maximum radius.
    /// @param target The position to search around.
    /// @param maxRadius The maximum distance from the target to consider.
    /// @return The closest element if found, or empty.
    std::optional<Element> FindNearest(const Vector2& target, float maxRadius = std::numeric_limits<float>::max()) const
    {
        std::optional<Element> nearest = std::nullopt;
        float bestDistanceSq = maxRadius * maxRadius;
        FindNearest(target, bestDistanceSq, nearest);
        return nearest;
    }
    
    /// Gathers a list of all elements contained by the given bounds.
    /// @param queryBounds The bounding box defining the search region.
    /// @return The list of elements found within the query bounds.
    std::vector<Element> SpatialQuery(const AABB& queryBounds) const
    {
        std::vector<Element> elements;
        SpatialQuery(queryBounds, elements);
        return elements;
    }
    
    /// Copy assignment is deleted to avoid accidental copies.
    Quadtree& operator=(const Quadtree&) = delete;

    /// Move assignment is defaulted to allow efficient transfer of ownership.
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    /// Determines the index to the children array based on where the position belongs to.
    /// @param position The position to check.
    /// @return The index to the corresponding child.
    int GetChildIndex(const Vector2& position) const
    {
        int index = 0;
        
        // Use a Z-order curve to map the children into a one-dimensional sequence.
        Vector2 center = mBounds.GetCenter();
        if (position.x > center.x)
        {
            index += 1;
        }
        if (position.y < center.y)
        {
            index += 2;
        }
        
        return index;
    }
    
    /// Divides this node into a branch by passing its elements into its children.
    void Subdivide()
    {
        Vector2 min = mBounds.min;
        Vector2 max = mBounds.max;
        Vector2 center = mBounds.GetCenter();

        AABB topLeft({min.x, center.y}, {center.x, max.y});
        AABB topRight(center, {max.x, max.y});
        AABB bottomLeft({min.x, min.y}, center);
        AABB bottomRight({center.x, min.y}, {max.x, center.y});

        mChildren[0] = std::make_unique<Quadtree>(topLeft);
        mChildren[1] = std::make_unique<Quadtree>(topRight);
        mChildren[2] = std::make_unique<Quadtree>(bottomLeft);
        mChildren[3] = std::make_unique<Quadtree>(bottomRight);

        for (const auto& element : mElements)
        {
            int index = GetChildIndex(element.position);
            mChildren[index]->Insert(element.data, element.position);
        }

        mIsLeaf = false;
        mElements.clear();
    }

    /// Attempt to merge the children back into this node if their elements fit within this node's capacity.
    void TryMerge()
    {
        for (const auto& child : mChildren)
        {
            if (!child->mIsLeaf)
            {
                return;
            }
        }

        size_t totalElements = 0;
        for (const auto& child : mChildren)
        {
            totalElements += child->mElements.size();
        }

        if (totalElements <= NodeCapacity)
        {
            mElements.reserve(totalElements);
            for (auto& child : mChildren)
            {
                for (auto& element : child->mElements)
                {
                    mElements.push_back(std::move(element));
                }
            }

            for (auto& child : mChildren)
            {
                child.reset();
            }

            mIsLeaf = true;
        }
    }

    /// Recursive helper for finding the nearest element.
    /// @param target The search position.
    /// @param bestDistanceSq The best squared distance found so far.
    /// @param nearest The closest element if found, or empty.
    void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element>& nearest) const
    {
        float distanceX = std::max({mBounds.min.x - target.x, 0.0f, target.x - mBounds.max.x});
        float distanceY = std::max({mBounds.min.y - target.y, 0.0f, target.y - mBounds.max.y});
        float distanceSq = (distanceX * distanceX) + (distanceY * distanceY);
        if (distanceSq > bestDistanceSq)
        {
            return;
        }
        
        if (mIsLeaf)
        {
            for (const auto& element : mElements)
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
        
        Vector2 center = mBounds.GetCenter();
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
            mChildren[index]->FindNearest(target, bestDistanceSq, nearest);
        }
    }
    
    /// Recursive helper for the spatial query.
    /// @param queryBounds The bounding box for the search.
    /// @param elements The list where elements are accumulated.
    void SpatialQuery(const AABB& queryBounds, std::vector<Element>& elements) const
    {
        if (!mBounds.Intersects(queryBounds))
        {
            return;
        }

        if (queryBounds.Contains(mBounds))
        {
            GatherAllElements(elements);
            return;
        }

        if (mIsLeaf)
        {
            for (const auto& element : mElements)
            {
                if (queryBounds.Contains(element.position))
                {
                    elements.push_back(element);
                }
            }
            return;
        }

        for (const auto& child : mChildren)
        {
            child->SpatialQuery(queryBounds, elements);
        }
    }
    
    /// Recursively collect all elements in this node and its children.
    /// @param elements The list where elements are accumulated.
    void GatherAllElements(std::vector<Element>& elements) const
    {
        if (mIsLeaf)
        {
            elements.insert(elements.end(), mElements.begin(), mElements.end());
            return;
        }
        
        for (const auto& child : mChildren)
        {
            child->GatherAllElements(elements);
        }
    }
    
    /// Defines the area covered by this node.
    AABB mBounds;
    
    /// Indicates if this node is an endpoint and can store elements or if it's a branch with children.
    bool mIsLeaf = true;
    
    /// Elements stored by this node when it's a leaf.
    std::vector<Element> mElements;
    
    /// Array containing the four child quadrants in Z-order: Top-Left, Top-Right, Bottom-Left, Bottom-Right.
    std::array<std::unique_ptr<Quadtree>, 4> mChildren;
};
