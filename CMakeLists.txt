cmake_minimum_required(VERSION 3.20)

project(Quadtree VERSION 1.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")

# --- QuadtreeLib ---

add_library(QuadtreeLib STATIC
    include/AABB.h
    include/Node.h
    include/Quadtree.h
    include/Vector2.h
    src/Node.cpp 
    src/Quadtree.cpp
)

target_compile_features(QuadtreeLib PUBLIC cxx_std_17)

target_include_directories(QuadtreeLib PUBLIC
    include
)

# --- QuadtreeTest ---

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF
)
FetchContent_MakeAvailable(googletest)

add_executable(QuadtreeTest 
    test/QuadtreeTest.cpp 
)

target_compile_features(QuadtreeTest PRIVATE cxx_std_17)

target_link_libraries(QuadtreeTest PRIVATE 
    QuadtreeLib
    GTest::gtest_main
)