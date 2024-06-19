#pragma once

#include "../point.h"
#include "../../../../../math/vec2.h"

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct math::vec<float, 2>;
		template struct math::vec<float, 2>;
		template struct math::vec<float, 2>;
		}
	namespace observer
		{
		template struct math::vec<float&, 2>;
		template struct math::vec<float&, 2>;
		template struct math::vec<float&, 2>;
		}
	namespace const_observer
		{
		template struct math::vec<const float&, 2>;
		template struct math::vec<const float&, 2>;
		template struct math::vec<const float&, 2>;
		}
	}