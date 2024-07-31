# Quadtree
A basic implementation of a quadtree spatial partition and a find-nearest search.
## Interface
- **Add:** Adds a point to the tree
- **Depth:** Returns the number of levels in the tree
- **FindNearest:** Returns the closest point to the query location
- **Remove:** Removes a point from the tree
- **Size:** Returns the number of points in the tree
## Implementation Details
### Bounds
An axis-aligned 2D box with a min and max coordinate.
### Node
Represents a region in the quadtree at a specific depth with a perimeter defined by its bounds. It can hold a point if it's a leaf node or it can use its children to hold multiple points. A node is created on-demand as points are added to the tree and can be destroyed if no longer needed when points are removed from the tree.
### Vector2
A 2D coordinate that supports equality and arithmetic operators.
