#include <cmath> //NaN check

#include <utils/math/vec.h>
#include <utils/math/vec2.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
	{
	TEST_CLASS(math_vec)
		{
		public:
			TEST_METHOD(constructors)
				{
				static_assert(sizeof(utils::math::vec2f) == (sizeof(float) * 2));

				utils::math::vec2f    vec2f_a;
				vec2f_a.x() = vec2f_a.y() = 10.f;
				utils::math::vec2f    vec2f_b{1.f};
				utils::math::vec2f    vec2f_c{1.f, 2.f};
				utils::math::vec2f    vec2f_d{vec2f_a};
				utils::math::vec2f    vec2f_e{vec2f_a +  vec2f_b};
				utils::math::vec2f    vec2f_f{vec2f_a += vec2f_b};
				utils::math::vecref2f vec2f_g{vec2f_a};
				utils::math::vecref2f vec2f_h{vec2f_g *= 2.f};
				utils::math::vec2f    vec2f_i{vec2f_g};

				utils::math::vec2f::storage_t::inner_create(vec2f_a);
				utils::math::vec<float&, 2> vec2f_j{utils::math::vecref2f{vec2f_a}};
				utils::math::vec2f vec2f_jk{vec2f_j};

				utils::storage::multiple<utils::math::vec2f, std::dynamic_extent> sequential_owner{vec2f_a, vec2f_b, vec2f_c, vec2f_d};

				utils::math::vec<const float&, 2> asdfgh{vec2f_a};
				utils::math::vec2f zxcvbnm{asdfgh};

				utils::storage::multiple<utils::math::vec2f&, std::dynamic_extent, false> sequential_observer{sequential_owner};
	
				for (const auto& element : sequential_observer)
					{
					std::cout << "[" << float{element.x()} << ", " << float{element.y()} << "] ";
					};
				std::cout << std::endl;
				}

			TEST_METHOD(memory_layout_and_sizes)
				{
				utils::math::vec<float, 4> vec_a{5.f, 0.f, 0.f, 5.f};
				utils::math::vec<float, 3> vec_b{vec_a};
				utils::math::vec<float, 2> vec_c{3.f, 2.f};
				//utils::math::vec<float, 8> vec_d{vec_c, 1.f};
				auto vec_e{!vec_a};
				utils::math::vec<float, 8> vec_f{3.f, 2.f, 1.f, 0.f};

				Assert::AreEqual(sizeof(decltype(vec_a)), (sizeof(float) * 4));
				Assert::AreEqual(sizeof(decltype(vec_b)), (sizeof(float) * 3));
				Assert::AreEqual(sizeof(decltype(vec_c)), (sizeof(float) * 2));
				//Assert::AreEqual(sizeof(decltype(vec_d)), (sizeof(float) * 8));
				Assert::AreEqual(sizeof(decltype(vec_e)), (sizeof(float) * 4));
				Assert::AreEqual(sizeof(decltype(vec_f)), (sizeof(float) * 8));

				Assert::AreEqual(sizeof(decltype(vec_c + vec_b)), (sizeof(float) * std::max(decltype(vec_c)::extent, decltype(vec_b)::extent)));
				Assert::AreEqual(sizeof(decltype(vec_b + vec_c)), (sizeof(float) * std::max(decltype(vec_c)::extent, decltype(vec_b)::extent)));
				}
			TEST_METHOD(constructors)
				{
				utils::math::vec<float, 2> zero_default;
				auto zero{utils::math::vec<float, 2>::create::zero()};

				Assert::AreEqual(0.f, zero_default[0]);
				Assert::AreEqual(0.f, zero_default[1]);
				Assert::AreEqual(0.f, zero        [0]);
				Assert::AreEqual(0.f, zero        [1]);

				auto a{utils::math::vec<float, 1>::create::right()};
				auto b{utils::math::vec<float, 2>::create::right()};

				auto c{utils::math::vec<float, 2>::create::down()};
				auto d{utils::math::vec<float, 3>::create::down()};

				auto e{utils::math::vec<float, 3>::create::forward()};
				auto f{utils::math::vec<float, 4>::create::forward()};

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

				a.x() = 3;
				a.y() = a.x();
				a.z() = a.y();
				a.w() = a.z();

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
