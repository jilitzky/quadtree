#include <optional>
#include <gtest/gtest.h>
#include "Quadtree.h"
#include "NewQuadtree.h"

class QuadtreeTest : public ::testing::Test
{
protected:
    Quadtree _tree = { AABB({ 0, 0 }, { 100, 100 }) };
    NewQuadtree _newTree = { AABB({ 0, 0 }, { 100, 100 }), 1 };
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

    _newTree.Insert({ 25, 25 });

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    *                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_newTree.GetSize() == 1);
    ASSERT_TRUE(_newTree.GetHeight() == 1);

    _newTree.Insert({ 87, 87 });

    //  __________ ___________
    // |          |        *  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 2);
    ASSERT_TRUE(_newTree.GetHeight() == 2);

    _newTree.Insert({ 56, 68 });

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          | *   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 3);
    ASSERT_TRUE(_newTree.GetHeight() == 3);

    _newTree.Insert({ 68, 56 });

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          |_*|__|     |
    // |__________|__|*_|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 4);
    ASSERT_TRUE(_newTree.GetHeight() == 4);
}

TEST_F(QuadtreeTest, Insert_OutOfBounds)
{
    const bool inserted = _newTree.Insert({ 101, 101 });
    ASSERT_FALSE(inserted);
    ASSERT_TRUE(_newTree.GetSize() == 0);
}

TEST_F(QuadtreeTest, FindNearest)
{
    _tree.Insert({ 25, 25 });
    _tree.Insert({ 87, 87 });
    _tree.Insert({ 87, 68 });
    _tree.Insert({ 56, 56 });
    _tree.Insert({ 56, 68 });

    const Vector2 expected = { 68, 68 };
    _tree.Insert(expected);

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
    _tree.Insert(expected);

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
    _newTree.Insert({ 25, 25 });
    _newTree.Insert({ 87, 87 });
    _newTree.Insert({ 56, 68 });
    _newTree.Insert({ 68, 56 });

    //  __________ ___________
    // |          |     |  *  |
    // |          |_____|_____|
    // |          |_*|__|     |
    // |__________|__|*_|_____|
    // |          |           |
    // |    *     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_newTree.GetSize() == 4);
    ASSERT_TRUE(_newTree.GetHeight() == 4);

    bool removed = _newTree.Remove({ 68, 56 });
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

    ASSERT_TRUE(_newTree.GetSize() == 3);
    ASSERT_TRUE(_newTree.GetHeight() == 3);

    removed = _newTree.Remove({ 56, 68 });
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

    ASSERT_TRUE(_newTree.GetSize() == 2);
    ASSERT_TRUE(_newTree.GetHeight() == 2);

    removed = _newTree.Remove({ 87, 87 });
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

    ASSERT_TRUE(_newTree.GetSize() == 1);
    ASSERT_TRUE(_newTree.GetHeight() == 1);

    removed = _newTree.Remove({ 25, 25 });
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
    bool removed = _newTree.Remove({ 50, 50 });
    ASSERT_FALSE(removed);
}
