#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include "../../include/utils/containers/multihandled_default.h"

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

//TODO moar complete tests

namespace Tests
	{
	TEST_CLASS(multihandled_default)
		{
		public:

			TEST_METHOD(assignment)
				{
				utils::containers::multihandled_default<int> a{-1};

				auto handle_1a{ a.emplace(1) };
				auto handle_2a{ a.emplace(2) };
				auto handle_3a{ a.emplace(3) };

				auto handle_1b{ a.clone_default() };

				Assert::AreEqual( 1, *handle_1a);
				Assert::AreEqual( 2, *handle_2a);
				Assert::AreEqual( 3, *handle_3a);
				Assert::AreEqual(-1, *handle_1b);
				}
			TEST_METHOD(remap)
				{
				utils::containers::multihandled_default<int> a;

				auto handle_1a{ a.emplace(1) };
				auto handle_2a{ a.emplace(2) };
				auto handle_3a{ a.emplace(3) };

				auto handle_1b{ a.clone_default() };
				a.remap_and_clear_target_handle(handle_1b, handle_2a);
				//auto handle_1b1{ handle_1b }; //TODO forbid copy in multihandled_default (don't forbid in multihandled_unsafe)

				Assert::AreEqual( 2, *handle_1b);
				Assert::AreEqual(-1, *handle_2a);
				}
		};
	}