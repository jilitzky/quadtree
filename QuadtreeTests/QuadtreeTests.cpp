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
            _tree.Add(1, Point{ 25, 25 });
            Assert::IsTrue(_tree.Size() == 1);

            _tree.Add(2, Point{ 87, 87 });
            Assert::IsTrue(_tree.Size() == 2);

            _tree.Add(3, Point{ 56, 68 });
            Assert::IsTrue(_tree.Size() == 3);

            _tree.Add(4, Point{ 68, 56 });
            Assert::IsTrue(_tree.Size() == 4);
        }

        TEST_METHOD(Add_CannotSubdivide)
        {
            Quadtree tinyTree = { Point{1, 1}, 2 };

            tinyTree.Add(1, Point{ 0, 0 });
            Assert::IsTrue(tinyTree.Size() == 1);

            tinyTree.Add(2, Point{ 1, 1 });
            Assert::IsTrue(tinyTree.Size() == 2);
        }

        TEST_METHOD(Add_OutOfBounds)
        {
            const bool added = _tree.Add(1, Point{ 101, 101 });
            Assert::IsFalse(added);
            Assert::IsTrue(_tree.Size() == 0);
        }

        TEST_METHOD(Add_Overlapping)
        {
            _tree.Add(1, Point{ 50, 50 });
            Assert::IsTrue(_tree.Size() == 1);

            _tree.Add(2, Point{ 50, 50 });
            Assert::IsTrue(_tree.Size() == 2);

            _tree.Add(3, Point{ 50, 50 });
            Assert::IsTrue(_tree.Size() == 3);
        }

        TEST_METHOD(Remove)
        {
            _tree.Add(1, Point{ 50, 50 });
            Assert::IsTrue(_tree.Size() == 1);

            _tree.Add(2, Point{ 87, 87 });
            Assert::IsTrue(_tree.Size() == 2);

            _tree.Add(3, Point{ 56, 68 });
            Assert::IsTrue(_tree.Size() == 3);

            _tree.Add(4, Point{ 68, 56 });
            Assert::IsTrue(_tree.Size() == 4);

            bool removed = _tree.Remove(4, Point{ 68, 56 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 3);

            removed = _tree.Remove(3, Point{ 56, 68 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 2);

            removed = _tree.Remove(2, Point{ 87, 87 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 1);

            removed = _tree.Remove(1, Point{ 50, 50 });
            Assert::IsTrue(removed);
            Assert::IsTrue(_tree.Size() == 0);
            Assert::IsTrue(_tree.Root().Empty());
        }

        TEST_METHOD(Remove_NotFound)
        {
            bool removed = _tree.Remove(1, Point{ 50, 50 });
            Assert::IsFalse(removed);

            _tree.Add(1, Point{ 50, 50 });

            removed = _tree.Remove(2, Point{ 87, 87 });
            Assert::IsFalse(removed);

            _tree.Add(2, Point{ 87, 87 });

            removed = _tree.Remove(2, Point{ 56, 68 });
            Assert::IsFalse(removed);
        }

    private:
        Quadtree _tree = { Point{50, 50}, 100 };
    };
}
