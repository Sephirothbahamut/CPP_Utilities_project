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

			using utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, EXTENT, true>::multiple;

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
			
			utils_gpu_available constexpr nonref_self_t scale    (const float                    & scaling    ) const noexcept { nonref_self_t ret{*this}; return ret.scale_self    (scaling    ); }
			utils_gpu_available constexpr nonref_self_t rotate   (const angle::base<float, 360.f>& rotation   ) const noexcept { nonref_self_t ret{*this}; return ret.rotate_self   (rotation   ); }
			utils_gpu_available constexpr nonref_self_t translate(const vec2f                    & translation) const noexcept { nonref_self_t ret{*this}; return ret.translate_self(translation); }
			utils_gpu_available constexpr nonref_self_t transform(const utils::math::transform2  & transform  ) const noexcept { nonref_self_t ret{*this}; return ret.transform_self(transform  ); }

			utils_gpu_available constexpr self_t& scale_self    (const float                    & scaling    ) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.scale_self    (scaling    ); } return *this; }
			utils_gpu_available constexpr self_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.rotate_self   (rotation   ); } return *this; }
			utils_gpu_available constexpr self_t& translate_self(const vec2f                    & translation) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.translate_self(translation); } return *this; }
			utils_gpu_available constexpr self_t& transform_self(const utils::math::transform2  & transform  ) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.transform_self(transform  ); } return *this; }
			
			utils_gpu_available constexpr shape::aabb bounding_box() const noexcept
				{
				return shape::aabb::create::from_vertices(*this);
				}
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