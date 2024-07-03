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

        TEST_METHOD(Add_OutOfBounds)
        {
            const bool added = _tree.Add(1, Point{ 101, 101 });
            Assert::IsFalse(added);
            Assert::IsTrue(_tree.Size() == 0);
        }

    private:
        Quadtree _tree = { Point{50, 50}, 100 };
    };
}
