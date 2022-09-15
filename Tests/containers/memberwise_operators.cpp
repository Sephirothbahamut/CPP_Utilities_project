/*#include <cmath> //NaN check

#include <utils/containers/memberwise_operators.h>
#include <utils/math/identity_numbers.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(memberwise_operators)
		{
		public:
			
			TEST_METHOD(vec_deque_operators)
				{
				using namespace utils::containers::memberwise_operators;
				const std::vector<float> a{5.f, 5.f, 5.f, 5.f};
				const std::vector<float> b{0.f, 1.f, 2.f};

				std::vector<float> ret;

				ret = a + b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(6.f, ret[1]);
				Assert::AreEqual(7.f, ret[2]);
				Assert::AreEqual(5.f, ret[3]);

				ret = a - b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(4.f, ret[1]);
				Assert::AreEqual(3.f, ret[2]);
				Assert::AreEqual(5.f, ret[3]);

				ret = a * b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::AreEqual( 0.f, ret[0]);
				Assert::AreEqual( 5.f, ret[1]);
				Assert::AreEqual(10.f, ret[2]);
				Assert::AreEqual( 5.f, ret[3]);

				ret = a / b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::IsTrue(std::isinf(ret[0]));
				Assert::AreEqual(5.f , ret[1]);
				Assert::AreEqual(2.5f, ret[2]);
				Assert::AreEqual(5.f , ret[3]);
				}
			TEST_METHOD(vec_deque_assignment)
				{
				using namespace utils::containers::memberwise_operators;
				std::vector<float> a{5.f, 5.f, 5.f, 5.f};
				std::vector<float> b{0.f, 1.f, 2.f};

				a += b;

				Assert::AreEqual(size_t{4}, a.size());
				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(6.f, a[1]);
				Assert::AreEqual(7.f, a[2]);
				Assert::AreEqual(5.f, a[3]);

				a -= b;

				Assert::AreEqual(size_t{4}, a.size());
				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);
				Assert::AreEqual(5.f, a[3]);

				a *= b;

				Assert::AreEqual(size_t{4}, a.size());
				Assert::AreEqual( 0.f, a[0]);
				Assert::AreEqual( 5.f, a[1]);
				Assert::AreEqual(10.f, a[2]);
				Assert::AreEqual( 5.f, a[3]);

				a /= b;

				Assert::AreEqual(size_t{4}, a.size());
				Assert::IsTrue(std::isnan(a[0]));
				Assert::AreEqual(5.f , a[1]);
				Assert::AreEqual(5.f, a[2]);
				Assert::AreEqual(5.f , a[3]);
				}
			TEST_METHOD(vec_deque_reverse_operators)
				{
				using namespace utils::containers::memberwise_operators;
				std::vector<float> a{5.f, 5.f, 5.f};
				std::vector<float> b{0.f, 1.f, 2.f, 5.f};

				std::vector<float> ret;

				ret = a + b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(6.f, ret[1]);
				Assert::AreEqual(7.f, ret[2]);

				ret = a - b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(4.f, ret[1]);
				Assert::AreEqual(3.f, ret[2]);

				ret = a * b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::AreEqual( 0.f, ret[0]);
				Assert::AreEqual( 5.f, ret[1]);
				Assert::AreEqual(10.f, ret[2]);

				ret = a / b;

				Assert::AreEqual(a.size(), ret.size());
				Assert::IsTrue(std::isinf(ret[0]));
				Assert::AreEqual(5.f , ret[1]);
				Assert::AreEqual(2.5f, ret[2]);
				}
			TEST_METHOD(vec_deque_reverse_assignment)
				{
				using namespace utils::containers::memberwise_operators;
				std::vector<float> a{5.f, 5.f, 5.f};
				std::vector<float> b{0.f, 1.f, 2.f, 5.f};

				a += b;

				Assert::AreEqual(size_t{3}, a.size());
				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(6.f, a[1]);
				Assert::AreEqual(7.f, a[2]);

				a -= b;

				Assert::AreEqual(size_t{3}, a.size());
				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);

				a *= b;

				Assert::AreEqual(size_t{3}, a.size());
				Assert::AreEqual( 0.f, a[0]);
				Assert::AreEqual( 5.f, a[1]);
				Assert::AreEqual(10.f, a[2]);

				a /= b;

				Assert::AreEqual(size_t{3}, a.size());
				Assert::IsTrue(std::isnan(a[0]));
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);
				}
		};
	}
	*/