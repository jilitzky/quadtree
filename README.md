# Quadtree
A basic implementation of a quadtree spatial partition and a find-nearest search.
## Interface
### Accessors
- **Depth:** Returns the number of levels
- **Size:** Returns the number of points
### Modifiers
- **Add:** Adds a point
- **FindNearest:** Returns the closest point to the query location
- **Remove:** Removes a point
## Implementation Details
### Node
Represents a region in the quadtree at a specific depth with a perimeter defined by its min and max coordinates. It can hold a point if it's a leaf node or it can use its children to hold multiple points. A node is created on demand as points are added to the tree and can be destroyed if no longer needed when points are removed from the tree.
### Point
A 2D coordinate that supports equality and arithmetic operators. Most game engines have an equivalent struct, usually named `Vector`.
