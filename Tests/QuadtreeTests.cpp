#include <optional>
#include "CppUnitTest.h"
#include "Quadtree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QuadtreeTests
{
    TEST_CLASS(QuadtreeTests)
    {
    public:
        TEST_METHOD(Add)
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

            Assert::IsTrue(_tree.Size() == 0);
            Assert::IsTrue(_tree.Depth() == 1);

            _tree.Add(Point{ 25, 25 });

            //  ______________________
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |    *                 |
            // |                      |
            // |______________________|

            Assert::IsTrue(_tree.Size() == 1);
            Assert::IsTrue(_tree.Depth() == 1);

            _tree.Add(Point{ 87, 87 });

            //  __________ ___________
            // |          |        *  |
            // |          |           |
            // |          |           |
            // |__________|___________|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            Assert::IsTrue(_tree.Size() == 2);
            Assert::IsTrue(_tree.Depth() == 2);

            _tree.Add(Point{ 56, 68 });

            //  __________ ___________
            // |          |     |  *  |
            // |          |_____|_____|
            // |          | *   |     |
            // |__________|_____|_____|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            Assert::IsTrue(_tree.Size() == 3);
            Assert::IsTrue(_tree.Depth() == 3);

            _tree.Add(Point{ 68, 56 });

            //  __________ ___________
            // |          |     |  *  |
            // |          |_____|_____|
            // |          |_*|__|     |
            // |__________|__|*_|_____|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            Assert::IsTrue(_tree.Size() == 4);
            Assert::IsTrue(_tree.Depth() == 4);
        }

        TEST_METHOD(Add_CannotSubdivide)
        {
            Quadtree tinyTree(Point{ 0, 0 }, Point{ 1, 1 });

            tinyTree.Add(Point{ 0, 0 });
            Assert::IsTrue(tinyTree.Size() == 1);

            const bool added = tinyTree.Add(Point{ 1, 1 });
            Assert::IsFalse(added);
            Assert::IsTrue(tinyTree.Size() == 1);
        }

        TEST_METHOD(Add_OutOfBounds)
        {
            const bool added = _tree.Add(Point{ 101, 101 });
            Assert::IsFalse(added);
            Assert::IsTrue(_tree.Size() == 0);
        }

        TEST_METHOD(Add_Overlapping)
        {
            _tree.Add(Point{ 50, 50 });
            Assert::IsTrue(_tree.Size() == 1);

            const bool added = _tree.Add(Point{ 50, 50 });
            Assert::IsFalse(added);
            Assert::IsTrue(_tree.Size() == 1);
        }

        TEST_METHOD(FindNearest)
        {
            _tree.Add(Point{ 25, 25 });
            _tree.Add(Point{ 87, 87 });
            _tree.Add(Point{ 87, 68 });
            _tree.Add(Point{ 56, 56 });
            _tree.Add(Point{ 56, 68 });

            const Point expected = Point{ 68, 68 };
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

            std::optional<Point> nearest = _tree.FindNearest(Point{ 75, 75 });
            Assert::IsTrue(nearest.value() == expected);
        }

        TEST_METHOD(FindNearest_Single)
        {
            const Point expected{ 25, 25 };
            _tree.Add(expected);

            std::optional<Point> nearest = _tree.FindNearest(Point{ 50, 50 });
            Assert::IsTrue(nearest.value() == expected);
        }

        TEST_METHOD(FindNearest_Empty)
        {
            std::optional<Point> nearest = _tree.FindNearest(Point{ 50, 50 });
            Assert::IsFalse(nearest.has_value());
        }

        TEST_METHOD(FindNearest_OutOfBounds)
        {
            std::optional<Point> nearest = _tree.FindNearest(Point{ 101, 101 });
            Assert::IsFalse(nearest.has_value());
        }

        TEST_METHOD(Remove)
        {
            _tree.Add(Point{ 25, 25 });
            _tree.Add(Point{ 87, 87 });
            _tree.Add(Point{ 56, 68 });
            _tree.Add(Point{ 68, 56 });

            //  __________ ___________
            // |          |     |  *  |
            // |          |_____|_____|
            // |          |_*|__|     |
            // |__________|__|*_|_____|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            Assert::IsTrue(_tree.Size() == 4);
            Assert::IsTrue(_tree.Depth() == 4);

            bool removed = _tree.Remove(Point{ 68, 56 });
            Assert::IsTrue(removed);

            //  __________ ___________
            // |          |     |  *  |
            // |          |_____|_____|
            // |          | *   |     |
            // |__________|_____|_____|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            Assert::IsTrue(_tree.Size() == 3);
            Assert::IsTrue(_tree.Depth() == 3);

            removed = _tree.Remove(Point{ 56, 68 });
            Assert::IsTrue(removed);

            //  __________ ___________
            // |          |        *  |
            // |          |           |
            // |          |           |
            // |__________|___________|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            Assert::IsTrue(_tree.Size() == 2);
            Assert::IsTrue(_tree.Depth() == 2);

            removed = _tree.Remove(Point{ 87, 87 });
            Assert::IsTrue(removed);

            //  ______________________
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |    *                 |
            // |                      |
            // |______________________|

            Assert::IsTrue(_tree.Size() == 1);
            Assert::IsTrue(_tree.Depth() == 1);

            removed = _tree.Remove(Point{ 25, 25 });
            Assert::IsTrue(removed);

            //  ______________________
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |                      |
            // |______________________|

            Assert::IsTrue(_tree.Size() == 0);
            Assert::IsTrue(_tree.Depth() == 1);
        }

        TEST_METHOD(Remove_NotFound)
        {
            bool removed = _tree.Remove(Point{ 50, 50 });
            Assert::IsFalse(removed);
        }

    private:
        Quadtree _tree = { Point{ 0, 0 }, Point{ 100, 100 } };
    };
}
