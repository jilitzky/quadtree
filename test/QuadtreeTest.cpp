#include <optional>
#include <gtest/gtest.h>
#include "LinearQuadtree.h"
#include "Quadtree.h"

class QuadtreeTest : public ::testing::Test
{
protected:
    using TestTree = Quadtree<int, 1>;
    TestTree _tree = { AABB({ 0, 0 }, { 100, 100 }) };
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

    ASSERT_TRUE(_tree.GetSize() == 0);
    ASSERT_TRUE(_tree.GetHeight() == 1);

    _tree.Insert({ 25, 25 }, 1);

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    1                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_tree.GetSize() == 1);
    ASSERT_TRUE(_tree.GetHeight() == 1);

    _tree.Insert({ 87, 87 }, 2);

    //  __________ ___________
    // |          |        2  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.GetSize() == 2);
    ASSERT_TRUE(_tree.GetHeight() == 2);

    _tree.Insert({ 56, 68 }, 3);

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          | 3   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.GetSize() == 3);
    ASSERT_TRUE(_tree.GetHeight() == 3);

    _tree.Insert({ 68, 56 }, 4);

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.GetSize() == 4);
    ASSERT_TRUE(_tree.GetHeight() == 4);
}

TEST_F(QuadtreeTest, Insert_OutOfBounds)
{
    bool inserted = _tree.Insert({ 101, 101 }, 1);
    ASSERT_FALSE(inserted);
    ASSERT_TRUE(_tree.GetSize() == 0);
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

    std::optional<TestTree::Element> nearest = _tree.FindNearest({ 75, 75 });
    ASSERT_TRUE(nearest.value().position == expected);
}

TEST_F(QuadtreeTest, FindNearest_Single)
{
    Vector2 expected = { 25, 25 };
    _tree.Insert(expected, 1);

    std::optional<TestTree::Element> nearest = _tree.FindNearest({ 50, 50 });
    ASSERT_TRUE(nearest.value().position == expected);
}

TEST_F(QuadtreeTest, FindNearest_Empty)
{
    std::optional<TestTree::Element> nearest = _tree.FindNearest({ 50, 50 });
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, FindNearest_OutOfBounds)
{
    std::optional<TestTree::Element> nearest = _tree.FindNearest({ 101, 101 });
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, Remove)
{
    _tree.Insert({ 25, 25 }, 1);
    _tree.Insert({ 87, 87 }, 2);
    _tree.Insert({ 56, 68 }, 3);
    _tree.Insert({ 68, 56 }, 4);

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.GetSize() == 4);
    ASSERT_TRUE(_tree.GetHeight() == 4);

    bool removed = _tree.Remove({ 68, 56 }, 4);
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

    ASSERT_TRUE(_tree.GetSize() == 3);
    ASSERT_TRUE(_tree.GetHeight() == 3);

    removed = _tree.Remove({ 56, 68 }, 3);
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

    ASSERT_TRUE(_tree.GetSize() == 2);
    ASSERT_TRUE(_tree.GetHeight() == 2);

    removed = _tree.Remove({ 87, 87 }, 2);
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

    ASSERT_TRUE(_tree.GetSize() == 1);
    ASSERT_TRUE(_tree.GetHeight() == 1);

    removed = _tree.Remove({ 25, 25 }, 1);
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

    ASSERT_TRUE(_tree.GetSize() == 0);
    ASSERT_TRUE(_tree.GetHeight() == 1);
}

TEST_F(QuadtreeTest, Remove_NotFound)
{
    bool removed = _tree.Remove({ 50, 50 }, 1);
    ASSERT_FALSE(removed);
}
