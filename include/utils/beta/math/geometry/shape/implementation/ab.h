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
		{
		return shape::owner::aabb
			{
			.ll{std::min(utils::remove_cvref_v(a.x), utils::remove_cvref_v(b.x))},
			.up{std::min(utils::remove_cvref_v(a.y), utils::remove_cvref_v(b.y))},
			.rr{std::max(utils::remove_cvref_v(a.x), utils::remove_cvref_v(b.x))},
			.dw{std::max(utils::remove_cvref_v(a.y), utils::remove_cvref_v(b.y))}
			};
		}
	}

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct shape::generic::ab<storage::type::owner, ends::create::open(false, false)>;
		template struct shape::generic::ab<storage::type::owner, ends::create::open(true , false)>;
		template struct shape::generic::ab<storage::type::owner, ends::create::open(true , true )>;
		}
	namespace observer
		{
		template struct shape::generic::ab<storage::type::observer, ends::create::open(false, false)>;
		template struct shape::generic::ab<storage::type::observer, ends::create::open(true , false)>;
		template struct shape::generic::ab<storage::type::observer, ends::create::open(true , true )>;
		}
	namespace const_observer
		{
		template struct shape::generic::ab<storage::type::const_observer, ends::create::open(false, false)>;
		template struct shape::generic::ab<storage::type::const_observer, ends::create::open(true , false)>;
		template struct shape::generic::ab<storage::type::const_observer, ends::create::open(true , true )>;
		}
	}