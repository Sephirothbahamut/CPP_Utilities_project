#pragma once

#include "../details/base_types.h"
#include "../../../../math/rect.h"

namespace utils::math::geometry::shape
	{
	struct aabb : utils::math::rect<float>, utils::math::geometry::shape::details::base<aabb, utils::math::geometry::shapes_enum::aabb>
		{
		utils_gpu_available constexpr aabb(auto... args) : utils::math::rect<float>{std::forward<decltype(args)>(args)...} {}

		utils_gpu_available aabb merge(const aabb& other) const noexcept
			{
			return
				{
				std::min(this->ll, other.ll),
				std::min(this->up, other.up),
				std::max(this->rr, other.rr),
				std::max(this->dw, other.dw)
				};
			}
		};
	}