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

void swizzle_as_reference()
	{
	utils::math::vec4f vec4{1.f, 2.f, 3.f, 4.f};
	test_function(vec4.wy());

	assert(vec4[0] =  1.f);
	assert(vec4[1] = 22.f);
	assert(vec4[2] =  3.f);
	assert(vec4[3] = 14.f);

	utils::math::angle::degf angle{180.f};
	utils::math::vec2f v2f{1.f, 0.f};
	utils::math::transform2 transform{v2f, angle, 1.f};
	v2f += angle;
	const auto at{v2f * vec4};
	at + v2f;
	v2f.transform_self(transform);
	utils::math::rect<float> rect;
	rect.transform_self(transform);
	v2f.sdf(v2f).gradient_signed_distance();
	}