#include "pch.h"

#include "../include/utils/id_pool.h"

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Tests
	{
	TEST_CLASS(Id_pool)
		{
		public:

			TEST_METHOD(operations)
				{
				using id_pool = utils::id_pool;
				using id_t = id_pool::id_t;

				id_pool pool;
				Assert::AreEqual(id_t{0}, pool.get());
				Assert::AreEqual(id_t{1}, pool.get());
				Assert::AreEqual(id_t{2}, pool.get());
				pool.release({0});
				pool.release({1});
				Assert::AreEqual(id_t{1}, pool.get());
				Assert::AreEqual(id_t{0}, pool.get());
				}
		};
	}