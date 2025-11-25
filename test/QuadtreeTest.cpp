#include <optional>
#include <gtest/gtest.h>
#include "Quadtree.h"

// TODO: Do I actually need this namespace?
namespace
{
    class QuadtreeTest : public ::testing::Test
    {
    protected:
        Quadtree _tree = { { 0, 0 }, { 100, 100 } };
    };
}

TEST_F(QuadtreeTest, Add)
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

    ASSERT_TRUE(_tree.Size() == 0);
    ASSERT_TRUE(_tree.Depth() == 1);

    _tree.Add({ 25, 25 });

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    *                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_tree.Size() == 1);
    ASSERT_TRUE(_tree.Depth() == 1);

    _tree.Add({ 87, 87 });

    //  __________ ___________
    // |          |        *  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.Size() == 2);
    ASSERT_TRUE(_tree.Depth() == 2);

    _tree.Add({ 56, 68 });

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          | *   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.Size() == 3);
    ASSERT_TRUE(_tree.Depth() == 3);

    _tree.Add({ 68, 56 });

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          |_*|__|     |
    // |__________|__|*_|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.Size() == 4);
    ASSERT_TRUE(_tree.Depth() == 4);
}

TEST_F(QuadtreeTest, Add_CannotSubdivide)
{
    Quadtree tinyTree({ 0, 0 }, { 1, 1 });

    tinyTree.Add({ 0, 0 });
    ASSERT_TRUE(tinyTree.Size() == 1);

    const bool added = tinyTree.Add({ 1, 1 });
    ASSERT_FALSE(added);
    ASSERT_TRUE(tinyTree.Size() == 1);
}

TEST_F(QuadtreeTest, Add_OutOfBounds)
{
    const bool added = _tree.Add({ 101, 101 });
    ASSERT_FALSE(added);
    ASSERT_TRUE(_tree.Size() == 0);
}

TEST_F(QuadtreeTest, Add_Overlapping)
{
    _tree.Add({ 50, 50 });
    ASSERT_TRUE(_tree.Size() == 1);

    const bool added = _tree.Add({ 50, 50 });
    ASSERT_FALSE(added);
    ASSERT_TRUE(_tree.Size() == 1);
}

TEST_F(QuadtreeTest, FindNearest)
{
    _tree.Add({ 25, 25 });
    _tree.Add({ 87, 87 });
    _tree.Add({ 87, 68 });
    _tree.Add({ 56, 56 });
    _tree.Add({ 56, 68 });

    const Vector2 expected = { 68, 68 };
    _tree.Add(expected);

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|O____|
    // |          |_*|_*|  *  |
    // |__________|_*|__|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    std::optional<Vector2> nearest = _tree.FindNearest({ 75, 75 });
    ASSERT_TRUE(nearest.value() == expected);
}

TEST_F(QuadtreeTest, FindNearest_Single)
{
    const Vector2 expected = { 25, 25 };
    _tree.Add(expected);

    std::optional<Vector2> nearest = _tree.FindNearest({ 50, 50 });
    ASSERT_TRUE(nearest.value() == expected);
}

TEST_F(QuadtreeTest, FindNearest_Empty)
{
    std::optional<Vector2> nearest = _tree.FindNearest({ 50, 50 });
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, FindNearest_OutOfBounds)
{
    std::optional<Vector2> nearest = _tree.FindNearest({ 101, 101 });
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, Remove)
{
    _tree.Add({ 25, 25 });
    _tree.Add({ 87, 87 });
    _tree.Add({ 56, 68 });
    _tree.Add({ 68, 56 });

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          |_*|__|     |
    // |__________|__|*_|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.Size() == 4);
    ASSERT_TRUE(_tree.Depth() == 4);

    bool removed = _tree.Remove({ 68, 56 });
    ASSERT_TRUE(removed);

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          | *   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.Size() == 3);
    ASSERT_TRUE(_tree.Depth() == 3);

    removed = _tree.Remove({ 56, 68 });
    ASSERT_TRUE(removed);

    //  __________ ___________
    // |          |        *  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.Size() == 2);
    ASSERT_TRUE(_tree.Depth() == 2);

    removed = _tree.Remove({ 87, 87 });
    ASSERT_TRUE(removed);

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    *                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_tree.Size() == 1);
    ASSERT_TRUE(_tree.Depth() == 1);

    removed = _tree.Remove({ 25, 25 });
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

    ASSERT_TRUE(_tree.Size() == 0);
    ASSERT_TRUE(_tree.Depth() == 1);
}

TEST_F(QuadtreeTest, Remove_NotFound)
{
    bool removed = _tree.Remove({ 50, 50 });
    ASSERT_FALSE(removed);
}
