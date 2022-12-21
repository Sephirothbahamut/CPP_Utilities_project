#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "../include/utils/oop/counting.h"
#include "../../../include/utils/containers/hive/next.h"

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

struct test_struct : civ
	{
	test_struct(int v) : v{v} {}
	int v;
	};

namespace Tests
	{
	TEST_CLASS(next)
		{
		public:

			TEST_METHOD(emplace)
				{
				test_struct::reset();
				utils::containers::hive::next<test_struct> a;
			
				auto handle_0{a.emplace(0)};
				Assert::AreEqual(size_t{1}, test_struct::count());
				auto handle_1{a.emplace(1)};
				Assert::AreEqual(size_t{2}, test_struct::count());
				auto handle_2{a.emplace(2)};
				Assert::AreEqual(size_t{3}, test_struct::count());
			
				Assert::AreEqual(0, handle_0->v);
				Assert::AreEqual(1, handle_1->v);
				Assert::AreEqual(2, handle_2->v);
				}
			TEST_METHOD(destructor)
				{
				test_struct::reset();
				if (true)
					{
					utils::containers::hive::next<test_struct> a;
			
					auto handle_0{a.emplace(0)};
					auto handle_1{a.emplace(1)};
					auto handle_2{a.emplace(2)};
					Assert::AreEqual(size_t{3}, test_struct::count());
					}
				Assert::AreEqual(size_t{0}, test_struct::count());
				}
			TEST_METHOD(scramble)
				{
				test_struct::reset();
				utils::containers::hive::next<test_struct> a;
			
				auto handle_0{a.emplace(0)};
				auto handle_1{a.emplace(1)};
				auto handle_2{a.emplace(2)};
				Assert::AreEqual(size_t{3}, test_struct::count());
			
				a.erase(handle_1);
				Assert::AreEqual(size_t{2}, test_struct::count());
			
				auto handle_3{a.emplace(3)};
				Assert::AreEqual(3, handle_3->v);
				Assert::AreEqual(3, handle_1->v);
				Assert::AreEqual(size_t{3}, test_struct::count());
				}
			TEST_METHOD(iterate)
				{
				test_struct::reset();
				utils::containers::hive::next<test_struct> a;
			
				auto handle_0{a.emplace(0)};
				auto handle_1{a.emplace(1)};
				auto handle_2{a.emplace(2)};
			
				a.erase(handle_1);
			
				auto handle_3{a.emplace(3)};

				size_t i{0};
				for (const auto& element : a)
					{
					switch (i)
						{
						case 0: Assert::AreEqual(0, element.v); break;
						case 1: Assert::AreEqual(3, element.v); break;
						case 2: Assert::AreEqual(2, element.v); break;
						}
					i++;
					}
				}
		};
	}