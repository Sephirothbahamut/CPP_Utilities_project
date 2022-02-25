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
					.up = center.y - radius,
					.dw = center.y + radius,
					.rr = center.x - radius,
					.ll = center.x - radius,
					};

				return ret;
			}
		};
	}