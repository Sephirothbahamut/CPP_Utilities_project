#pragma once

#include <vector>

#include "../../../index_range.h"
#include "../common/begin.h"
#include "aabb.h"

namespace utils::math::geometry
	{
	class polygon : public shape_base<polygon>
		{
		public:
			using vertices_t = std::vector<vec2f>;

			inline polygon(std::initializer_list<vec2f>&& vertices) : _vertices{ std::forward<std::initializer_list<vec2f>>(vertices) } {};

			inline polygon(const std::vector<vec2f>& vertices) : _vertices{vertices} {}
			inline polygon(std::vector<vec2f>&& vertices) : _vertices{std::move(vertices)} {}

			inline std::ranges::ref_view<      std::vector<vec2f>> get_vertices()       noexcept { return std::views::all(_vertices); };
			inline std::ranges::ref_view<const std::vector<vec2f>> get_vertices() const noexcept { return std::views::all(_vertices); };

			inline auto get_edges() noexcept
				{
				auto vertices{get_vertices()};
				vecref2f vr{vertices[0]};
				return utils::index_range{0, vertices.size(), [&vertices](size_t index) -> geometry::edge_ref { return {vertices[index], vertices[(index + 1) % vertices.size()]}; }};
				}
			inline auto get_edges() const noexcept
				{
				auto vertices{get_vertices()};
				return utils::index_range{0, vertices.size(), [&vertices](size_t index) -> geometry::segment { return {vertices[index], vertices[(index + 1) % vertices.size()]}; }};
				}

		public:
			inline aabb bounding_box() const noexcept
				{
				aabb ret
					{
					.ll{ constants::finf},
					.up{ constants::finf},
					.rr{-constants::finf},
					.dw{-constants::finf},
					};

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

		public:
			using shape_base<polygon>::closest_point_and_distance;
			using shape_base<polygon>::closest_point_to;
			using shape_base<polygon>::distance_min;
			using shape_base<polygon>::vector_to;
			using shape_base<polygon>::intersects;
			using shape_base<polygon>::intersection;
			using shape_base<polygon>::contains;
			using shape_base<polygon>::collides_with;

			closest_point_and_distance_t closest_point_and_distance(const point& other) const noexcept;
			bool                 intersects      (const point& other) const noexcept;
			std::optional<vec2f> intersection    (const point& other) const noexcept;
			bool                 contains        (const point& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const segment& other) const noexcept;
			bool                 intersects      (const segment& other) const noexcept;
			std::optional<vec2f> intersection    (const segment& other) const noexcept;
			bool                 contains        (const segment& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const aabb& other) const noexcept;
			bool                 intersects      (const aabb& other) const noexcept;
			std::optional<vec2f> intersection    (const aabb& other) const noexcept;
			bool                 contains        (const aabb& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const polygon& other) const noexcept;
			bool                 intersects      (const polygon& other) const noexcept;
			std::optional<vec2f> intersection    (const polygon& other) const noexcept;
			bool                 contains        (const polygon& other) const noexcept;

			closest_point_and_distance_t closest_point_and_distance(const circle& other) const noexcept;
			bool                 intersects      (const circle& other) const noexcept;
			std::optional<vec2f> intersection    (const circle& other) const noexcept;
			bool                 contains        (const circle& other) const noexcept;
	
			inline polygon& scale_self    (const float      & scaling    ) noexcept { for(auto& vertex : get_vertices()) { vertex.scale_self    (scaling    ); } return *this; }
			inline polygon& rotate_self   (const angle::radf& rotation   ) noexcept { for(auto& vertex : get_vertices()) { vertex.rotate_self   (rotation   ); } return *this; }
			inline polygon& translate_self(const vec2f      & translation) noexcept { for(auto& vertex : get_vertices()) { vertex.translate_self(translation); } return *this; }
		};

	class convex_polygon : public polygon
		{
		public:
			using derived_t = convex_polygon;
			using polygon::polygon;

			inline convex_polygon(std::initializer_list<vec2f>&& vertices) : polygon{std::forward<std::initializer_list<vec2f>>(vertices)} {};
			inline convex_polygon(const std::vector<vec2f>& vertices) : polygon{vertices} {}
			inline convex_polygon(      std::vector<vec2f>& vertices) : polygon{std::move(vertices)} {}

			using shape_base<polygon>::contains;

			bool contains(const point  & other) const noexcept;
			bool contains(const aabb   & other) const noexcept;
			bool contains(const polygon& other) const noexcept;
		};
	}