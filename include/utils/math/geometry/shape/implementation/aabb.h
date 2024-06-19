#pragma once

#include "../aabb.h"
#include "../../../../../math/rect.h"

namespace utils::math::geometry::shape
	{
	namespace owner 
		{
		template struct math::rect<float>;
		template struct math::rect<float>;
		template struct math::rect<float>;
		}
	namespace observer
		{
		template struct math::rect<float&>;
		template struct math::rect<float&>;
		template struct math::rect<float&>;
		}
	namespace const_observer
		{
		template struct math::rect<const float&>;
		template struct math::rect<const float&>;
		template struct math::rect<const float&>;
		}
	}