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
* **Modern Design:** Written in C++17.
* **Generic:** The templated data type allows you to store any custom structure or pointer.
* **Dynamic:** Efficient insertion, removal and automatic subdivision/merging of nodes.
* **Performant:** Fast searches for nearest neighbour and to find all elements contained within a region.
* **Header-only:** Easy to drop into any project.

## Installation
Simply copy the contents of the `include/` directory to your project's include path.

## Usage
### Find Nearest
```cpp
// Create a Quadtree that covers an area of 100 x 100 and can hold up to 1 element per node.
AABB bounds = {{ 0, 0 }, { 100, 100 }};
size_t nodeCapacity = 1;
Quadtree<int> tree = { bounds, nodeCapacity };

// Insert a few elements into it.
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

// Find the closest element to the "x" mark.
auto nearest = tree.FindNearest({ 75, 75 }); // The nearest element is 6
```
### Find All
```cpp
// Create a Quadtree that covers an area of 100 x 100 and can hold up to 1 element per node.
AABB bounds = {{ 0, 0 }, { 100, 100 }};
size_t nodeCapacity = 1;
Quadtree<int> tree = { bounds, nodeCapacity };

// Insert a few elements into it.
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

// Draw a region around some of the elements.
AABB region = {{40, 38}, {75, 88}};
auto elements = tree.FindAll(region); // The found elements are 3 and 4
```

## Performance
### Benchmark
1. Create a Quadtree that covers an area of 2000 x 2000.
2. Configure its nodes to hold up to 8 elements before subdividing.
3. Allow the tree to have a max depth of 4 (0 being the root level).
4. Read a pre-generated file with 10,000 positions.
5. Measure inserting an element at every position, run find queries and then remove all elements.
### Results (Apple M2 Pro)
| Operation     | Time (Avg) |
| ------------- | ---------- |
| Insertion     | 187 ns     |
| Removal       | 403 ns     |
| Find Nearest  | 1450 ns    |
| Find All      | 46502 ns   |

## License
Distributed under the MIT License. See LICENSE for more information.
