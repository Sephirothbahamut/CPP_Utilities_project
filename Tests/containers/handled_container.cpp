#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "../../include/utils/containers/handled_container.h"

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

struct disney { void hallo() const {} };
struct pippo : disney { pippo(float param) {} void hello() const {} };
struct pluto : disney { pluto(char  param) {} void there() const {} };

namespace Tests
	{
	TEST_CLASS(Handled_container)
		{
		public:

			TEST_METHOD(assignment)
				{
				utils::containers::handled_container<int> a;

				auto handle_1{a.emplace(1)};
				auto handle_2{a.emplace(2)};
				auto handle_3{a.emplace(3)};

				Assert::AreEqual(1, a[handle_1]);
				Assert::AreEqual(2, a[handle_2]);
				Assert::AreEqual(3, a[handle_3]);
				}
			TEST_METHOD(scramble)
				{
				utils::containers::handled_container<int> a;

				auto handle_1{a.emplace(1)};
				auto handle_2{a.emplace(2)};
				auto handle_3{a.emplace(3)};

				a.remove(handle_2);

				auto handle_4{a.emplace(4)};

				Assert::AreEqual(1, a[handle_1]);
				Assert::AreEqual(4, a[handle_4]);
				Assert::AreEqual(3, a[handle_3]);
				}
		};
	}