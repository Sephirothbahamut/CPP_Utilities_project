#pragma once

#include "../details/base_types.h"
#include "../../../../math/rect.h"

namespace utils::math::geometry::shape
	{
	struct aabb : utils::math::rect<float>, utils::math::geometry::shape::details::base<aabb, utils::math::geometry::shapes_enum::aabb>
		{
		utils_gpu_available constexpr aabb(auto... args) : utils::math::rect<float>{std::forward<decltype(args)>(args)...} {}
		};
	}