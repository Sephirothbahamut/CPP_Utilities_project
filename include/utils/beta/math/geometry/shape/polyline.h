#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "../details/base_types.h"
#include "../details/vertices_sequence.h"
#include "ab.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type, ends ENDS, size_t EXTENT>
		struct polyline : vertices_sequence<polyline<storage_type, ENDS, EXTENT>, storage_type, ENDS, EXTENT>
			{
			inline static constexpr size_t extent{EXTENT};
			inline static constexpr ends ends{ENDS};

			using self_t = polyline<storage_type, ends, extent>;
			using nonref_self_t = polyline<storage::type::create::owner(), ends, extent>;

			template <bool is_function_const>
			using edge_t = utils::math::geometry::shape::generic::segment
				<
				(is_function_const || storage_type.is_const()) ? storage::type::create::const_observer() : storage::type::create::observer()
				>;
			
			//template <typename edge_T>
			//class edges_view : public std::ranges::view_interface<edge_T>
			//	{
			//	public:
			//		using edge_t = edge_T;
			//		using vert_t = edge_t::vertex_observer<false>;
			//		using span_t = std::span<vert_t>;
			//
			//		class iterator
			//			{
			//			public:
			//				using iterator_category = std::contiguous_iterator_tag;
			//				using difference_type   = std::ptrdiff_t;
			//				using value_type        = edge_t;
			//				using pointer           = value_type*;
			//				using reference         = value_type&;
			//
			//				utils_gpu_available constexpr iterator(span_t span, size_t index = 0) noexcept : span{span}, index{index} {}
			//
			//				utils_gpu_available constexpr value_type operator*() const noexcept { return edge_t{span[index], span[index_next()]}; }
			//
			//				utils_gpu_available constexpr iterator& operator++() noexcept { index++; return *this; }
			//				utils_gpu_available           iterator  operator++(int) noexcept { iterator ret{*this}; ++(*this); return ret; }
			//
			//				utils_gpu_available friend constexpr bool operator== (const iterator& a, const iterator& b) noexcept { return a.index == b.index && a.span.begin() == b.span.begin(); };
			//				utils_gpu_available friend constexpr auto operator<=>(const iterator& a, const iterator& b) noexcept { return a.index <=> b.index; }
			//
			//			private:
			//				span_t span;
			//				size_t index{0};
			//				utils_gpu_available size_t index_next() const noexcept
			//					{
			//					if constexpr (ends.is_closed())
			//						{
			//						return (index + 1) % span.size();
			//						}
			//					else
			//						{
			//						return index + 1;
			//						}
			//					}
			//			};
			//
			//		utils_gpu_available constexpr edges_view(span_t span) : span{span} {}
			//
			//		span_t span;
			//
			//		utils_gpu_available constexpr auto begin() const noexcept { return iterator{span, 0}; }
			//		utils_gpu_available constexpr auto end  () const noexcept
			//			{
			//			if constexpr (ends.is_closed()) { return iterator{span, span.size()}; }
			//			else { return iterator{span, span.size() - 1}; }
			//			 }
			//	};
			//
			//utils_gpu_available constexpr auto get_edges() const noexcept { return edges_view<edge_t<true >>{storage_t::storage}; }
			//utils_gpu_available constexpr auto get_edges()       noexcept { return edges_view<edge_t<false>>{storage_t::storage}; }

			utils_gpu_available constexpr shape::aabb bounding_box() const noexcept
				{
				shape::aabb::create::from_vertices(*this);
				}
			};

		template <storage::type storage_type, size_t extent>
		using polygon = polyline<storage_type, ends::create::closed(), extent>;
		}

	
	namespace concepts
		{
		template <typename T> concept polyline = std::derived_from<T, shape::generic::polyline<T::storage_type, T::ends, T::extent>>;
		template <typename T> concept polygon  = polyline<T> && T::ends == ends::create::closed();
		}
	
	namespace owner 
		{
		template <ends ends, size_t extent>
		using polyline = shape::generic::polyline<storage::type::create::owner(), ends, extent>;
		template <size_t extent>
		using polygon = shape::generic::polyline<storage::type::create::owner(), ends::create::closed(), extent>;
		}
	namespace observer
		{
		template <ends ends, size_t extent>
		using polyline = shape::generic::polyline<storage::type::create::observer(), ends, extent>;
		template <size_t extent>
		using polygon = shape::generic::polyline<storage::type::create::observer(), ends::create::closed(), extent>;
		}
	namespace const_observer
		{
		template <ends ends, size_t extent>
		using polyline = shape::generic::polyline<storage::type::create::const_observer(), ends, extent>;
		template <size_t extent>
		using polygon = shape::generic::polyline<storage::type::create::const_observer(), ends::create::closed(), extent>;
		}
	}