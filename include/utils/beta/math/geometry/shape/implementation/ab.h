#pragma once

#include "../ab.h"
#include "../aabb.h"
#include "../point.h"

namespace utils::math::geometry::shape::generic
	{
	template <storage::type storage_type, ends ends>
	utils_gpu_available constexpr ab<storage_type, ends>& ab<storage_type, ends>::scale_self    (const float      & scaling    ) noexcept
		{}

	template <storage::type storage_type, ends ends>
	utils_gpu_available constexpr ab<storage_type, ends>& ab<storage_type, ends>::rotate_self   (const angle::degf& rotation   ) noexcept
		{}

	template <storage::type storage_type, ends ends>
	utils_gpu_available constexpr ab<storage_type, ends>& ab<storage_type, ends>::translate_self(const vec2f      & translation) noexcept
		{}

	template <storage::type storage_type, ends ends>
	utils_gpu_available constexpr ab<storage_type, ends>& ab<storage_type, ends>::transform_self(const transform2 & transform  ) noexcept
		{}

	template <storage::type storage_type, ends ends>
	utils_gpu_available constexpr shape::owner::aabb ab<storage_type, ends>::bounding_box() const noexcept
		{}
	}