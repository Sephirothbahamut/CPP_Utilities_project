#pragma once

#include "../common/begin.h"
#include "aabb.h"

namespace utils::math::details
	{
	template <class T, typename DT> geometry::aabb<false> vec_sized_specialization<T, 2, DT>::bounding_box() const noexcept { return {derived().x, derived().y, derived().x, derived().y}; }
	}