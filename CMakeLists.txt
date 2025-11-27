cmake_minimum_required(VERSION 3.20)

project(Quadtree VERSION 1.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF
)
FetchContent_MakeAvailable(googletest)

# --- Quadtree ---

add_library(Quadtree STATIC
    include/AABB.h
    include/Node.h
    include/Quadtree.h
    include/Vector2.h
    src/Node.cpp 
    src/Quadtree.cpp
)

target_compile_features(Quadtree PUBLIC cxx_std_17)

target_include_directories(Quadtree PUBLIC
    include
)

# --- QuadtreeTest ---

add_executable(QuadtreeTest 
    test/QuadtreeTest.cpp 
)

target_compile_features(QuadtreeTest PRIVATE cxx_std_17)

target_link_libraries(QuadtreeTest PRIVATE 
    Quadtree
    GTest::gtest_main
)

# --- QuadtreeBenchmark ---

add_executable(QuadtreeBenchmark 
    bench/QuadtreeBenchmark.cpp 
)

target_compile_features(QuadtreeBenchmark PRIVATE cxx_std_17)

target_link_libraries(QuadtreeBenchmark PRIVATE 
    Quadtree
    GTest::gtest_main
)
