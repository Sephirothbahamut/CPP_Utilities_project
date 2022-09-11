#include "pch.h"

#include <vector>
#include <algorithm>
#include "../Beta/include/utils/deferred_RAII.h"
#include <iostream>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

size_t count{0};
struct SomeClass
	{
	bool moved_from = false;
	SomeClass() { count++; }
	SomeClass(const SomeClass& copy) { count++; }
	SomeClass& operator=(const SomeClass& copy) { count++; }
	SomeClass(SomeClass&& move) noexcept { move.moved_from = true; }
	SomeClass& operator=(SomeClass&& move) noexcept { move.moved_from = true; }
	~SomeClass() { if (!moved_from) { count--; } }
	};

namespace Tests
	{
	TEST_CLASS(Deferred)
		{
		public:
			TEST_METHOD(manual_flush)
				{
				utils::deferred_deleter<SomeClass> deleter;
				if (true)
					{
					utils::deferred_wrapper<SomeClass> instance{deleter};
					auto moved_instance = std::move(instance);
					auto copied_instance = instance;

					Assert::AreEqual(size_t(2), count);
					}

				Assert::AreEqual(size_t(2), count);
				deleter.flush();
				Assert::AreEqual(size_t(0), count);
				}

			TEST_METHOD(deleter_destructor)
				{
				if (true)
					{
					utils::deferred_deleter<SomeClass> deleter;

					utils::deferred_wrapper<SomeClass> instance{deleter};

					Assert::AreEqual(size_t(1), count);
					}
				Assert::AreEqual(size_t(0), count);
				}
		};
	}