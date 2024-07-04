#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "../details/base_types.h"
#include "ab.h"
#include "point.h"
#include "vertices.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type, ends ENDS, size_t extent = std::dynamic_extent>
		struct polyline : vertices<storage_type, ENDS, extent>
			{
			using vertices_t = vertices<storage_type, ENDS, extent>;
			using vertices_t::size;
			using vertices_t::ends;
			using vertices_t::vertices;
			using vertices_t::operator[];
			using typename vertices_t::value_type;
			using typename vertices_t::const_aware_value_type;

			using self_t = polyline<storage_type, ends, extent>;
			using nonref_self_t = polyline<storage::type::create::owner(), ends, extent>;

			template <bool is_function_const>
			using edge = utils::math::geometry::shape::generic::segment
				<
				(is_function_const || storage_type.is_const()) ? storage::type::create::const_observer() : storage::type::create::observer()
				>;
			template <bool is_function_const>
			using vertex = std::conditional_t
				<
				(is_function_const || storage_type.is_const()),
				const value_type, 
				const_aware_value_type
				>;

			template <bool is_view_const>
			struct edges_view : std::ranges::view_interface<edges_view<is_view_const>>, utils::oop::non_copyable, utils::oop::non_movable
				{
				using vert_t = vertex<is_view_const>;
			
				using polyline_t = std::conditional_t<is_view_const, const self_t, self_t>;
				polyline_t* polyline_ptr{nullptr};

				utils_gpu_available constexpr edges_view(polyline_t& polyline) : polyline_ptr{&polyline} {}

				template <bool is_iterator_const>
				struct iterator : std::conditional_t<ENDS.open, std::contiguous_iterator_tag, std::random_access_iterator_tag>
					{
					//TODO random access iterator implementation https://en.cppreference.com/w/cpp/iterator/random_access_iterator
					
					using polyline_t = std::conditional_t<is_view_const, const self_t, self_t>;
					polyline_t* polyline_ptr{nullptr};
					
					//using iterator_category = std::conditional_t<ENDS.open, std::contiguous_iterator_tag, std::random_access_iterator_tag>;
					using difference_type   = std::ptrdiff_t;
					using value_type        = edge<is_iterator_const>;
					using pointer           = value_type*;
					using reference         = value_type&;

					utils_gpu_available constexpr iterator() noexcept = default;
					utils_gpu_available constexpr iterator(polyline_t& polyline, size_t index = 0) noexcept : polyline_ptr{&polyline}, index{index} {}

					utils_gpu_available constexpr auto operator*() const noexcept { return edge<true             >{polyline_ptr->ends_aware_access(index), polyline_ptr->ends_aware_access(index + 1)}; }
					utils_gpu_available constexpr auto operator*()       noexcept { return edge<is_iterator_const>{polyline_ptr->ends_aware_access(index), polyline_ptr->ends_aware_access(index + 1)}; }
				
					utils_gpu_available constexpr iterator& operator++(   ) noexcept { index++; return *this; }
					utils_gpu_available constexpr iterator& operator--(   ) noexcept { index--; return *this; }
					utils_gpu_available constexpr iterator  operator++(int) noexcept { iterator ret{*this}; ++(*this); return ret; }
					utils_gpu_available constexpr iterator  operator--(int) noexcept { iterator ret{*this}; --(*this); return ret; }
				
					utils_gpu_available friend constexpr bool operator== (const iterator& a, const iterator& b) noexcept { return a.index == b.index && a.polyline_ptr == b.polyline_ptr; };
					utils_gpu_available friend constexpr auto operator<=>(const iterator& a, const iterator& b) noexcept { return a.index <=> b.index; }
					
					size_t index{0};
					};
				
				//static_assert(std::bidirectional_iterator<iterator<true>>); //TODO check why non copy constructible if storage inner container is span?
				//static_assert(std::random_access_iterator<iterator>);
				//static_assert(std::condiguous_iterator   <iterator>);
			
			
				utils_gpu_available constexpr auto operator[](const size_t& index) const noexcept                          { return edge<true         >{polyline_ptr->ends_aware_access(index), polyline_ptr->ends_aware_access(index + 1)}; }
				utils_gpu_available constexpr auto operator[](const size_t& index)       noexcept requires(!is_view_const) { return edge<is_view_const>{polyline_ptr->ends_aware_access(index), polyline_ptr->ends_aware_access(index + 1)}; }

				utils_gpu_available constexpr auto begin() const noexcept { return iterator<true         >{*polyline_ptr, 0     }; }
				utils_gpu_available constexpr auto begin()       noexcept { return iterator<is_view_const>{*polyline_ptr, 0     }; }
				utils_gpu_available constexpr auto end  () const noexcept { return iterator<true         >{*polyline_ptr, size()}; }
				utils_gpu_available constexpr auto end  ()       noexcept { return iterator<is_view_const>{*polyline_ptr, size()}; }
			
				utils_gpu_available constexpr bool   empty() const noexcept { return polyline_ptr->empty() || polyline_ptr->size() == 1; }
				utils_gpu_available constexpr size_t size () const noexcept { return polyline_ptr->ends_aware_size() - 1; }
				};
			
			/// <summary> 
			/// Usage note: This shape contains vertices, not edges. 
			/// The operator[] and dereferencing an iterator in the edges view will not return a reference to an edge, it will return an actual edge object which itself contains references to the vertices.
			/// So don't write `for(auto& edge : x.get_edges())`, write `for(auto edge : x.get_edges())` instead.
			/// If this shape is const or the edges view is stored in a const variable, edges will be const observers.
			/// Otherwise they will be simple observers, and changing them will affect the vertices of this shape.
			/// A lot of tears and blood were poured into making this seemingly seamless, it's part of the reason I restarted the geometry portion of this library from scratch at least 3 times, 
			/// please appreciate my efforts for such an useless feature nobody will ever need :)
			/// </summary>
			utils_gpu_available constexpr auto get_edges() const noexcept { return edges_view<true>{*this}; }

			/// <summary> 
			/// Usage note: This shape contains vertices, not edges. 
			/// The operator[] and dereferencing an iterator in the edges view will not return a reference to an edge, it will return an actual edge object which itself contains references to the vertices.
			/// So don't write `for(auto& edge : x.get_edges())`, write `for(auto edge : x.get_edges())` instead.
			/// If this shape is const or the edges view is stored in a const variable, edges will be const observers.
			/// Otherwise they will be simple observers, and changing them will affect the vertices of this shape.
			/// A lot of tears and blood were poured into making this seemingly seamless, it's part of the reason I restarted the geometry portion of this library from scratch at least 3 times, 
			/// please appreciate my efforts for such an useless feature nobody will ever need :)
			/// </summary>
			utils_gpu_available constexpr auto get_edges() noexcept { return edges_view<storage_type.is_const()>{*this}; }
			};

		template <storage::type storage_type, size_t extent = std::dynamic_extent>
		using polygon = polyline<storage_type, ends::create::closed(), extent>;
		}

	
	namespace concepts
		{
		template <typename T> concept polyline = std::derived_from<T, shape::generic::polyline<T::storage_type, T::ends, T::extent>>;
		template <typename T> concept polygon  = polyline<T> && T::ends == ends::create::closed();
		}
	
	namespace owner 
		{
		template <ends ends, size_t extent = std::dynamic_extent>
		using polyline = shape::generic::polyline<storage::type::create::owner(), ends, extent>;
		template <size_t extent = std::dynamic_extent>
		using polygon = shape::generic::polygon<storage::type::create::owner(), extent>;
		}
	namespace observer
		{
		template <ends ends, size_t extent = std::dynamic_extent>
		using polyline = shape::generic::polyline<storage::type::create::observer(), ends, extent>;
		template <size_t extent = std::dynamic_extent>
		using polygon = shape::generic::polygon<storage::type::create::observer(), extent>;
		}
	namespace const_observer
		{
		template <ends ends, size_t extent = std::dynamic_extent>
		using polyline = shape::generic::polyline<storage::type::create::const_observer(), ends, extent>;
		template <size_t extent = std::dynamic_extent>
		using polygon = shape::generic::polygon<storage::type::create::const_observer(), extent>;
		}
	}