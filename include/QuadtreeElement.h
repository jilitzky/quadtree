/// Copyright (c) 2025 Jose Ilitzky

#pragma once

#include "Vector2.h"

/// Represents an item stored in the tree.
/// @tparam T The type of data representing the element.
template<typename T>
struct QuadtreeElement
{
    /// The data representing the element.
    T data;
    
    /// The position linked to the data.
    Vector2 position;
};
