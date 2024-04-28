#pragma once

#include "../details/base_types.h"
#include "../../../../math/vec2.h"

namespace utils::math::geometry::shape
	{
	struct point : utils::math::vec2f, utils::math::geometry::shape::details::base<point, utils::math::geometry::shapes_enum::point>
		{
		using utils::math::geometry::shape::details::base<point, utils::math::geometry::shapes_enum::point>::derived;
		utils_gpu_available constexpr point(auto... args) : utils::math::vec2f{std::forward<decltype(args)>(args)...} {}
		};

	namespace view
		{
		struct point : utils::math::vecref2f, utils::math::geometry::shape::details::base<point, utils::math::geometry::shapes_enum::point>
			{
			using utils::math::geometry::shape::details::base<point, utils::math::geometry::shapes_enum::point>::derived;

			utils_gpu_available constexpr point(auto... args) : utils::math::vecref2f{std::forward<decltype(args)>(args)...} {}
			};
		}
	}