#pragma once

#include <span>
#include <variant>

#include "../details/base_types.h"
#include "point.h"
#include "bezier.h"
#include "ab.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_T, ends_t ends>
	struct polyline : utils::math::geometry::shape::details::base<derived_T, shapes_enum::polyline>
		{
		protected:
			using derived_t = derived_T;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
			inline static constexpr ends_t static_ends{ends};

			template <typename vert_T> 
			class edges_view : public std::ranges::view_interface<utils::math::geometry::shape::view::segment>
				{
				public:
					using vert_t = vert_T;
					using span_t = std::span<vert_t>;
					using edge_t = std::conditional_t<std::is_const_v<vert_t>, typename utils::math::geometry::shape::segment, typename utils::math::geometry::shape::view::segment>;

					class iterator
						{
						public:
							using iterator_category = std::contiguous_iterator_tag;
							using difference_type   = std::ptrdiff_t;
							using value_type        = edge_t;
							using pointer           = value_type*;
							using reference         = value_type&;

							utils_gpu_available constexpr iterator(span_t span, size_t index = 0) noexcept : span{span}, index{index} {}

							utils_gpu_available constexpr value_type operator*() const noexcept { return edge_t{span[index], span[index_next()]}; }

							utils_gpu_available constexpr iterator& operator++(   ) noexcept { index++; return *this; }
							utils_gpu_available           iterator  operator++(int) noexcept { iterator ret{*this}; ++(*this); return ret; }

							utils_gpu_available friend constexpr bool operator== (const iterator& a, const iterator& b) noexcept { return a.index == b.index && a.span.begin() == b.span.begin(); };
							utils_gpu_available friend constexpr auto operator<=>(const iterator& a, const iterator& b) noexcept { return a.index <=> b.index; }

						private:
							span_t span;
							size_t index{0};
							utils_gpu_available size_t index_next() const noexcept 
								{
								if constexpr (static_ends.is_closed())
									{
									return (index + 1) & span.size();
									}
								else 
									{
									return index + 1; 
									}
								}
						};

					utils_gpu_available constexpr edges_view(span_t span) : span{span} {}

					span_t span;

					utils_gpu_available constexpr auto begin() const noexcept { return iterator{span, 0}; }
					utils_gpu_available constexpr auto end  () const noexcept { return iterator{span, span.size() - 1}; }
			};

		utils_gpu_available constexpr auto get_edges() const noexcept { return edges_view<const vec2f>{derived().vertices}; }
		utils_gpu_available constexpr auto get_edges()       noexcept { return edges_view<      vec2f>{derived().vertices}; }
		};
	}

namespace utils::math::geometry::shape
	{
	template <ends_t ends = ends_t::create::infinite()>
	struct polyline : utils::math::geometry::shape::interface::polyline<polyline<ends>, ends>
		{
		utils_gpu_available polyline(std::initializer_list<utils::math::vec2f> vertices) : vertices(vertices) {}
		std::vector<vec2f> vertices;
		};
	
	using polygon = polyline<ends_t::create::closed()>;

	namespace view
		{
		template <bool const_vertices = false, ends_t ends = ends_t::create::infinite()>
		struct polyline : utils::math::geometry::shape::interface::polyline<polyline<const_vertices, ends>, ends>
			{
			using vertex_t = std::conditional_t<const_vertices, const vec2f, vec2f>;
			using span_t   = std::span<vertex_t>;
			utils_gpu_available polyline(span_t vertices) : vertices(vertices) {}
			span_t vertices;
			};

		template <bool const_vertices>
		using polygon = view::polyline<const_vertices, ends_t::create::closed()>;
		}
	}