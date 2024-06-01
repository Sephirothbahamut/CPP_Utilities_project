#pragma once

#include "../details/base_types.h"
#include "../../../../math/vec2.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type>
		using point = ::utils::math::vec<storage::single<storage_type, float>, 2>;
		}
	namespace concepts
		{
		template <typename T>
		concept point = std::same_as<T, shape::generic::point<T::static_storage_type>>;
		}

	namespace owner         { using point = shape::generic::point<storage::type::owner         >; }
	namespace observer      { using point = shape::generic::point<storage::type::observer      >; }
	namespace const_observer{ using point = shape::generic::point<storage::type::const_observer>; }
	}