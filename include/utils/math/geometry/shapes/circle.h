#pragma once

#include "../common/begin.h"
#include "aabb.h"

namespace utils::math::geometry
	{
	template <bool view>
	class circle : shape_base<circle<view>>
		{
		public:
			using vertex_t = std::conditional_t<view, vecref2f, vec2f>;
			using scalar_t = std::conditional_t<view, std::reference_wrapper<float>, float>;
			vertex_t center;
			scalar_t radius;

			aabb<false> bounding_box() const noexcept
				{
				aabb<false> ret
					{{
					.ll{center.x - radius},
					.up{center.y - radius},
					.rr{center.x + radius},
					.dw{center.y + radius},
					}};

				return ret;
				}
			
			using shape_base<circle<view>>::closest_point_and_distance;
			using shape_base<circle<view>>::closest_point_to;
			using shape_base<circle<view>>::distance_min;
			using shape_base<circle<view>>::vector_to;
			using shape_base<circle<view>>::intersects;
			using shape_base<circle<view>>::intersection;
			using shape_base<circle<view>>::contains;
			using shape_base<circle<view>>::collision;
			
			template <bool view> vec2f closest_point_to(const point<view>& other) const noexcept;
			template <bool view> bool  contains        (const point<view>& other) const noexcept;

			template <bool view> vec2f                closest_point_to(const segment<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const segment<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const segment<view>& other) const noexcept;
			template <bool view> bool                 contains        (const segment<view>& other) const noexcept;

			template <bool view> vec2f                closest_point_to(const aabb<view>& other) const noexcept;
			template <bool view> vec2f                distance_min    (const aabb<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const aabb<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const aabb<view>& other) const noexcept;
			template <bool view> bool                 contains        (const aabb<view>& other) const noexcept;

			template <bool view> vec2f                closest_point_to(const polygon<view>& other) const noexcept;
			template <bool view> vec2f                distance_min    (const polygon<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const polygon<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const polygon<view>& other) const noexcept;
			template <bool view> bool                 contains        (const polygon<view>& other) const noexcept;

			template <bool view> vec2f                closest_point_to(const circle<view>& other) const noexcept;
			template <bool view> vec2f                distance_min    (const circle<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const circle<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const circle<view>& other) const noexcept;
			template <bool view> bool                 contains        (const circle<view>& other) const noexcept;
		};
	}