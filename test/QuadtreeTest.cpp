#include <optional>
#include <gtest/gtest.h>
#include "LinearQuadtree.h"
#include "Quadtree.h"

class QuadtreeTest : public ::testing::Test
{
protected:
    Quadtree<int, 1> _tree = { AABB({ 0, 0 }, { 100, 100 }) };
    LinearQuadtree<int, 1> _newTree = { AABB({ 0, 0 }, { 100, 100 }) };
};

TEST_F(QuadtreeTest, Insert)
{
    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_newTree.GetSize() == 0);
    ASSERT_TRUE(_newTree.GetHeight() == 1);

    _newTree.Insert(1, { 25, 25 });

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    1                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_newTree.GetSize() == 1);
    ASSERT_TRUE(_newTree.GetHeight() == 1);

    _newTree.Insert(2, { 87, 87 });

    //  __________ ___________
    // |          |        2  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 2);
    ASSERT_TRUE(_newTree.GetHeight() == 2);

    _newTree.Insert(3, { 56, 68 });

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          | 3   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 3);
    ASSERT_TRUE(_newTree.GetHeight() == 3);

    _newTree.Insert(4, { 68, 56 });

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 4);
    ASSERT_TRUE(_newTree.GetHeight() == 4);
}

TEST_F(QuadtreeTest, Insert_OutOfBounds)
{
    bool inserted = _newTree.Insert(1, { 101, 101 });
    ASSERT_FALSE(inserted);
    ASSERT_TRUE(_newTree.GetSize() == 0);
}

TEST_F(QuadtreeTest, FindNearest)
{
    _tree.Insert({ 25, 25 }, 1);
    _tree.Insert({ 87, 87 }, 2);
    _tree.Insert({ 87, 68 }, 3);
    _tree.Insert({ 56, 56 }, 4);
    _tree.Insert({ 56, 68 }, 5);

    Vector2 expected = { 68, 68 };
    _tree.Insert(expected, 6);

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|x____|
    // |          |_5|_6|  3  |
    // |__________|_4|__|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    auto nearest = _tree.FindNearest({ 75, 75 });
    ASSERT_TRUE(nearest.value().position == expected);
}

TEST_F(QuadtreeTest, FindNearest_Single)
{
    Vector2 expected = { 25, 25 };
    _tree.Insert(expected, 1);

    auto nearest = _tree.FindNearest({ 50, 50 });
    ASSERT_TRUE(nearest.value().position == expected);
}

TEST_F(QuadtreeTest, FindNearest_Empty)
{
    auto nearest = _tree.FindNearest({ 50, 50 });
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, FindNearest_OutOfBounds)
{
    auto nearest = _tree.FindNearest({ 101, 101 });
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, Remove)
{
    _newTree.Insert(1, { 25, 25 });
    _newTree.Insert(2, { 87, 87 });
    _newTree.Insert(3, { 56, 68 });
    _newTree.Insert(4, { 68, 56 });

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 4);
    ASSERT_TRUE(_newTree.GetHeight() == 4);

    bool removed = _newTree.Remove(4, { 68, 56 });
    ASSERT_TRUE(removed);

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          | 3   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 3);
    ASSERT_TRUE(_newTree.GetHeight() == 3);

    removed = _newTree.Remove(3, { 56, 68 });
    ASSERT_TRUE(removed);

    //  __________ ___________
    // |          |        2  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 2);
    ASSERT_TRUE(_newTree.GetHeight() == 2);

    removed = _newTree.Remove(2, { 87, 87 });
    ASSERT_TRUE(removed);

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    1                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_newTree.GetSize() == 1);
    ASSERT_TRUE(_newTree.GetHeight() == 1);

    removed = _newTree.Remove(1, { 25, 25 });
    ASSERT_TRUE(removed);

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_newTree.GetSize() == 0);
    ASSERT_TRUE(_newTree.GetHeight() == 1);
}

TEST_F(QuadtreeTest, Remove_NotFound)
{
    bool removed = _newTree.Remove(1, { 50, 50 });
    ASSERT_FALSE(removed);
}
