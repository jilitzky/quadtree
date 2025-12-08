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
    Quadtree(const AABB& bounds);
    
    /// Copy constructor is deleted to prevent accidental copy operations.
    Quadtree(const Quadtree& other) = delete;
    
    /// Move constructor to allow for efficient transfer of ownership from a temporary object.
    Quadtree(Quadtree&& other) = default;
    
    /// Returns the bounding box representing this node.
    /// @return A constant reference to the bounding box.
    const AABB& GetBounds() const;
    
    /// Calculates and returns the total number of elements in this node and all its children.
    /// @return The total number of elements in this node.
    size_t GetSize() const;

    /// Calculates and returns the depth of the deepest branch, originating from this node.
    /// @return The height of this node from the bottom of the tree.
    size_t GetHeight() const;
    
    /// Inserts a new element with the data payload at a given position.
    /// @param data The payload to store.
    /// @param position The position where the element is.
    /// @return True if the element was successfully added.
    bool Insert(T data, const Vector2& position);
    
    /// Removes an element matching the given data at a specified position
    /// @param data The data of the element to remove.
    /// @param position The position where the element is.
    /// @return True if the element was successfully removed.
    bool Remove(T data, const Vector2& position);
    
    /// Finds the closest element to the specified position.
    /// @param target The position to search around.
    /// @return The closest element if found, or empty
    std::optional<Element> FindNearest(const Vector2& target) const;
    
    /// Get a list of all elements contained by the given bounds.
    /// @param bounds The bounding box defining the search region.
    /// @return The list of elements found within the query bounds.
    std::vector<Element> Query(const AABB& bounds) const;
    
    /// Copy assignment is deleted to guard against accidental copies.
    Quadtree& operator=(const Quadtree&) = delete;

    /// Move assignment is allowed.
    Quadtree& operator=(Quadtree&&) = default;
    
private:
    /// Determines the index to the children array based on where the given position belongs.
    /// @param position The position to check.
    /// @return The index to the corresponding child.
    int GetChildIndex(const Vector2& position) const;
    
    /// Divides this node into a branch by passing its elements into its children.
    void Subdivide();

    /// Attempt to merge the children back into this node if their elements fit within this node's capacity.
    void TryMerge();

    /// Recursive helper for finding the nearest element.
    /// @param target The search position.
    /// @param bestDistanceSq The best squared distance found so far.
    /// @param nearest The closest element if found, or empty.
    void FindNearest(const Vector2& target, float& bestDistanceSq, std::optional<Element>& nearest) const;
    
    /// Recursive helper for the spatial query.
    /// @param bounds The bounding box for the search.
    /// @param elements The list where elements are accumulated.
    void Query(const AABB& bounds, std::vector<Element>& elements) const;
    
    /// Recursively collect all elements in this node and its children.
    /// @param elements The list where elements are accumulated.
    void GatherAllElements(std::vector<Element>& elements) const;
    
    /// Defines the area covered by this node.
    AABB mBounds;
    
    /// Indicates if this node is an endpoint and can store elements or if it's a branch with children.
    bool mIsLeaf = true;
    
    /// Elements stored by this node when it's a leaf.
    std::vector<Element> mElements;
    
    /// Array containing the four child quadrants: Top-Left, Top-Right, Bottom-Left, Bottom-Right.
    std::array<std::unique_ptr<Quadtree>, 4> mChildren;
};

#include "Quadtree.inl"
