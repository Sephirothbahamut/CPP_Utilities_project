#include <vector>
#include <deque>
#include <list>
#include <algorithm>

#include "../../include/utils/containers/object_pool.h"
#include "../../include/utils/oop/counting.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using civ = utils::oop::counting_invalidating_move;

struct test_struct : civ
	{
	test_struct(int v) : v{v} {}
	int v;
	};

namespace Microsoft
	{
	namespace VisualStudio
		{
		namespace CppUnitTestFramework
			{
			}
		}
	}

namespace Tests
	{
	TEST_CLASS(object_pool_raw)
		{
		public:
			TEST_METHOD(insertion_and_destructor)
				{
				test_struct::reset();

				if (true)
					{
					utils::containers::object_pool<test_struct, 8, true, false, false> op;

					auto h0{op.emplace(0)};

					Assert::AreEqual(size_t{1}, test_struct::count());

					if (true)
						{
						auto h1{op.emplace(1)};

						Assert::AreEqual(size_t{2}, test_struct::count());
						}

					Assert::AreEqual(size_t{2}, test_struct::count());

					auto h2{op.emplace(2)};
					auto h3{op.emplace(3)};
					auto h4{op.emplace(4)};
					auto h5{op.emplace(5)};
					auto h6{op.emplace(6)};
					auto h7{op.emplace(7)};
					auto h8{op.emplace(8)};
					auto h9{op.emplace(9)};

					Assert::AreEqual(size_t{10}, test_struct::count());
					}

				Assert::AreEqual(size_t{0}, test_struct::count());
				}

			TEST_METHOD(shuffle)
				{
				test_struct::reset();

				utils::containers::object_pool<test_struct, 8, true, false, false> op;

				auto h0{op.emplace(0)};
				auto h1{op.emplace(1)};
				auto h2{op.emplace(2)};

				op.emplace(-1);
				op.emplace(-2);
				op.emplace(-3);

				Assert::AreEqual(size_t{6}, test_struct::count());

				auto h2_2{h2};

				auto address{&*h2};

				h2.reset();

				Assert::AreEqual(size_t{5}, test_struct::count());

				Assert::IsFalse(h2  .has_value());
				Assert::IsFalse(h2_2.has_value());

				auto h2_3{op.emplace(3)};

				Assert::AreEqual(static_cast<void*>(address), static_cast<void*>(&*h2_3));
				Assert::AreEqual(size_t{6}, test_struct::count());
				}
		};

	TEST_CLASS(object_pool_unique)
		{
		public:
			TEST_METHOD(insertion_and_destructor)
				{
				test_struct::reset();

				if (true)
					{
					utils::containers::object_pool<test_struct, 8, false, true, false> op;

					auto h0{op.make_unique(0)};

					Assert::AreEqual(size_t{1}, test_struct::count());

					if (true)
						{
						auto h1{op.make_unique(1)};

						Assert::AreEqual(size_t{2}, test_struct::count());
						}

					Assert::AreEqual(size_t{1}, test_struct::count());

					auto h1{op.make_unique(1)};
					auto h2{op.make_unique(2)};
					auto h3{op.make_unique(3)};
					auto h4{op.make_unique(4)};
					auto h5{op.make_unique(5)};
					auto h6{op.make_unique(6)};
					auto h7{op.make_unique(7)};
					auto h8{op.make_unique(8)};
					auto h9{op.make_unique(9)};

					Assert::AreEqual(size_t{10}, test_struct::count());
					}

				Assert::AreEqual(size_t{0}, test_struct::count());
				}
		};

	TEST_CLASS(object_pool_shared)
		{
		public:
			TEST_METHOD(insertion_and_destructor)
				{
				test_struct::reset();

				if (true)
					{
					utils::containers::object_pool<test_struct, 8, false, false, true> op;

					auto h0{op.make_shared(0)};

					Assert::AreEqual(size_t{1}, test_struct::count());

					if (true)
						{
						auto h1{op.make_shared(1)};

						Assert::AreEqual(size_t{2}, test_struct::count());
						}

					Assert::AreEqual(size_t{1}, test_struct::count());

					auto h1{op.make_shared(1)};
					auto h2{op.make_shared(2)};
					auto h3{op.make_shared(3)};
					auto h4{op.make_shared(4)};
					auto h5{op.make_shared(5)};
					auto h6{op.make_shared(6)};
					auto h7{op.make_shared(7)};
					auto h8{op.make_shared(8)};
					auto h9{op.make_shared(9)};

					Assert::AreEqual(size_t{10}, test_struct::count());
					}

				Assert::AreEqual(size_t{0}, test_struct::count());
				}
		};
	

	TEST_CLASS(object_pool_all)
		{
		public:
			TEST_METHOD(insertion_and_destructor)
				{
				test_struct::reset();

				if (true)
					{
					utils::containers::object_pool<test_struct, 8, true, true, true> op;

					auto h0{op.make_unique(0)};

					Assert::AreEqual(size_t{1}, test_struct::count());

					if (true)
						{
						auto h1{op.make_unique(1)};

						Assert::AreEqual(size_t{2}, test_struct::count());
						}

					Assert::AreEqual(size_t{1}, test_struct::count());

					auto uq1{op.make_unique(1)};
					auto uq2{op.make_unique(2)};
					auto uq3{op.make_unique(3)};

					auto rw4{op.emplace(4)};
					auto uq4{decltype(op)::handle_unique{rw4}};

					auto sh5{op.make_shared(5)};
					auto sh52{sh5};

					auto rw6{op.emplace(6)};
					auto sh6{decltype(op)::handle_shared{rw6}};
					auto sh62{sh6};

					auto uq7{op.make_unique(7)};
					auto uq8{op.make_unique(8)};
					auto uq9{op.make_unique(9)};

					Assert::AreEqual(size_t{10}, test_struct::count());
					}

				Assert::AreEqual(size_t{0}, test_struct::count());
				}
		};
	}