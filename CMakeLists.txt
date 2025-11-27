cmake_minimum_required(VERSION 3.14)

project(Quadtree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF
)
FetchContent_MakeAvailable(googletest)

add_library(Quadtree STATIC
    src/Node.cpp 
    src/Quadtree.cpp 
    include/AABB.h
    include/Node.h
    include/Quadtree.h
    include/Vector2.h
)

target_include_directories(Quadtree PUBLIC 
    include
)

add_executable(QuadtreeTest 
    test/QuadtreeTest.cpp 
)

target_link_libraries(QuadtreeTest PRIVATE 
    Quadtree
    GTest::gtest_main
)
