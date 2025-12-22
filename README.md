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
* **Performant:** Fast searches for nearest neighbour and all elements contained within a region.
* **Header-only:** Easy to drop into any project.

## Installation
Simply copy the contents of the `include/` directory to your project's include path.

## Usage
### Find Nearest
```cpp
AABB bounds = {{ 0, 0 }, { 100, 100 }};
size_t nodeCapacity = 1;
Quadtree<int> tree = { bounds, nodeCapacity };

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
### Find All
```cpp
AABB bounds = {{ 0, 0 }, { 100, 100 }};
size_t nodeCapacity = 1;
Quadtree<int> tree = { bounds, nodeCapacity };

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

AABB region = {{40, 38}, {75, 88}};
auto elements = tree.FindAll(region); // Elements contains 3 and 4.
```

## Performance
Benchmark was run averaging 10,000 calls per operation on an Apple M2 Pro.
| Operation     | Time (Avg) |
| ------------- | ---------- |
| Insertion     | 187 ns     |
| Removal       | 403 ns     |
| Find Nearest  | 1450 ns    |
| Find All      | 46502 ns   |

## License
Distributed under the MIT License. See LICENSE for more information.
