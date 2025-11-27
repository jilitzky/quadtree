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
    SUCCEED();
}
