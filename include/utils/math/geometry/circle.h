#pragma once

#include "../vec2.h"

namespace utils::math::geometry
	{
	class circle
		{
		public:
			vec2f center;
			float radius;

			operator aabb() const noexcept
			{
				aabb ret
					{
					.ll = center.x - radius,
					.up = center.y - radius,
					.rr = center.x - radius,
					.dw = center.y + radius,
					};

				return ret;
			}
		};
	}