#pragma once
#include <optional>
#include <vector>
#include "AABB.h"
#include "Vector2.h"

/// A data structure that partitions 2D space into quadrants and provides efficient spatial queries.
/// @tparam T The type of data payload representing elements in the tree.
/// @tparam Capacity The maximum number of elements that can be stored per node before subdividing.
template<typename T, size_t Capacity>
class Quadtree
{
public:
    /// Represents an item stored in the Quadtree.
    struct Element
    {
        /// The data payload for this element.
        T data;
        
        /// The position linked to the data.
        Vector2 position;
    };

    /// Constructor that initializes the Quadtree to the given bounding box.
    /// @param bounds The bounding box that defines the area covered by this node.
    Quadtree(const AABB& bounds) : mBounds(bounds)
    {
    }
    
    /// Copy constructor is deleted to prevent accidental copy operations.
    Quadtree(const Quadtree& other) = delete;
    
    /// Move constructor to allow for efficient transfer of ownership from a temporary object.
    Quadtree(Quadtree&& other) = default;
    
    /// Returns the bounding box representing this node.
    /// @return A constant reference to the bounding box.
    const AABB& GetBounds() const
    {
       return mBounds;
    }
    
    /// Calculates and returns the total number of elements in this node and all its children.
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

    /// Calculates and returns the depth of the deepest branch, originating from this node.
    /// @return The height of this node from the bottom of the tree.
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
    
    /// Inserts a new element with the data payload at a given position.
    /// @param data The payload to store.
    /// @param position The position where the element is.
    /// @return True if the element was successfully added.
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

        if (mElements.size() > Capacity)
        {
            Subdivide();
        }

        return true;
    }
    
    /// Removes an element matching the given data at a specified position
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
    
    /// Finds the closest element to the specified position.
    /// @param target The position to search around.
    /// @return The closest element if found, or empty
    std::optional<Element> FindNearest(const Vector2& target) const
    {
        std::optional<Element> nearest = std::nullopt;
        float bestDistanceSq = std::numeric_limits<float>::max();
        FindNearest(target, bestDistanceSq, nearest);
        return nearest;
    }
    
    /// Get a list of all elements contained by the given bounds.
    /// @param bounds The bounding box defining the search region.
    /// @return The list of elements found within the query bounds.
    std::vector<Element> Query(const AABB& bounds) const
    {
        std::vector<Element> elements;
        Query(bounds, elements);
        return elements;
    }
    
    /// Copy assignment is deleted to guard against accidental copies.
    Quadtree& operator=(const Quadtree&) = delete;

    /// Move assignment is allowed.
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    /// Determines the index to the children array based on where the given position belongs.
    /// @param position The position to check.
    /// @return The index to the corresponding child.
    int GetChildIndex(const Vector2& position) const
    {
        int index = 0;
     
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

        AABB topLeft(Vector2(min.x, center.y), Vector2(center.x, max.y));
        AABB topRight(Vector2(center.x, center.y), Vector2(max.x, max.y));
        AABB bottomLeft(Vector2(min.x, min.y), Vector2(center.x, center.y));
        AABB bottomRight(Vector2(center.x, min.y), Vector2(max.x, center.y));

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

        size_t totalSize = 0;
        for (const auto& child : mChildren)
        {
            totalSize += child->mElements.size();
        }

        if (totalSize <= Capacity)
        {
            mElements.reserve(totalSize);
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
        for (const auto& element : mElements)
        {
            float distanceSq = element.position.DistanceSquared(target);
            if (distanceSq < bestDistanceSq)
            {
                bestDistanceSq = distanceSq;
                nearest = element;
            }
        }

        if (mIsLeaf)
        {
            return;
        }
        
        Vector2 center = mBounds.GetCenter();
        int isRight = target.x >= center.x;
        int isBottom = target.y < center.y;

        std::array<int, 4> sortedIndices;
        sortedIndices[0] = isBottom * 2 + isRight;
        sortedIndices[1] = isBottom * 2 + (1 - isRight);
        sortedIndices[2] = (1 - isBottom) * 2 + isRight;
        sortedIndices[3] = (1 - isBottom) * 2 + (1 - isRight);

        for (int index : sortedIndices)
        {
            const auto& child = mChildren[index];
            AABB childBounds = child->mBounds;
            float dx = std::max({childBounds.min.x - target.x, 0.0f, target.x - childBounds.max.x});
            float dy = std::max({childBounds.min.y - target.y, 0.0f, target.y - childBounds.max.y});
            float childDistanceSq = (dx * dx) + (dy * dy);
            if (childDistanceSq < bestDistanceSq)
            {
                child->FindNearest(target, bestDistanceSq, nearest);
            }
        }
    }
    
    /// Recursive helper for the spatial query.
    /// @param bounds The bounding box for the search.
    /// @param elements The list where elements are accumulated.
    void Query(const AABB& bounds, std::vector<Element>& elements) const
    {
        if (!mBounds.Intersects(bounds))
        {
            return;
        }

        if (bounds.Contains(mBounds))
        {
            GatherAllElements(elements);
            return;
        }

        if (mIsLeaf)
        {
            for (const auto& element : mElements)
            {
                if (bounds.Contains(element.position))
                {
                    elements.push_back(element);
                }
            }
            return;
        }

        for (const auto& child : mChildren)
        {
            child->Query(bounds, elements);
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
    
    /// Array containing the four child quadrants: Top-Left, Top-Right, Bottom-Left, Bottom-Right.
    std::array<std::unique_ptr<Quadtree>, 4> mChildren;
};
