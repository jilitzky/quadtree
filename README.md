# Quadtree

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

**A modern C++ implementation of a quadtree spatial partition.**

Provides a fast, lightweight and generic Quadtree data structure for efficient 2D spatial partitioning and querying.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Performance](#performance)
- [License](#license)

## Features
* **Modern C++ Design:** Written using C++17.
* **Generic:** The templated data type allows you to store any custom structure or pointer.
* **Dynamic:** Efficient insertion, removal and automatic subdivision/merging of nodes.
* **Spatial Queries:** Fast nearest-neighbor lookups and range queries (retrieve all objects within a boundary).
* **Header-only:** Easy to drop into any project.

## Installation
Simply copy the contents of the `include/` directory to your project's include path.

## Usage
### Find Nearest
```cpp
// Initialize a Quadtree that stores up to 1 int per node
Quadtree<int, 1> tree = { AABB({ 0, 0 }, { 100, 100 }) };

tree.Insert(1, { 25, 25 });
tree.Insert(2, { 87, 87 });
tree.Insert(3, { 87, 68 });
tree.Insert(4, { 56, 56 });
tree.Insert(5, { 56, 68 });
tree.Insert(6, { 68, 68 });

//  __________ ___________
// |          |     |  2  |
// |          |_____|x____|
// |          |_5|_6|  3  |
// |__________|_4|__|_____|
// |          |           |
// |    1     |           |
// |          |           |
// |__________|___________|

auto nearest = tree.FindNearest({ 75, 75 }); // Nearest is 6.
```
### Spatial Query
```cpp
// Initialize a Quadtree that stores up to 1 int per node
Quadtree<int, 1> tree = { AABB({ 0, 0 }, { 100, 100 }) };

tree.Insert(1, { 25, 25 });
tree.Insert(2, { 87, 87 });
tree.Insert(3, { 56, 68 });
tree.Insert(4, { 68, 56 });

//  __________ ___________
// |        ..|.....|. 2  |
// |        . |_____|.____|
// |        . |_3|__|.    |
// |________._|__|4_|.____|
// |        ..|.......    |
// |    1     |           |
// |          |           |
// |__________|___________|

AABB queryBounds = {{40, 38}, {75, 88}};
auto elements = tree.SpatialQuery(queryBounds); // Elements contains 3 and 4.
```

## Performance
Benchmark was run averaging 10,000 operations with a `NodeCapacity` of 16 on an Apple M2 Pro.
| Operation     | Time (Avg) |
| ------------- | ---------- |
| Insertion     | 328 ns     |
| Removal       | 401 ns     |
| Find Nearest  | 1445 ns    |
| Spatial Query | 68179 ns   |

## License
Distributed under the MIT License. See LICENSE for more information.
