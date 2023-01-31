#include <cmath> //NaN check

#include <utils/math/vec.h>
#include <utils/math/identity_numbers.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(memberwise_operators_array)
		{
		public:
			TEST_METHOD(constructors)
				{
				utils::math::vec<float, 3> a{1.f, 2.f};

				Assert::AreEqual(1.f, a[0]);
				Assert::AreEqual(2.f, a[1]);
				Assert::AreEqual(2.f, a[2]);

				utils::math::vec<float, 4> b(a, 2.f);
				Assert::AreEqual(a[0], b[0]);
				Assert::AreEqual(a[1], b[1]);
				Assert::AreEqual(a[2], b[2]);
				Assert::AreEqual( 2.f, b[3]);
				}

			TEST_METHOD(scalar_operators)
				{
				const utils::math::vec<float, 4> a{5.f, 5.f, 5.f, 5.f};
				const float b{2.f};

				utils::math::vec<float, 4> ret;

				ret = -a;
				Assert::AreEqual(-5.f, ret[0]);
				Assert::AreEqual(-5.f, ret[1]);
				Assert::AreEqual(-5.f, ret[2]);
				Assert::AreEqual(-5.f, ret[3]);

				ret = a + b;

				Assert::AreEqual(7.f, ret[0]);
				Assert::AreEqual(7.f, ret[1]);
				Assert::AreEqual(7.f, ret[2]);
				Assert::AreEqual(7.f, ret[3]);

				ret = a - b;

				Assert::AreEqual(3.f, ret[0]);
				Assert::AreEqual(3.f, ret[1]);
				Assert::AreEqual(3.f, ret[2]);
				Assert::AreEqual(3.f, ret[3]);

				ret = a * b;

				Assert::AreEqual(10.f, ret[0]);
				Assert::AreEqual(10.f, ret[1]);
				Assert::AreEqual(10.f, ret[2]);
				Assert::AreEqual(10.f, ret[3]);

				ret = a / b;

				Assert::AreEqual(2.5f, ret[0]);
				Assert::AreEqual(2.5f, ret[1]);
				Assert::AreEqual(2.5f, ret[2]);
				Assert::AreEqual(2.5f, ret[3]);
				}
			TEST_METHOD(scalar_assignment)
				{
				utils::math::vec<float, 4> a{5.f, 5.f, 5.f, 5.f};
				const float b{2.f};

				a += b;

				Assert::AreEqual(7.f, a[0]);
				Assert::AreEqual(7.f, a[1]);
				Assert::AreEqual(7.f, a[2]);
				Assert::AreEqual(7.f, a[3]);

				a -= b;

				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);
				Assert::AreEqual(5.f, a[3]);

				a *= b;

				Assert::AreEqual(10.f, a[0]);
				Assert::AreEqual(10.f, a[1]);
				Assert::AreEqual(10.f, a[2]);
				Assert::AreEqual(10.f, a[3]);

				a /= b;

				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);
				Assert::AreEqual(5.f, a[3]);
				}
			TEST_METHOD(array_operators)
				{
				const utils::math::vec<float, 4> a{5.f, 5.f, 5.f, 5.f};
				const utils::math::vec<float, 3> b{0.f, 1.f, 2.f};

				utils::math::vec<float, 4> ret;

				ret = a + b;

				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(6.f, ret[1]);
				Assert::AreEqual(7.f, ret[2]);
				Assert::AreEqual(5.f, ret[3]);

				ret = a - b;

				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(4.f, ret[1]);
				Assert::AreEqual(3.f, ret[2]);
				Assert::AreEqual(5.f, ret[3]);

				ret = a * b;

				Assert::AreEqual( 0.f, ret[0]);
				Assert::AreEqual( 5.f, ret[1]);
				Assert::AreEqual(10.f, ret[2]);
				Assert::AreEqual( 5.f, ret[3]);

				ret = a / b;

				Assert::IsTrue(std::isinf(ret[0]));
				Assert::AreEqual(5.f , ret[1]);
				Assert::AreEqual(2.5f, ret[2]);
				Assert::AreEqual(5.f , ret[3]);
				}
			TEST_METHOD(array_assignment)
				{
				utils::math::vec<float, 4> a{5.f, 5.f, 5.f, 5.f};
				utils::math::vec<float, 3> b{0.f, 1.f, 2.f};

				a += b;

				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(6.f, a[1]);
				Assert::AreEqual(7.f, a[2]);
				Assert::AreEqual(5.f, a[3]);

				a -= b;

				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);
				Assert::AreEqual(5.f, a[3]);

				a *= b;

				Assert::AreEqual( 0.f, a[0]);
				Assert::AreEqual( 5.f, a[1]);
				Assert::AreEqual(10.f, a[2]);
				Assert::AreEqual( 5.f, a[3]);

				a /= b;

				Assert::IsTrue(std::isnan(a[0]));
				Assert::AreEqual(5.f , a[1]);
				Assert::AreEqual(5.f, a[2]);
				Assert::AreEqual(5.f , a[3]);
				}
			TEST_METHOD(array_reverse_operators)
				{
				utils::math::vec<float, 3> a{5.f, 5.f, 5.f};
				utils::math::vec<float, 4> b{0.f, 1.f, 2.f, 5.f};

				utils::math::vec<float, 3> ret;

				ret = a + b;

				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(6.f, ret[1]);
				Assert::AreEqual(7.f, ret[2]);

				ret = a - b;

				Assert::AreEqual(5.f, ret[0]);
				Assert::AreEqual(4.f, ret[1]);
				Assert::AreEqual(3.f, ret[2]);

				ret = a * b;

				Assert::AreEqual( 0.f, ret[0]);
				Assert::AreEqual( 5.f, ret[1]);
				Assert::AreEqual(10.f, ret[2]);

				ret = a / b;

				Assert::IsTrue(std::isinf(ret[0]));
				Assert::AreEqual(5.f , ret[1]);
				Assert::AreEqual(2.5f, ret[2]);
				}
			TEST_METHOD(array_reverse_assignment)
				{
				utils::math::vec<float, 3> a{5.f, 5.f, 5.f};
				utils::math::vec<float, 4> b{0.f, 1.f, 2.f, 5.f};

				a += b;

				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(6.f, a[1]);
				Assert::AreEqual(7.f, a[2]);

				a -= b;

				Assert::AreEqual(5.f, a[0]);
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);

				a *= b;

				Assert::AreEqual( 0.f, a[0]);
				Assert::AreEqual( 5.f, a[1]);
				Assert::AreEqual(10.f, a[2]);

				a /= b;

				Assert::IsTrue(std::isnan(a[0]));
				Assert::AreEqual(5.f, a[1]);
				Assert::AreEqual(5.f, a[2]);
				}
			
			TEST_METHOD(minmax)
				{
				utils::math::vec<float, 2> a{2.f, 3.f};
				utils::math::vec<float, 3> b{3.f, 1.f};

				utils::math::vec<float, 2> ret;

				ret = utils::math::min(a, b);

				Assert::AreEqual(2.f, ret[0]);
				Assert::AreEqual(1.f, ret[1]);

				ret = utils::math::max(a, b);

				Assert::AreEqual(3.f, ret[0]);
				Assert::AreEqual(3.f, ret[1]);
				}
			TEST_METHOD(clamp)
				{
				utils::math::vec<float, 2> a{1.f, 5.f};
				utils::math::vec<float, 2> b{2.f, 1.f};
				utils::math::vec<float, 2> c{3.f, 3.f};

				utils::math::vec<float, 2> ret;

				ret = utils::math::clamp(a, b, c);

				Assert::AreEqual(2.f, ret[0]);
				Assert::AreEqual(3.f, ret[1]);
				}
			TEST_METHOD(lerp)
				{
				utils::math::vec<float, 2> a{2.f, 1.f};
				utils::math::vec<float, 3> b{2.f, 3.f};
				
				utils::math::vec<float, 2> ret;

				ret = utils::math::lerp(a, b, .5);

				Assert::AreEqual(2.f, ret[0]);
				Assert::AreEqual(2.f, ret[1]);

				ret = utils::math::lerp(a, b, 1.5);

				Assert::AreEqual(2.f, ret[0]);
				Assert::AreEqual(4.f, ret[1]);
				}
		};
	}
