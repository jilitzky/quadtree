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
* **Generic:** The templated arguments allow you to configure the type of data and 2D vectors stored by the tree.
* **Dynamic:** Efficient insertion, removal and automatic subdivision/merging of nodes.
* **Performant:** Fast searches for nearest neighbour and to find all elements within a search area.
* **Header-only:** Easy to drop into any project.

## Installation
Open the `include` folder and copy `Quadtree.h` into your project's include path.

## Usage
### Find Nearest
```cpp
// Create a Quadtree that covers an area of 100 x 100 and can hold up to 1 element per node.
size_t nodeCapacity = 1;
Quadtree<int, glm::vec2> tree = {{0, 0}, {100, 100}, nodeCapacity};

// Insert a few elements into it.
tree.Insert(1, {25, 25});
tree.Insert(2, {87, 87});
tree.Insert(3, {87, 68});
tree.Insert(4, {56, 56});
tree.Insert(5, {56, 68});
tree.Insert(6, {68, 68});

//  __________ ___________
// |          |     |  2  |
// |          |_____|x____|
// |          |_5|_6|  3  |
// |__________|_4|__|_____|
// |          |           |
// |    1     |           |
// |          |           |
// |__________|___________|

// Find the nearest odd element to the "x" mark.
auto isOdd = [](const auto& element) { return element.data % 2 == 1; };
auto nearest = tree.FindNearest({75, 75}, isOdd); // nearest is 3
```
### Find All
```cpp
// Create a Quadtree that covers an area of 100 x 100 and can hold up to 1 element per node.
size_t nodeCapacity = 1;
Quadtree<int, glm::vec2> tree = {{0, 0}, {100, 100}, nodeCapacity};

// Insert a few elements into it.
tree.Insert(1, {25, 25});
tree.Insert(2, {87, 87});
tree.Insert(3, {87, 68});
tree.Insert(4, {56, 56});
tree.Insert(5, {56, 68});
tree.Insert(6, {68, 68});

//  __________ ___________
// |        ..|.....|. 2  |
// |        . |_____|.____|
// |        . |_5|_6|. 3  |
// |________._|_4|__|.____|
// |        ..|.......    |
// |    1     |           |
// |          |           |
// |__________|___________|

// Find all even elements inside the search area.
auto isEven = [](const auto& element) { return element.data % 2 == 0; };
auto elements = tree.FindAll({40, 38}, {75, 88}, isEven); // elements contains 4 and 6
```

## Performance
### Benchmark
1. Create a Quadtree that covers an area of 2000 x 2000
2. Configure its nodes to hold up to 8 elements before subdividing
3. Allow the tree to have a max depth of 4 (0 being the root level)
4. Read a pre-generated file with 10,000 positions
5. Measure inserting an element at every position, run find queries and then remove all elements
### Results (Intel i7-13700H)
| Operation     | Time (Avg) |
| ------------- | ---------- |
| Insertion     | 59 ns      |
| Find Nearest  | 356 ns     |
| Find All      | 26808 ns   |
| Removal       | 84 ns      |

## License
Distributed under the MIT License. See LICENSE for more information.
