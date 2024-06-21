#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "../details/base_types.h"
#include "ab.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type, ends ENDS, size_t EXTENT = std::dynamic_extent>
		struct polyline : utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, EXTENT, true>
			{
			inline static constexpr ends   ends  {ENDS  };
			inline static constexpr size_t extent{EXTENT};

			using storage_t = utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, EXTENT, true>;
			using storage_t::multiple;
			using typename storage_t::value_type;
			using typename storage_t::const_aware_value_type;

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
			struct edges_view : std::ranges::view_interface<edges_view<is_view_const>>
				{
				using vert_t = vertex<is_view_const>;
			
				using span_t = std::span<vert_t, extent>;

				template <bool is_iterator_const>
				class iterator : std::conditional_t<ENDS.open, std::contiguous_iterator_tag, std::random_access_iterator_tag>
					{
					//TODO random access iterator implementation https://en.cppreference.com/w/cpp/iterator/random_access_iterator
					public:
						//using iterator_category = std::conditional_t<ENDS.open, std::contiguous_iterator_tag, std::random_access_iterator_tag>;
						using difference_type   = std::ptrdiff_t;
						using value_type        = edge<is_view_const || is_iterator_const>;
						using pointer           = value_type*;
						using reference         = value_type&;
			
						utils_gpu_available constexpr iterator() noexcept = default;
						utils_gpu_available constexpr iterator(span_t span, size_t index = 0) noexcept : span{span}, index{index} {}

						utils_gpu_available constexpr edge<true > operator*() const noexcept                                                { return edge<true >{span[index], span[index_next()]}; }
						utils_gpu_available constexpr edge<false> operator*()       noexcept requires(!is_view_const && !is_iterator_const) { return edge<false>{span[index], span[index_next()]}; }
				
						utils_gpu_available constexpr iterator& operator++(   ) noexcept { index++; return *this; }
						utils_gpu_available constexpr iterator& operator--(   ) noexcept { index--; return *this; }
						utils_gpu_available constexpr iterator  operator++(int) noexcept { iterator ret{*this}; ++(*this); return ret; }
						utils_gpu_available constexpr iterator  operator--(int) noexcept { iterator ret{*this}; --(*this); return ret; }
				
						utils_gpu_available friend constexpr bool operator== (const iterator& a, const iterator& b) noexcept { return a.index == b.index && a.span.begin() == b.span.begin(); };
						utils_gpu_available friend constexpr auto operator<=>(const iterator& a, const iterator& b) noexcept { return a.index <=> b.index; }
				
					private:
						span_t span;
						size_t index{0};
						utils_gpu_available size_t index_next() const noexcept
							{
							if constexpr (ends.is_closed())
								{
								return (index + 1) % span.size();
								}
							else
								{
								return index + 1;
								}
							}
					};
				
				static_assert(std::bidirectional_iterator<iterator<true>>);
				//static_assert(std::random_access_iterator<iterator>);
				//static_assert(std::condiguous_iterator   <iterator>);
			
				template <typename ...Args>
				utils_gpu_available constexpr edges_view(Args&&... args) : span{std::forward<Args>(args)...} {}
				
				span_t span;
			
				utils_gpu_available const edge<true > operator[](const size_t& index) const noexcept                          { return edge<true >{span[index], span[index_next(index)]}; }//////////////////// some constness mess here
				utils_gpu_available       edge<false> operator[](const size_t& index)       noexcept requires(!is_view_const) { return edge<false>{span[index], span[index_next(index)]}; }

				utils_gpu_available constexpr auto begin() const noexcept                          { return iterator<true >{span, 0}; }
				utils_gpu_available constexpr auto begin()       noexcept requires(!is_view_const) { return iterator<false>{span, 0}; }
				utils_gpu_available constexpr auto end  () const noexcept
					{
					if constexpr (ends.is_closed()) { return iterator<true>{span, span.size()}; }
					else { return iterator{span, span.size() - 1}; }
					}
				utils_gpu_available constexpr auto end() noexcept requires(!is_view_const)
					{
					if constexpr (ends.is_closed()) { return iterator<false>{span, span.size()}; }
					else { return iterator{span, span.size() - 1}; }
					}
			
				utils_gpu_available constexpr size_t size() const noexcept
					{
					if constexpr (ends.is_closed()) { return span.size(); }
					else { return span.empty() ? 0 : span.size() - 1; }
					}
			
				utils_gpu_available size_t index_next(size_t index) const noexcept
					{
					if constexpr (ends.is_closed())
						{
						return (index + 1) % span.size();
						}
					else
						{
						return index + 1;
						}
					}
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
			utils_gpu_available constexpr auto get_edges() const noexcept { return edges_view<true                   >{storage_t::storage.begin(), storage_t::storage.size()}; }

			/// <summary> 
			/// Usage note: This shape contains vertices, not edges. 
			/// The operator[] and dereferencing an iterator in the edges view will not return a reference to an edge, it will return an actual edge object which itself contains references to the vertices.
			/// So don't write `for(auto& edge : x.get_edges())`, write `for(auto edge : x.get_edges())` instead.
			/// If this shape is const or the edges view is stored in a const variable, edges will be const observers.
			/// Otherwise they will be simple observers, and changing them will affect the vertices of this shape.
			/// A lot of tears and blood were poured into making this seemingly seamless, it's part of the reason I restarted the geometry portion of this library from scratch at least 3 times, 
			/// please appreciate my efforts for such an useless feature nobody will ever need :)
			/// </summary>
			utils_gpu_available constexpr auto get_edges()       noexcept { return edges_view<storage_type.is_const()>{storage_t::storage.begin(), storage_t::storage.size()}; }
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