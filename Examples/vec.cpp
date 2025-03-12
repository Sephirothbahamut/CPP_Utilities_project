#include <utils/math/vec.h>
#include <utils/math/rect.h>
#include <utils/math/geometry/transform/aabb.h>
#include <utils/math/geometry/transform/point.h>
#include <utils/math/geometry/sdf/point.h>

void test_function(utils::math::vecref2f vecref)
	{
	vecref[0] += 10.f;
	vecref[1] += 20.f;
	}

void vec()
	{
	utils::math::vec4f vec4{1.f, 2.f, 3.f, 4.f};
	test_function(vec4.wy());

	assert(vec4[0] =  1.f);
	assert(vec4[1] = 22.f);
	assert(vec4[2] =  3.f);
	assert(vec4[3] = 14.f);

	utils::math::angle::degf angle{180.f};
	utils::math::vec2f v2f{1.f, 0.f};
	utils::math::vec2f v2f2{v2f};
	utils::math::vecref2f vr2f{v2f};
	const utils::math::vec2f vc2f{v2f};
	utils::math::vecref<const float, 2> vcr2f{v2f};
	utils::math::vecref<const float, 2> vcr2f2{vc2f};
	//vr2f = utils::math::vec3f{3.f, 9.f, 12.f};

	utils::storage::multiple<utils::storage::multiple<float, 2>> multest{utils::storage::multiple<float, 2>{1.f, 2.f}, utils::storage::multiple<float, 2>{3.f, 4.f}};
	utils::storage::multiple<float, 2> multest2{1.f, 2.f};
	multest2 = multest2;
	v2f = std::move(multest2);

	utils::math::transform2 transform{v2f, angle, 1.f};
	v2f += angle;
	const auto at{v2f * vec4};
	at + v2f;
	v2f.transform_self(transform);
	utils::math::rect<float> rect;
	rect.transform_self(transform);
	v2f.sdf(v2f).direction_signed_distance();
	}