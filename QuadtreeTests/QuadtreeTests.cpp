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
            //  __________ ___________
            // |          |     |  *  |
            // |          |_____|_____|
            // |          |_*|__|     |
            // |__________|__|*_|_____|
            // |          |           |
            // |    *     |           |
            // |          |           |
            // |__________|___________|

            _tree.Add(Point{ 25, 25 });
            Assert::IsTrue(_tree.Size() == 1);

            _tree.Add(Point{ 87, 87 });
            Assert::IsTrue(_tree.Size() == 2);

            _tree.Add(Point{ 56, 68 });
            Assert::IsTrue(_tree.Size() == 3);

            _tree.Add(Point{ 68, 56 });
            Assert::IsTrue(_tree.Size() == 4);
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

        TEST_METHOD(FindNearest_Empty)
        {
            const Point* point = _tree.FindNearest(Point{ 50, 50 });
            Assert::IsTrue(point == nullptr);
        }

        TEST_METHOD(FindNearest_OutOfBounds)
        {
            const Point* point = _tree.FindNearest(Point{ 101, 101 });
            Assert::IsTrue(point == nullptr);
        }

        TEST_METHOD(Remove)
        {
            _tree.Add(Point{ 50, 50 });
            Assert::IsTrue(_tree.Size() == 1);

            _tree.Add(Point{ 87, 87 });
            Assert::IsTrue(_tree.Size() == 2);

            _tree.Add(Point{ 56, 68 });
            Assert::IsTrue(_tree.Size() == 3);

            _tree.Add(Point{ 68, 56 });
            Assert::IsTrue(_tree.Size() == 4);

            bool removed = _tree.Remove(Point{ 68, 56 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 3);

            removed = _tree.Remove(Point{ 56, 68 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 2);

            removed = _tree.Remove(Point{ 87, 87 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 1);

            removed = _tree.Remove(Point{ 50, 50 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 0);
            Assert::IsTrue(_tree.Root().Empty());
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
