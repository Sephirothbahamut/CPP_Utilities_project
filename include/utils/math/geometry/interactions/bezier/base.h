#pragma once

#include "../base_types.h"
#include "../../shape/bezier.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr return_types::bounding_box bounding_box(const shape::concepts::bezier auto& bezier) noexcept
		{
		assert(bezier.size() == 3 || bezier.size() == 4);
		//https://iquilezles.org/articles/bezierbbox/
		
		// extremes
		utils::math::vec2f min{utils::math::min(bezier[0], bezier[bezier.size() - 1])};
		utils::math::vec2f max{utils::math::max(bezier[0], bezier[bezier.size() - 1])};

		if (bezier.size() == 3)
			{
			if (bezier[1].x() < min.x() || bezier[1].x() > max.x() || bezier[1].y() < min.y() || bezier[1].y() > max.y())
				{
				const vec2 t{clamp((bezier[0] - bezier[1]) / (bezier[0] - 2.f * bezier[1] + bezier[2]), 0.f, 1.f)};
				const vec2 s{1.f - t};
				const vec2 q{s * s * bezier[0] + 2.f * s * t * bezier[1] + t * t * bezier[2]};
				min = utils::math::min(min, q);
				max = utils::math::max(max, q);
				}
			}
		else if (bezier.size() == 4)
			{
			const utils::math::vec2f k0{-1.f * bezier[0] + 1.f * bezier[1]};
			const utils::math::vec2f k1{+1.f * bezier[0] - 2.f * bezier[1] + 1.f * bezier[2]};
			const utils::math::vec2f k2{-1.f * bezier[0] + 3.f * bezier[1] - 3.f * bezier[2] + 1.f * bezier[3]};
			
			const utils::math::vec2f h{k1 * k1 - k0 * k2};

			if (h.x() > 0.f)
				{
				h.x() = sqrt(h.x());
				//float t = ( - k1.x() - h.x()) / k2.x();
				float t{k0.x() / (-k1.x() - h.x())};
				if (t > 0.f && t < 1.f)
					{
					const float s{1.f - t};
					const float q{s * s * s * bezier[0].x() + 3.f * s * s * t * bezier[1].x() + 3.f * s * t * t * bezier[2].x() + t * t * t * bezier[3].x()};
					min.x() = utils::math::min(min.x(), q);
					max.x() = utils::math::max(max.x(), q);
					}
					//t = ( - k1.x() + h.x()) / k2.x();
				t = k0.x() / (-k1.x() + h.x());
				if (t > 0.f && t < 1.f)
					{
					const float s{1.f - t};
					const float q{s * s * s * bezier[0].x() + 3.f * s * s * t * bezier[1].x() + 3.f * s * t * t * bezier[2].x() + t * t * t * bezier[3].x()};
					min.x() = utils::math::min(min.x(), q);
					max.x() = utils::math::max(max.x(), q);
					}
				}

			if (h.y() > 0.f)
				{
				h.y() = sqrt(h.y());
				//float t = ( - k1.y() - h.y()) / k2.y();
				float t{k0.y() / (-k1.y() - h.y())};
				if (t > 0.f && t < 1.f)
					{
					float s = 1.f - t;
					float q = s * s * s * bezier[0].y() + 3.f * s * s * t * bezier[1].y() + 3.f * s * t * t * bezier[2].y() + t * t * t * bezier[3].y();
					min.y() = utils::math::min(min.y(), q);
					max.y() = utils::math::max(max.y(), q);
					}
					//t = ( - k1.y() + h.y()) / k2.y();
				t = k0.y() / (-k1.y() + h.y());
				if (t > 0.f && t < 1.f)
					{
					float s = 1.f - t;
					float q = s * s * s * bezier[0].y() + 3.f * s * s * t * bezier[1].y() + 3.f * s * t * t * bezier[2].y() + t * t * t * bezier[3].y();
					min.y() = utils::math::min(min.y(), q);
					max.y() = utils::math::max(max.y(), q);
					}
				}
			}

		return return_types::bounding_box::create::from_ul_dr(min, max);
		}
	}