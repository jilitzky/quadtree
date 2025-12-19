/// Copyright (c) 2025 Jose Ilitzky

#include <optional>
#include <gtest/gtest.h>
#include "Quadtree.h"

class QuadtreeTest : public ::testing::Test
{
protected:
    Quadtree<int> _tree = { AABB({ 0, 0 }, { 100, 100 }), 1 };
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

    ASSERT_TRUE(_tree.CountElements() == 0);
    ASSERT_TRUE(_tree.GetHeight() == 1);

    _tree.Insert(1, { 25, 25 });

    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    1                 |
    // |                      |
    // |______________________|

    ASSERT_TRUE(_tree.CountElements() == 1);
    ASSERT_TRUE(_tree.GetHeight() == 1);

    _tree.Insert(2, { 87, 87 });

    //  __________ ___________
    // |          |        2  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.CountElements() == 2);
    ASSERT_TRUE(_tree.GetHeight() == 2);

    _tree.Insert(3, { 56, 68 });

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          | 3   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.CountElements() == 3);
    ASSERT_TRUE(_tree.GetHeight() == 3);

    _tree.Insert(4, { 68, 56 });

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.CountElements() == 4);
    ASSERT_TRUE(_tree.GetHeight() == 4);
}

TEST_F(QuadtreeTest, Insert_SamePosition)
{
    _tree.Insert(1, { 50, 50 });
    _tree.Insert(2, { 50, 50 });
    ASSERT_TRUE(_tree.CountElements() == 2);
}

TEST_F(QuadtreeTest, Insert_OutOfBounds)
{
    bool inserted = _tree.Insert(1, { 101, 101 });
    ASSERT_FALSE(inserted);
    ASSERT_TRUE(_tree.CountElements() == 0);
}

TEST_F(QuadtreeTest, FindNearest)
{
    _tree.Insert(1, { 25, 25 });
    _tree.Insert(2, { 87, 87 });
    _tree.Insert(3, { 87, 68 });
    _tree.Insert(4, { 56, 56 });
    _tree.Insert(5, { 56, 68 });
    _tree.Insert(6, { 68, 68 });

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
    ASSERT_TRUE(nearest.value().data == 6);
}

TEST_F(QuadtreeTest, FindNearest_Single)
{
    Vector2 expected = { 25, 25 };
    _tree.Insert(1,expected);

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
    _tree.Insert(1, { 25, 25 });
    _tree.Insert(2, { 87, 87 });
    _tree.Insert(3, { 56, 68 });
    _tree.Insert(4, { 68, 56 });

    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|

    ASSERT_TRUE(_tree.CountElements() == 4);
    ASSERT_TRUE(_tree.GetHeight() == 4);

    bool removed = _tree.Remove(4, { 68, 56 });
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

    ASSERT_TRUE(_tree.CountElements() == 3);
    ASSERT_TRUE(_tree.GetHeight() == 3);

    removed = _tree.Remove(3, { 56, 68 });
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

    ASSERT_TRUE(_tree.CountElements() == 2);
    ASSERT_TRUE(_tree.GetHeight() == 2);

    removed = _tree.Remove(2, { 87, 87 });
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

    ASSERT_TRUE(_tree.CountElements() == 1);
    ASSERT_TRUE(_tree.GetHeight() == 1);

    removed = _tree.Remove(1, { 25, 25 });
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

    ASSERT_TRUE(_tree.CountElements() == 0);
    ASSERT_TRUE(_tree.GetHeight() == 1);
}

TEST_F(QuadtreeTest, Remove_NotFound)
{
    bool removed = _tree.Remove(1, { 50, 50 });
    ASSERT_FALSE(removed);
}

TEST_F(QuadtreeTest, SpatialQuery)
{
    _tree.Insert(1, { 25, 25 });
    _tree.Insert(2, { 87, 87 });
    _tree.Insert(3, { 56, 68 });
    _tree.Insert(4, { 68, 56 });
    
    //  __________ ___________
    // |        ..|.....|. 2  |
    // |        . |_____|.____|
    // |        . |_3|__|.    |
    // |________._|__|4_|.____|
    // |        ..|.......    |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    AABB queryBounds = {{40, 38}, {75, 88}};
    auto elements = _tree.SpatialQuery(queryBounds);
    ASSERT_TRUE(elements.size() == 2);
    
    auto it3 = std::find_if(elements.begin(), elements.end(), [](const auto& element)
    {
        return element.data == 3;
    });
    ASSERT_TRUE(it3 != elements.end());
    
    auto it4 = std::find_if(elements.begin(), elements.end(), [](const auto& element)
    {
        return element.data == 4;
    });
    ASSERT_TRUE(it4 != elements.end());
}
