#pragma once

#include "common.h"

namespace utils::math::geometry
	{
	template <bool hollow>
	struct aabb : public rect<float>, shape_base<aabb<hollow>, hollow>
		{
		aabb bounding_box() const noexcept { return this; }
		};
	}