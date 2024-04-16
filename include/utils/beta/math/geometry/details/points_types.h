#pragma once
#include <vector>
#include <ranges>
#include <concepts>

#include "../common.h"
#include "../interface/segment.h"

namespace utils::math::geometry::shape::details
	{
	namespace points_types
		{
		template <typename derived_t>
		struct interface : details::base<derived_t>
			{
			using crtp = details::base<derived_t>;

			//template <typename vert_T> 
			class edges_view : public std::ranges::view_interface<utils::math::geometry::shape::view::segment>
				{
				public:
					using vert_t = utils::math::vec2f; //TODO deal with constness
					using span_t = std::span<vert_t>;
					using edge_t = utils::math::geometry::shape::view::segment;
					
					class iterator
						{
						public:
							using iterator_category = std::contiguous_iterator_tag;
							using difference_type   = std::ptrdiff_t;
							using value_type        = edge_t;
							using pointer           = value_type*;
							using reference         = value_type&;

							utils_gpu_available constexpr iterator(span_t span, size_t index = 0) noexcept : span{span} {}

							utils_gpu_available constexpr value_type operator*() const noexcept { return edge_t{span[index], span[index_next()]}; }

							utils_gpu_available constexpr iterator& operator++(   ) noexcept { index++; return *this; }
							utils_gpu_available iterator  operator++(int) noexcept { iterator ret{*this}; ++(*this); return ret; }

							utils_gpu_available friend constexpr bool operator== (const iterator& a, const iterator& b) noexcept { return a.index == b.index && a.span.begin() == b.span.begin(); };
							utils_gpu_available friend constexpr bool operator!= (const iterator& a, const iterator& b) noexcept { return a.index != b.index || a.span.begin() != b.span.begin(); };

						private:
							span_t span;
							size_t index{0};
							utils_gpu_available size_t index_next() const noexcept { return (index + 1) & span.size(); }
						};

					utils_gpu_available constexpr edges_view(span_t span) : span{span} {}

					span_t span;

					utils_gpu_available constexpr auto begin() const noexcept { return iterator{span, 0}; }
					utils_gpu_available constexpr auto end  () const noexcept { return iterator{span, span.size() - 1}; }

				private:
				};

			utils_gpu_available constexpr edges_view get_edges() const noexcept { return edges_view{derived_t::points}; }
			};

		struct data
			{
			utils_gpu_available data(std::initializer_list<utils::math::vec2f> points) : points{points} {}
			std::vector<utils::math::vec2f> points;
			utils_gpu_available constexpr const auto& get_vertices() const noexcept { return points; }
			};

		struct view
			{
			std::span<utils::math::vec2f> points;
			utils_gpu_available constexpr const auto& get_vertices() const noexcept { return points; }
			};
		}
	}