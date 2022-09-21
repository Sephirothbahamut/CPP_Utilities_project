#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "../../include/utils/containers/multihandled.h"

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
	TEST_CLASS(multihandled)
		{
		public:

			TEST_METHOD(assignment)
				{
				utils::containers::multihandled<int> a;

				auto handle_1a{ a.emplace(1) };
				auto handle_2a{ a.emplace(2) };
				auto handle_3a{ a.emplace(3) };
				
				auto handle_1b{ a.splice(handle_1a) };

				Assert::AreEqual(1, a[handle_1a]);
				Assert::AreEqual(2, a[handle_2a]);
				Assert::AreEqual(3, a[handle_3a]);
				Assert::AreEqual(1, a[handle_1b]);
				}
			TEST_METHOD(remap)
				{
				utils::containers::multihandled<int> a;

				auto handle_1a{ a.emplace(1) };
				auto handle_2a{ a.emplace(2) };
				auto handle_3a{ a.emplace(3) };

				auto handle_1b{ a.splice(handle_1a) };
				a.remap(handle_1b, handle_2a);
				auto handle_1b1{ handle_1b };

				Assert::AreEqual(2, a[handle_1b1]);
				}
		};
	}