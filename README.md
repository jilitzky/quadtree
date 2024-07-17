# quadtree
## Summary
A basic implementation of a quadtree spatial partition and a find-nearest search.
## API
### Accessors
- **Depth:** Access the number of levels in the tree
- **Size:** Access the number of points in the tree
### Modifiers
- **Add:** Adds a point
- **FindNearest:** Returns the closest point to the query location
- **Remove:** Removes a point
## Implementation Details
### Node
Represents a region in the quadtree at a specific depth. Its perimeter is defined by its min and max coordinates. It can hold a point if it's a leaf node or it can use its children to hold multiple points. A node is created on demand as points are added to the tree and can be destroyed if no longer needed when points are removed from the tree.
### Point
A 2D coordinate that supports equality and arithmetic operators. Most game engines have an equivalent struct, usually named `Vector`.
### Quadtree
A data structure that partitions space into four equivalent regions and continues to do so in finer increments as it deepens. It runs validation to ensure the points given as arguments are within its bounds and keeps track of its size by taking into account the result of every add and remove operation performed on its root node.
