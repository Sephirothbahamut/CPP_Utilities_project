#pragma once

#include "../details/base_types.h"
#include "../../../../math/rect.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type>
		using aabb = ::utils::math::rect<utils::storage::storage_type_for<float, storage_type>>;
		}
	namespace concepts
		{
		template <typename T>
		concept aabb = std::same_as<T, shape::generic::aabb<T::storage_type>>;
		}

	namespace owner         { using aabb = shape::generic::aabb<storage::type::create::owner         ()>; }
	namespace observer      { using aabb = shape::generic::aabb<storage::type::create::observer      ()>; }
	namespace const_observer{ using aabb = shape::generic::aabb<storage::type::create::const_observer()>; }
	}