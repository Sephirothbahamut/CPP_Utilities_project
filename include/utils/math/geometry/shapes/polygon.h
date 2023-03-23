#pragma once

#include <vector>

#include "../common/begin.h"
#include "aabb.h"

namespace utils::math::geometry
	{
	template <bool view>
	class polygon : public shape_with_vertices_base<polygon<view>>
		{
		private:
			using shape_with_vertices_base<polygon<view>>::get_edges;
		public:
			using vertices_t = std::conditional_t<view, std::span<vec2f>, std::vector<vec2f>>;

			polygon(std::initializer_list<vec2f>&& vertices) requires(view) : _vertices{ std::forward<std::initializer_list<vec2f>>(vertices) } {};

			polygon(const std::vector<vec2f>& vertices) requires(view) : _vertices{vertices} {}
			polygon(std::vector<vec2f>&& vertices) requires(view) : _vertices{std::move(vertices)} {}

			std::ranges::ref_view<      std::vector<vec2f>> get_vertices()       noexcept { return std::views::all(_vertices); };
			std::ranges::ref_view<const std::vector<vec2f>> get_vertices() const noexcept { return std::views::all(_vertices); };

		public:
			aabb<false> bounding_box() const noexcept
				{
				aabb<false> ret
					{{
					.ll{ constants::finf},
					.up{ constants::finf},
					.rr{-constants::finf},
					.dw{-constants::finf},
					}};

				for(const auto& vertex : get_vertices())
					{
					ret.left  = std::min(ret.left,  vertex.x);
					ret.up    = std::min(ret.up,    vertex.y);
					ret.right = std::max(ret.right, vertex.x);
					ret.down  = std::max(ret.down,  vertex.y);
					}

				return ret;
				}

		private:
			vertices_t _vertices;
				
			using shape_base<polygon>::closest_point_and_distance;
			using shape_base<polygon>::closest_point_to;
			using shape_base<polygon>::distance_min;
			using shape_base<polygon>::vector_to;
			using shape_base<polygon>::intersects;
			using shape_base<polygon>::intersection;
			using shape_base<polygon>::contains;
			using shape_base<polygon>::collision;

			template <bool view> closest_point_and_distance_t closest_point_and_distance(const point<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const point<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const point<view>& other) const noexcept;
			template <bool view> bool                 contains        (const point<view>& other) const noexcept;

			template <bool view> closest_point_and_distance_t closest_point_and_distance(const segment<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const segment<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const segment<view>& other) const noexcept;
			template <bool view> bool                 contains        (const segment<view>& other) const noexcept;

			template <bool view> closest_point_and_distance_t closest_point_and_distance(const aabb<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const aabb<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const aabb<view>& other) const noexcept;
			template <bool view> bool                 contains        (const aabb<view>& other) const noexcept;

			template <bool view> closest_point_and_distance_t closest_point_and_distance(const polygon<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const polygon<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const polygon<view>& other) const noexcept;
			template <bool view> bool                 contains        (const polygon<view>& other) const noexcept;

			template <bool view> closest_point_and_distance_t closest_point_and_distance(const circle<view>& other) const noexcept;
			template <bool view> bool                 intersects      (const circle<view>& other) const noexcept;
			template <bool view> std::optional<vec2f> intersection    (const circle<view>& other) const noexcept;
			template <bool view> bool                 contains        (const circle<view>& other) const noexcept;
		};

	template <bool view>
	struct convex_polygon : polygon<view>
		{
		using polygon<view>;
		convex_polygon(std::initializer_list<vec2f>&& vertices) : polygon{std::forward<std::initializer_list<vec2f>>(vertices)} {};
		convex_polygon(const std::vector<vec2f>& vertices) : polygon{vertices} {}
		convex_polygon(      std::vector<vec2f>& vertices) : polygon{std::move(vertices)} {}

		using shape_base<polygon<view>>::contains;

		template <bool view> bool contains(const point  <view>& other) const noexcept;
		template <bool view> bool contains(const aabb   <view>& other) const noexcept;
		template <bool view> bool contains(const polygon<view>& other) const noexcept;
		};
	}

#include "../common/with_vertices_end.h"