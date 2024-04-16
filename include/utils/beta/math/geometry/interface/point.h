#pragma once

#include "../common.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_t>
	struct point : details::base<derived_t>
		{
		using crtp = details::base<derived_t>::crtp;
		
		};
	}

// Note: need to specify conversion consntructor cause inheriting parent constructors explicitely doesn't take in the copy constructor of the parent class.

namespace utils::math::geometry::shape
	{
	struct point : utils::math::geometry::shape::interface::point<point>, ::utils::math::vec2f 
		{
		using ::utils::math::vec2f::vec2f;
		utils_gpu_available point(const ::utils::math::vec2f& vec) : ::utils::math::vec2f{vec} {}

		using utils::math::geometry::shape::interface::point<point>::distance; //shape's distance takes precedence over vec2f's distance, which is however used internally.
		};

	namespace view
		{
		struct point : utils::math::geometry::shape::interface::point<point>, ::utils::math::vecref2f 
			{
			using ::utils::math::vecref2f::vecref2f; 
			utils_gpu_available point(const ::utils::math::vecref2f& vec) : ::utils::math::vecref2f{vec} {}

			using utils::math::geometry::shape::interface::point<point>::distance; //shape's distance takes precedence over vec2f's distance, which is however used internally.
			};
		}
	}