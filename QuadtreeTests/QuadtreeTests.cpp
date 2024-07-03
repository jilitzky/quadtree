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
            Quadtree tree = { Point{50, 50}, 100 };

            tree.Add(1, Point{ 25, 25 });
            Assert::IsTrue(tree.Size() == 1);

            tree.Add(2, Point{ 87, 87 });
            Assert::IsTrue(tree.Size() == 2);

            tree.Add(3, Point{ 56, 68 });
            Assert::IsTrue(tree.Size() == 3);

            tree.Add(4, Point{ 68, 56 });
            Assert::IsTrue(tree.Size() == 4);
        }

        TEST_METHOD(Add_CannotSubdivide)
        {
            Quadtree tree = { Point{1, 1}, 2 };

            tree.Add(1, Point{ 0, 0 });
            Assert::IsTrue(tree.Size() == 1);

            tree.Add(2, Point{ 1, 1 });
            Assert::IsTrue(tree.Size() == 2);
        }

        TEST_METHOD(Add_OutOfBounds)
        {
            Quadtree tree = { Point{50, 50}, 100 };

            const bool added = tree.Add(1, Point{ 101, 101 });
            Assert::IsFalse(added);
            Assert::IsTrue(tree.Size() == 0);
        }

        TEST_METHOD(Add_Overlapping)
        {
            Quadtree tree = { Point{50, 50}, 100 };

            tree.Add(1, Point{ 50, 50 });
            Assert::IsTrue(tree.Size() == 1);

            tree.Add(2, Point{ 50, 50 });
            Assert::IsTrue(tree.Size() == 2);

            tree.Add(3, Point{ 50, 50 });
            Assert::IsTrue(tree.Size() == 3);
        }
    };
}
