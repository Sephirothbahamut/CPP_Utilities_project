#pragma once

#include "../details/base_types.h"
#include "../../../../math/rect.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type>
		using aabb = ::utils::math::rect<storage::single<storage_type, float, false>>;
		}
	namespace concepts
		{
		template <typename T>
		concept aabb = std::same_as<T, shape::generic::aabb<T::static_storage_type>>;
		}
	
	namespace owner         { using aabb = shape::generic::aabb<storage::type::owner         >; }
	namespace observer      { using aabb = shape::generic::aabb<storage::type::observer      >; }
	namespace const_observer{ using aabb = shape::generic::aabb<storage::type::const_observer>; }
	}