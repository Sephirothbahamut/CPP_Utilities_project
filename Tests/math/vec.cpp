#include <cmath> //NaN check

#include <utils/math/vec.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(math_vec)
		{
		public:
			TEST_METHOD(constructors)
				{
				utils::math::vec<float, 2> zero_default;
				auto zero{utils::math::vec<float, 2>::zero()};

				Assert::AreEqual(0.f, zero_default[0]);
				Assert::AreEqual(0.f, zero_default[1]);
				Assert::AreEqual(0.f, zero        [0]);
				Assert::AreEqual(0.f, zero        [1]);

				auto a{utils::math::vec<float, 1>::right()};
				auto b{utils::math::vec<float, 2>::right()};

				auto c{utils::math::vec<float, 2>::down()};
				auto d{utils::math::vec<float, 3>::down()};

				auto e{utils::math::vec<float, 3>::forward()};
				auto f{utils::math::vec<float, 4>::forward()};

				Assert::AreEqual(1.f, a[0]);

				Assert::AreEqual(1.f, b[0]);
				Assert::AreEqual(0.f, b[1]);

				Assert::AreEqual(0.f, c[0]);
				Assert::AreEqual(1.f, c[1]);

				Assert::AreEqual(0.f, d[0]);
				Assert::AreEqual(1.f, d[1]);
				Assert::AreEqual(0.f, d[2]);

				Assert::AreEqual(0.f, e[0]);
				Assert::AreEqual(0.f, e[1]);
				Assert::AreEqual(1.f, e[2]);

				Assert::AreEqual(0.f, f[0]);
				Assert::AreEqual(0.f, f[1]);
				Assert::AreEqual(1.f, f[2]);
				Assert::AreEqual(0.f, f[3]);
				}
			TEST_METHOD(properties)
				{
				using namespace utils::math::operators;
				utils::math::vec<float, 4> a;

				a.x = 3;
				a.y = a.x;
				a.z = a.y;
				a.w = a.z;

				Assert::AreEqual(3.f, a[0]);
				Assert::AreEqual(3.f, a[1]);
				Assert::AreEqual(3.f, a[2]);
				Assert::AreEqual(3.f, a[3]);

				}
			TEST_METHOD(custom_operators)
				{
				using namespace utils::math::operators;
				utils::math::vec<float, 2> a{2.f, 1.f};
				utils::math::vec<float, 3> b{2.f, 1.f};

				utils::math::vec<float, 2> ret;

				float ret_f = a <dot> b;

				Assert::AreEqual(5.f, ret_f);

				ret = a <cross> b;

				Assert::AreEqual(4.f, ret[0]);
				Assert::AreEqual(1.f, ret[1]);
				}

			//TODO test the following:
			/*
			distance
			length
			slerp &co
			*/
		};
	}
