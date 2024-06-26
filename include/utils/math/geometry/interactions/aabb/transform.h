#pragma once

#include "base.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr auto& scale_self(shape::concepts::aabb auto& aabb, const float& scaling) noexcept 
		{
		aabb.size() *= scaling; 
		return aabb; 
		}

	utils_gpu_available constexpr auto& rotate_self(shape::concepts::aabb auto& aabb, const angle::concepts::angle auto& rotation) noexcept 
		{
		return aabb; 
		}

	utils_gpu_available constexpr auto& translate_self(shape::concepts::aabb auto& aabb, const vec2f& translation) noexcept 
		{
		aabb.pos() += translation; 
		return aabb; 
		}
	}