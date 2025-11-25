add_library(QuadtreeLib STATIC
    src/Node.cpp 
    src/Quadtree.cpp 
    include/Bounds.h
    include/Node.h
    include/Quadtree.h
    include/Vector2.h
)

target_include_directories(QuadtreeLib PUBLIC 
    include
)
