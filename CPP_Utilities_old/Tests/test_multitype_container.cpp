#include "pch.h"

#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <utils/../../Beta/include/utils/containers/multitype_container.h>

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
struct pippo  : disney { pippo(float param) {} void hello() const {} };
struct pluto  : disney { pluto(char  param) {} void there() const {} };

namespace Tests
	{
	TEST_CLASS(Multitype_container)
		{
		public:

			TEST_METHOD(emplacement_vector)
				{
				utils::multitype_container<std::vector, int, pippo, pluto> a;

				a.emplace<pippo>(5.f);
				a.emplace<pluto>('c');
				a.emplace<int>(3);

				int counter{0};
				a.for_each_element_of_type<pippo>([&counter](const pippo& pippo) { pippo.hello(); counter++; });
				Assert::AreEqual(1, counter);

				counter = 0;
				a.for_each_element_of_type<disney>([&counter](const disney& adsqg) { adsqg.hallo(); counter++; });
				Assert::AreEqual(2, counter);
				}
			TEST_METHOD(emplacement_deque)
				{
				utils::multitype_container<std::deque, int, pippo, pluto> b;

				b.emplace<pippo>(5.f);
				b.emplace<pluto>('c');
				b.emplace<int>(3);

				int counter{0};
				b.for_each_element_of_type<pippo>([&counter](const pippo& pippo) { pippo.hello(); counter++; });

				Assert::AreEqual(1, counter);
				counter = 0;
				b.for_each_element_of_type<disney>([&counter](const disney& adsqg) { adsqg.hallo(); counter++; });
				Assert::AreEqual(2, counter);
}
			TEST_METHOD(emplacement_set)
				{
				utils::multitype_container<std::list, int, pippo, pluto> c;
				c.emplace<pippo>(5.f);
				c.emplace<pluto>('c');
				c.emplace<int>(3);

				int counter{0};
				c.for_each_element_of_type<pippo>([&counter](const pippo& pippo) { pippo.hello(); counter++; });
				Assert::AreEqual(1, counter);
				counter = 0;
				c.for_each_element_of_type<disney>([&counter](const disney& adsqg) { adsqg.hallo(); counter++; });
				Assert::AreEqual(2, counter);
				}
			TEST_METHOD(emplacement_inheritance)
				{
				utils::multitype_container<std::vector, pippo, disney> a;

				a.emplace<pippo>(5.f);
				a.emplace<disney>();

				int counter{0};
				Assert::AreEqual(size_t{1}, a.get_containing_type<pippo>().size());

				counter = 0;
				Assert::AreEqual(size_t{1}, a.get_containing_type<disney>().size());
				}
		};
	}