#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "../include/utils/oop/counting.h"

#include "CppUnitTest.h"

namespace Microsoft
	{
	namespace VisualStudio
		{
		namespace CppUnitTestFramework
			{
			}
		}
	}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using civ = utils::oop::counting_invalidating_move;

namespace Tests
	{
	TEST_CLASS(counting)
		{
		public:

			TEST_METHOD(count)
				{
				utils::oop::counting::reset();
				Assert::AreEqual(size_t{0}, utils::oop::counting::count());

				utils::oop::counting a;
				Assert::AreEqual(size_t{1}, utils::oop::counting::count());

				utils::oop::counting b;
				Assert::AreEqual(size_t{2}, utils::oop::counting::count());

				if (true)
					{
					utils::oop::counting c;
					Assert::AreEqual(size_t{3}, utils::oop::counting::count());

					b = c;
					Assert::AreEqual(size_t{3}, utils::oop::counting::count());

					a = std::move(c);
					Assert::AreEqual(size_t{3}, utils::oop::counting::count());
					}

				Assert::AreEqual(size_t{2}, utils::oop::counting::count());
				}
			TEST_METHOD(count_invalidating_move)
				{
				civ::reset();
				Assert::AreEqual(size_t{0}, civ::count());

				civ a;
				Assert::AreEqual(size_t{1}, civ::count());

				civ b;
				Assert::AreEqual(size_t{2}, civ::count());

				if (true)
					{
					civ c;
					Assert::AreEqual(size_t{3}, civ::count());

					b = c;
					Assert::AreEqual(size_t{3}, civ::count());

					a = std::move(c);

					Assert::IsTrue (a.valid());
					Assert::IsFalse(c.valid());
					Assert::AreEqual(size_t{2}, civ::count());
					}

				Assert::IsTrue(a.valid());
				Assert::IsTrue(b.valid());

				if (true)
					{
					civ c;
					Assert::AreEqual(size_t{3}, civ::count());

					b = c;
					Assert::AreEqual(size_t{3}, civ::count());

					c = std::move(a);

					Assert::IsFalse(a.valid());
					Assert::IsTrue (c.valid());
					Assert::AreEqual(size_t{2}, civ::count());
					}

				Assert::IsFalse(a.valid());
				Assert::IsTrue (b.valid());
				Assert::AreEqual(size_t{1}, civ::count());
				}
		};
	}