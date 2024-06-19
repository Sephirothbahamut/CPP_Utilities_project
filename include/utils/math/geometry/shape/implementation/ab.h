#pragma once

#include "../ab.h"
#include "../aabb.h"
#include "../point.h"

namespace utils::math::geometry::shape::generic
	{
	template <storage::type storage_type>
	utils_gpu_available constexpr shape::aabb ab<storage_type>::bounding_box() const noexcept
		{
		return shape::aabb::create::from_vertices(a, b);
		}
	}

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct shape::generic::ab     <storage::type::create::owner()>;
		template struct shape::generic::line   <storage::type::create::owner()>;
		template struct shape::generic::ray    <storage::type::create::owner()>;
		template struct shape::generic::segment<storage::type::create::owner()>;
		}
	namespace observer
		{
		template struct shape::generic::ab     <storage::type::create::observer()>;
		template struct shape::generic::line   <storage::type::create::observer()>;
		template struct shape::generic::ray    <storage::type::create::observer()>;
		template struct shape::generic::segment<storage::type::create::observer()>;
		}
	namespace const_observer
		{
		template struct shape::generic::ab     <storage::type::create::const_observer()>;
		template struct shape::generic::line   <storage::type::create::const_observer()>;
		template struct shape::generic::ray    <storage::type::create::const_observer()>;
		template struct shape::generic::segment<storage::type::create::const_observer()>;
		}
	}