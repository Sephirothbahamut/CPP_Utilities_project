#pragma once

#include "../details/base_types.h"
#include "point.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_T>
	struct circle : details::base<derived_T, shapes_enum::circle>
		{
		protected:
			using derived_t = derived_T;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
			utils_gpu_available constexpr view ::point center_point()       noexcept { return {derived().center}; }
			utils_gpu_available constexpr shape::point center_point() const noexcept { return {derived().center}; }
		};
	}

namespace utils::math::geometry::shape
	{
	struct circle : utils::math::geometry::shape::interface::circle<circle>
		{
		circle(utils::math::vec2f center, float radius) : center{center}, radius{radius} {}
		::utils::math::vec2f center;
		float radius;
		};

	namespace view
		{
		struct circle : utils::math::geometry::shape::interface::circle<circle>
			{
			::utils::math::vecref2f center;
			std::reference_wrapper<float> radius;
			};
		}
	}
