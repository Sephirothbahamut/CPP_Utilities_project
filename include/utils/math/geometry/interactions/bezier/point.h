#pragma once

#include "base.h"
#include "../point.h"
#include "../ab.h"

namespace utils::math::geometry::interactions
	{
	utils_gpu_available constexpr float closest_t(const shape::concepts::bezier auto& bezier, const vec2f& point, float t_min = 0.f, float t_max = 1.f) noexcept
		{//https://www.shadertoy.com/view/NdfSDl
		if (bezier.size())
			{
			const auto dot2{[](utils::math::vec2f v) -> float
				{
				return utils::math::vec2f::dot(v, v); 
				}};

			utils::math::vec2f c1 = point - bezier[0];
			utils::math::vec2f c2 = (bezier[1] * 2.f) - bezier[2] - bezier[0];
			utils::math::vec2f c3 = bezier[0] - bezier[1];

			// Cubic coefficients ---> t3*t^3 + t2*t^2 + t1*t + t0*t^0
			float t3 = utils::math::vec2f::dot(c2, c2);
			float t2 = utils::math::vec2f::dot(c3, c2) * 3.f;
			float t1 = utils::math::vec2f::dot(c1, c2) + 2.f * utils::math::vec2f::dot(c3, c3);
			float t0 = utils::math::vec2f::dot(c1, c3);

			// Reduce by dividing by leading coefficient
			// This simplifies out a lot of things
			t2 /= t3, t1 /= t3, t0 /= t3;

			// Depressed cubic coefficients (point and q) and precomputation
			float t22 = t2 * t2;
			utils::math::vec2f pq{t1 - t22 / 3.f, t22 * t2 / 13.5f - t2 * t1 / 3.f + t0};
			float ppp = pq.x() * pq.x() * pq.x(), qq = pq.y() * pq.y();

			float p2 = std::abs(pq.x());
			float r1 = 1.5f / pq.x() * pq.y();

			// Solutions and details gathered from here: https://en.wikipedia.org/wiki/Cubic_equation
			if (qq * 0.25f + ppp / 27.f > 0.f) 
				{ // One real root, use hyperbolic trig
				float r2 = r1 * std::sqrt(3.f / p2), root;
				if (pq.x() < 0.f) root = utils::math::sign(pq.y()) * std::cosh(std::acosh(r2 * -utils::math::sign(pq.y())) / 3.f);
				else root = std::sinh(std::asinh(r2) / 3.f);
				root = -2.f * std::sqrt(p2 / 3.f) * root - t2 / 3.f;
				root = clamp(root, t_min, t_max);
				//return utils::math::vec2f(length(point - posBezier(bezier[0], bezier[1], bezier[2], root)), root);
				return root;
				}
			else 
				{ // Three real roots (only need to use two), use "normal" trig
				float ac = std::acos(r1 * std::sqrt(-3.f / pq.x())) / 3.f; // 4pi/3 goes here --v
				//utils::math::vec2f roots = 2.f * std::sqrt(-pq.x() / 3.f) * std::cos(utils::math::vec2f(ac, ac - 4.18879020479f)) - t2 / 3.f;
				utils::math::vec2f roots = (utils::math::vec2f{float{std::cos(ac)}, float{std::cos(ac - 4.18879020479f)}} * 2.f * float{std::sqrt(-pq.x() / 3.f)}) - t2 / 3.f;
				roots = clamp(roots, t_min, t_max);
				float d1 = dot2(point - bezier.at(roots.x()).point());
				float d2 = dot2(point - bezier.at(roots.y()).point());
				//return d1 < d2 ? utils::math::vec2f(sqrt(d1), roots.x()) : utils::math::vec2f(sqrt(d2), roots.t);
				return d1 < d2 ? roots.x() : roots.y();
				}
			}
		}

	utils_gpu_available constexpr auto closest_proxy(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const float t{closest_t(bezier, point)};
		const auto proxy{bezier.at(t)};
		return proxy;
		}

	utils_gpu_available constexpr shape::point closest_point(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const auto proxy{closest_proxy(bezier, point)};
		return proxy.point();
		}
	
	utils_gpu_available constexpr float minimum_distance(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const auto proxy{closest_proxy(bezier, point)};
		return minimum_distance(proxy.point(), point);
		}
	
	utils_gpu_available constexpr return_types::closest_point_with_distance closest_with_distance(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const auto proxy{closest_proxy(bezier, point)};
		const float distance{minimum_distance(proxy.point(), point)};
		return {proxy.point(), distance};
		}
	
	utils_gpu_available constexpr return_types::side side(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const auto proxy{closest_proxy(bezier, point)};
		const auto left{proxy.normal()};
		const auto a_to_point{point - proxy.point()};
		const return_types::side ret{utils::math::vec2f::dot(left, a_to_point)};
		return ret;
		}

	utils_gpu_available constexpr return_types::signed_distance signed_distance(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const auto distance{minimum_distance(bezier, point)};
		const auto side{interactions::side(bezier, point)};
		return {distance * side.value()};
		}
	
	utils_gpu_available constexpr return_types::closest_point_with_signed_distance closest_with_signed_distance(const shape::concepts::bezier auto& bezier, const vec2f& point) noexcept
		{
		const auto proxy{closest_proxy(bezier, point)};
		const auto closest{proxy.point()};
		const float distance{minimum_distance(closest, point)};

		const auto left{proxy.normal()};
		const auto a_to_point{point - closest};
		const return_types::side side{utils::math::vec2f::dot(left, a_to_point)};

		const return_types::signed_distance signed_distance{distance * side};
		const return_types::closest_point_with_signed_distance ret{closest, signed_distance};
		return ret;
		}
	
	}