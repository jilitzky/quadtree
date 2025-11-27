#include <optional>
#include <gtest/gtest.h>
#include "Quadtree.h"

class QuadtreeBenchmark : public ::testing::Test
{
protected:
    Quadtree _tree = { { 0, 0 }, { 1000, 1000 } };
};

TEST_F(QuadtreeBenchmark, Benchmark)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Benchmark time: " << duration << " ms" << std::endl;

    SUCCEED();
}
