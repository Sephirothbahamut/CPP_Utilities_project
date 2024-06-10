#pragma once

#include "../ab.h"
#include "../aabb.h"
#include "../point.h"

namespace utils::math::geometry::shape::generic
	{
	template <storage::type storage_type>
	utils_gpu_available constexpr shape::aabb ab<storage_type>::bounding_box() const noexcept
		{
		return shape::aabb::from_vertices(a(), b());
		}
	}

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct shape::generic::ab     <storage::type::owner>;
		template struct shape::generic::line   <storage::type::owner>;
		template struct shape::generic::ray    <storage::type::owner>;
		template struct shape::generic::segment<storage::type::owner>;
		}
	namespace observer
		{
		template struct shape::generic::ab     <storage::type::observer>;
		template struct shape::generic::line   <storage::type::observer>;
		template struct shape::generic::ray    <storage::type::observer>;
		template struct shape::generic::segment<storage::type::observer>;
		}
	namespace const_observer
		{
		template struct shape::generic::ab     <storage::type::const_observer>;
		template struct shape::generic::line   <storage::type::const_observer>;
		template struct shape::generic::ray    <storage::type::const_observer>;
		template struct shape::generic::segment<storage::type::const_observer>;
		}
	}