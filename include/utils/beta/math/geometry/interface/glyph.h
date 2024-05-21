#pragma once

#include <span>
#include <variant>

#include "../details/base_types.h"

#include "ab.h"
#include "bezier.h"
#include "polyline.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_T, ends_t ends>
	struct glyph : utils::math::geometry::shape::details::base<derived_T, shapes_enum::glyph>
		{
		protected:
			using derived_t = derived_T;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
			inline static constexpr ends_t static_ends{ends};

			template <auto callback>
			utils_gpu_available constexpr void for_each() const noexcept
				{
				const size_t index_vertex_last{static_ends.is_closed() ? derived().vertices.size() : derived().vertices.size() - 1};


				size_t index_curves_indices{0};
				size_t next_curve_index{std::numeric_limits<size_t>::max()};

				if (index_curves_indices < derived().curves_indices.size())
					{
					next_curve_index = derived().curves_indices[index_curves_indices];
					index_curves_indices++;
					}

				size_t index_vertex{0};

				while (index_vertex < index_vertex_last)
					{
					if (index_vertex == next_curve_index)
						{
						shape::bezier<2> curve;
						curve.control_points[0] = derived().vertices[index_vertex];
						curve.control_points[1] = derived().vertices[index_vertex + 1];
						curve.control_points[2] = derived().vertices[index_next(index_vertex + 1)];
						callback(curve);
						index_vertex += 2;

						if (index_curves_indices < derived().curves_indices.size())
							{
							next_curve_index = derived().curves_indices[index_curves_indices];
							index_curves_indices++;
							}
						else { next_curve_index = std::numeric_limits<size_t>::max(); }
						}
					else
						{
						const shape::segment segment{derived().vertices[index_vertex], derived().vertices[index_next(index_vertex)]};
						callback(segment);
						index_vertex++;
						}
					}
				};

		private:
			utils_gpu_available size_t index_next(size_t index) const noexcept
				{
				if constexpr (static_ends.is_closed()) { return (index + 1) % derived().vertices.size(); }
				else { return index + 1; }
				}
		};
	}

namespace utils::math::geometry::shape
	{
	template <ends_t ends = ends_t::create::closed()>
	struct glyph : utils::math::geometry::shape::interface::glyph<glyph<ends>, ends>
		{
		std::vector<vec2f> vertices;
		std::vector<size_t> curves_indices;
		};

	namespace view
		{
		template <bool const_vertices = false, ends_t ends = ends_t::create::infinite()>
		struct glyph : utils::math::geometry::shape::interface::glyph<glyph<const_vertices, ends>, ends>
			{
			using vertex_t = std::conditional_t<const_vertices, const vec2f, vec2f>;
			using span_t   = std::span<vertex_t>;
			using span_curves_indices_t = std::span<size_t>;
			utils_gpu_available glyph(span_t vertices, span_curves_indices_t curves_indices) : vertices(vertices), curves_indices(curves_indices) {}
			span_t vertices;
			span_curves_indices_t curves_indices;
			};
		}
	}