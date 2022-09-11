#include "pch.h"

#include "../include/utils/id_pool.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Tests
	{
	TEST_CLASS(Id_pool)
		{
		public:

			TEST_METHOD(manual_management)
				{
				using id_pool = utils::id_pool_manual;

				id_pool pool;
				Assert::AreEqual(size_t{0}, pool.get());
				Assert::AreEqual(size_t{1}, pool.get());
				Assert::AreEqual(size_t{2}, pool.get());

				Assert::AreEqual(size_t{3}, pool.used());

				pool.release({0});
				pool.release({1});

				Assert::AreEqual(size_t{1}, pool.used());

				Assert::AreEqual(size_t{1}, pool.get());
				Assert::AreEqual(size_t{0}, pool.get());
				}

			TEST_METHOD(raii_version)
				{
				using id_pool = utils::id_pool;
				using id_t = utils::id_t;

				id_pool pool;

				if (true)
					{
					id_t first_id{pool};
					Assert::AreEqual(size_t{0}, first_id.value());
					id_t second_id{pool};
					Assert::AreEqual(size_t{1}, second_id.value());

					Assert::AreEqual(size_t{2}, pool.used());
					}
				Assert::AreEqual(size_t{0}, pool.used());
				}
		};
	}