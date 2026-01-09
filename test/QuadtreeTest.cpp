/// Copyright (c) 2025 Jose Ilitzky

#include <optional>
#include <vector>
#include <glm/vec2.hpp>
#include <gtest/gtest.h>
#include "Quadtree.h"

class QuadtreeTest : public ::testing::Test
{
    using Tree = Quadtree<int, glm::vec2>;
    
protected:
    Tree tree = {{0, 0}, {100, 100}, 1};
    
    bool ContainsData(const std::vector<Tree::Element>& elements, int data)
    {
        for (const auto& element : elements)
        {
            if (element.data == data)
            {
                return true;
            }
        }
        return false;
    }
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
    
    ASSERT_TRUE(tree.CountElements() == 0);
    ASSERT_TRUE(tree.GetHeight() == 1);
    
    tree.Insert(1, {25, 25});
    
    //  ______________________
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |                      |
    // |    1                 |
    // |                      |
    // |______________________|
    
    ASSERT_TRUE(tree.CountElements() == 1);
    ASSERT_TRUE(tree.GetHeight() == 1);
    
    tree.Insert(2, {87, 87});
    
    //  __________ ___________
    // |          |        2  |
    // |          |           |
    // |          |           |
    // |__________|___________|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    ASSERT_TRUE(tree.CountElements() == 2);
    ASSERT_TRUE(tree.GetHeight() == 2);
    
    tree.Insert(3, {56, 68});
    
    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          | 3   |     |
    // |__________|_____|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    ASSERT_TRUE(tree.CountElements() == 3);
    ASSERT_TRUE(tree.GetHeight() == 3);
    
    tree.Insert(4, {68, 56});
    
    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    ASSERT_TRUE(tree.CountElements() == 4);
    ASSERT_TRUE(tree.GetHeight() == 4);
}

TEST_F(QuadtreeTest, Insert_SamePosition)
{
    tree.Insert(1, {50, 50});
    tree.Insert(2, {50, 50});
    ASSERT_TRUE(tree.CountElements() == 2);
}

TEST_F(QuadtreeTest, Insert_OutOfBounds)
{
    bool inserted = tree.Insert(1, {101, 101});
    ASSERT_FALSE(inserted);
    ASSERT_TRUE(tree.CountElements() == 0);
}

TEST_F(QuadtreeTest, Remove)
{
    tree.Insert(1, {25, 25});
    tree.Insert(2, {87, 87});
    tree.Insert(3, {56, 68});
    tree.Insert(4, {68, 56});
    
    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|_____|
    // |          |_3|__|     |
    // |__________|__|4_|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    ASSERT_TRUE(tree.CountElements() == 4);
    ASSERT_TRUE(tree.GetHeight() == 4);
    
    bool removed = tree.Remove(4, {68, 56});
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
    
    ASSERT_TRUE(tree.CountElements() == 3);
    ASSERT_TRUE(tree.GetHeight() == 3);
    
    removed = tree.Remove(3, {56, 68});
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
    
    ASSERT_TRUE(tree.CountElements() == 2);
    ASSERT_TRUE(tree.GetHeight() == 2);
    
    removed = tree.Remove(2, {87, 87});
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
    
    ASSERT_TRUE(tree.CountElements() == 1);
    ASSERT_TRUE(tree.GetHeight() == 1);
    
    removed = tree.Remove(1, {25, 25});
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
    
    ASSERT_TRUE(tree.CountElements() == 0);
    ASSERT_TRUE(tree.GetHeight() == 1);
}

TEST_F(QuadtreeTest, Remove_NotFound)
{
    bool removed = tree.Remove(1, {50, 50});
    ASSERT_FALSE(removed);
}

TEST_F(QuadtreeTest, FindAll)
{
    tree.Insert(1, {25, 25});
    tree.Insert(2, {87, 87});
    tree.Insert(3, {87, 68});
    tree.Insert(4, {56, 56});
    tree.Insert(5, {56, 68});
    tree.Insert(6, {68, 68});
    
    //  __________ ___________
    // |        ..|.....|. 2  |
    // |        . |_____|.____|
    // |        . |_5|_6|. 3  |
    // |________._|_4|__|.____|
    // |        ..|.......    |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    auto elements = tree.FindAll({40, 38}, {75, 88});
    ASSERT_TRUE(elements.size() == 3);
    ASSERT_TRUE(ContainsData(elements, 4));
    ASSERT_TRUE(ContainsData(elements, 5));
    ASSERT_TRUE(ContainsData(elements, 6));
}

TEST_F(QuadtreeTest, FindAll_Condition)
{
    tree.Insert(1, {25, 25});
    tree.Insert(2, {87, 87});
    tree.Insert(3, {87, 68});
    tree.Insert(4, {56, 56});
    tree.Insert(5, {56, 68});
    tree.Insert(6, {68, 68});
    
    //  __________ ___________
    // |        ..|.....|. 2  |
    // |        . |_____|.____|
    // |        . |_5|_6|. 3  |
    // |________._|_4|__|.____|
    // |        ..|.......    |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    auto isEven = [](const auto& element) { return element.data % 2 == 0; };
    auto elements = tree.FindAll({40, 38}, {75, 88}, isEven);
    ASSERT_TRUE(elements.size() == 2);
    ASSERT_TRUE(ContainsData(elements, 4));
    ASSERT_TRUE(ContainsData(elements, 6));
}

TEST_F(QuadtreeTest, FindNearest)
{
    tree.Insert(1, {25, 25});
    tree.Insert(2, {87, 87});
    tree.Insert(3, {87, 68});
    tree.Insert(4, {56, 56});
    tree.Insert(5, {56, 68});
    tree.Insert(6, {68, 68});
    
    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|x____|
    // |          |_5|_6|  3  |
    // |__________|_4|__|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    auto nearest = tree.FindNearest({75, 75});
    ASSERT_TRUE(nearest.value().data == 6);
}

TEST_F(QuadtreeTest, FindNearest_Condition)
{
    tree.Insert(1, {25, 25});
    tree.Insert(2, {87, 87});
    tree.Insert(3, {87, 68});
    tree.Insert(4, {56, 56});
    tree.Insert(5, {56, 68});
    tree.Insert(6, {68, 68});
    
    //  __________ ___________
    // |          |     |  2  |
    // |          |_____|x____|
    // |          |_5|_6|  3  |
    // |__________|_4|__|_____|
    // |          |           |
    // |    1     |           |
    // |          |           |
    // |__________|___________|
    
    auto isOdd = [](const auto& element) { return element.data % 2 == 1; };
    auto nearest = tree.FindNearest({75, 75}, isOdd);
    ASSERT_TRUE(nearest.value().data == 3);
}

TEST_F(QuadtreeTest, FindNearest_Single)
{
    tree.Insert(1, {25, 25});
    auto nearest = tree.FindNearest({50, 50});
    ASSERT_TRUE(nearest.value().data == 1);
}

TEST_F(QuadtreeTest, FindNearest_Empty)
{
    auto nearest = tree.FindNearest({50, 50});
    ASSERT_FALSE(nearest.has_value());
}

TEST_F(QuadtreeTest, FindNearest_OutOfBounds)
{
    tree.Insert(1, {25, 25});
    auto nearest = tree.FindNearest({101, 101});
    ASSERT_TRUE(nearest.has_value());
}

