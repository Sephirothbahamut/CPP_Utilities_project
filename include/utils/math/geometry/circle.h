#pragma once

#include "../vec2.h"

namespace utils::math::geometry
	{
	class circle
		{
		public:
			vec2f center;
			float radius;

			aabb bounding_box() const noexcept
			{
				aabb ret
					{
					.ll = center.x - radius,
					.up = center.y - radius,
					.rr = center.x + radius,
					.dw = center.y + radius,
					};

				return ret;
			}
		};

	aabb bounding_box(const circle& from) { return from.bounding_box(); }
	}