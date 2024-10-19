#include <utils/math/vec.h>


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
	}