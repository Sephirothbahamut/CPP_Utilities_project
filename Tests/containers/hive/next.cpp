#include <vector>
#include <deque>
#include <list>
#include <algorithm>
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

struct disney 
	{
	inline static size_t count{0};
	int v;
	disney(int v) : v{v} { count++; }
	~disney() { count--; }

	static void reset() { count = 0; }
	};

namespace Tests
	{
	TEST_CLASS(next)
		{
		public:

			TEST_METHOD(emplace)
				{
				disney::reset();
				utils::containers::hive::next<disney> a;

				auto handle_0{a.emplace(0)};
				Assert::AreEqual(size_t{1}, disney::count);
				auto handle_1{a.emplace(1)};
				Assert::AreEqual(size_t{2}, disney::count);
				auto handle_2{a.emplace(2)};
				Assert::AreEqual(size_t{3}, disney::count);

				Assert::AreEqual(0, a[handle_0].v);
				Assert::AreEqual(1, a[handle_1].v);
				Assert::AreEqual(2, a[handle_2].v);
				}
			TEST_METHOD(destructor)
				{
				disney::reset();
				if (true)
					{
					utils::containers::hive::next<disney> a;
			
					auto handle_0{a.emplace(0)};
					auto handle_1{a.emplace(1)};
					auto handle_2{a.emplace(2)};
					Assert::AreEqual(size_t{3}, disney::count);
					}
				Assert::AreEqual(size_t{0}, disney::count);
				}
			TEST_METHOD(scramble)
				{
				disney::reset();
				utils::containers::hive::next<disney> a;
			
				auto handle_0{a.emplace(0)};
				auto handle_1{a.emplace(1)};
				auto handle_2{a.emplace(2)};
				Assert::AreEqual(size_t{3}, disney::count);
			
				a.remove(handle_1);
				Assert::AreEqual(size_t{2}, disney::count);
			
				auto handle_3{a.emplace(3)};
				Assert::AreEqual(3, a[handle_3].v);
				Assert::AreEqual(size_t{3}, disney::count);
				}
		};
	}