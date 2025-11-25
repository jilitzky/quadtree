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

            Assert::IsTrue(_tree.Size() == 1);
            Assert::IsTrue(_tree.Depth() == 1);

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

            Assert::IsTrue(_tree.Size() == 2);
            Assert::IsTrue(_tree.Depth() == 2);

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

            Assert::IsTrue(_tree.Size() == 3);
            Assert::IsTrue(_tree.Depth() == 3);

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

            Assert::IsTrue(_tree.Size() == 4);
            Assert::IsTrue(_tree.Depth() == 4);
        }

        TEST_METHOD(Add_CannotSubdivide)
        {
            Quadtree tinyTree({ 0, 0 }, { 1, 1 });

            tinyTree.Add({ 0, 0 });
            Assert::IsTrue(tinyTree.Size() == 1);

            const bool added = tinyTree.Add({ 1, 1 });
            Assert::IsFalse(added);
            Assert::IsTrue(tinyTree.Size() == 1);
        }

        TEST_METHOD(Add_OutOfBounds)
        {
            const bool added = _tree.Add({ 101, 101 });
            Assert::IsFalse(added);
            Assert::IsTrue(_tree.Size() == 0);
        }

        TEST_METHOD(Add_Overlapping)
        {
            _tree.Add({ 50, 50 });
            Assert::IsTrue(_tree.Size() == 1);

            const bool added = _tree.Add({ 50, 50 });
            Assert::IsFalse(added);
            Assert::IsTrue(_tree.Size() == 1);
        }

        TEST_METHOD(FindNearest)
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
            Assert::IsTrue(nearest.value() == expected);
        }

        TEST_METHOD(FindNearest_Single)
        {
            const Vector2 expected = { 25, 25 };
            _tree.Add(expected);

            std::optional<Vector2> nearest = _tree.FindNearest({ 50, 50 });
            Assert::IsTrue(nearest.value() == expected);
        }

        TEST_METHOD(FindNearest_Empty)
        {
            std::optional<Vector2> nearest = _tree.FindNearest({ 50, 50 });
            Assert::IsFalse(nearest.has_value());
        }

        TEST_METHOD(FindNearest_OutOfBounds)
        {
            std::optional<Vector2> nearest = _tree.FindNearest({ 101, 101 });
            Assert::IsFalse(nearest.has_value());
        }

        TEST_METHOD(Remove)
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

            Assert::IsTrue(_tree.Size() == 4);
            Assert::IsTrue(_tree.Depth() == 4);

            bool removed = _tree.Remove({ 68, 56 });
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

            removed = _tree.Remove({ 56, 68 });
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

            removed = _tree.Remove({ 87, 87 });
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

            removed = _tree.Remove({ 25, 25 });
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
            bool removed = _tree.Remove({ 50, 50 });
            Assert::IsFalse(removed);
        }

    private:
        Quadtree _tree = { { 0, 0 }, { 100, 100 } };
    };
}
