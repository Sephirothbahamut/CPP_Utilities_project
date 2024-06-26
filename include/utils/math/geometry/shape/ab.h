#pragma once

#include "../details/base_types.h"
#include "../../../storage.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type STORAGE_TYPE>
		struct ab : geometry::shape_flag
			{
			inline static constexpr storage::type storage_type = STORAGE_TYPE;

			using self_t = ab<storage_type>;
			using vertex_t = generic::point<storage_type>;
			template <bool is_function_const>
			using vertex_observer = generic::point<storage::type::create::observer(is_function_const)>;
			using nonref_self_t = ab<storage::type::create::owner()>;

			utils_gpu_available constexpr ab(const utils::math::concepts::vec_size<2> auto& a, const utils::math::concepts::vec_size<2> auto& b) noexcept
				requires(storage_type.can_construct_from_const()) : 
				a{a}, b{b} {}
			
			utils_gpu_available constexpr ab(utils::math::concepts::vec_size<2> auto& a, utils::math::concepts::vec_size<2> auto& b) noexcept :
				a{a}, b{b} {}

			template <storage::type other_storage_type>
			utils_gpu_available constexpr ab(ab<other_storage_type>& other) noexcept
				requires(storage::constness_matching<self_t, ab<other_storage_type>>::compatible_constness) :
				a{other.a}, b{other.b} {}

			template <storage::type other_storage_type>
			utils_gpu_available constexpr ab(const ab<other_storage_type>& other) noexcept
				requires(storage_type.can_construct_from_const()) :
				a{other.a}, b{other.b} {}

			vertex_t a;
			vertex_t b;

			utils_gpu_available constexpr const vertex_t closest_vertex(const concepts::point auto& other) const noexcept { return shape::point::distance2(other, a) < shape::point::distance2(other, b) ? a : b; }
			utils_gpu_available constexpr       vertex_t closest_vertex(      concepts::point auto& other)       noexcept { return shape::point::distance2(other, a) < shape::point::distance2(other, b) ? a : b; }

			utils_gpu_available constexpr float length2() const noexcept { return shape::point::distance2(a, b); }
			utils_gpu_available constexpr float length () const noexcept { return shape::point::distance (a, b); }

			/// <summary> Vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f a_to_b() const noexcept { return b - a; }
			/// <summary> Vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f b_to_a() const noexcept { return a - b; }
			/// <summary> Unit vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f forward() const noexcept { return a_to_b().normalize(); }
			/// <summary> Unit vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f backward() const noexcept { return b_to_a().normalize(); }
			/// <summary> Unit vector perpendicular on the left from a to b. </summary>
			utils_gpu_available constexpr vec2f perpendicular_right() const noexcept { return forward().perpendicular_right(); }
			/// <summary> Unit vector perpendicular on the right from a to b. </summary>
			utils_gpu_available constexpr vec2f perpendicular_left () const noexcept { return forward().perpendicular_left (); }

			/// <summary> Projecting point to the line that goes through a-b, at what percentage of the segment a-b it lies. < 0 is before a, > 1 is after b, proportionally to the a-b distance </summary>
			template <bool clamp_a, bool clamp_b>
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& point) const noexcept
				{
				//from shadertoy version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//const utils::math::vec2f b_a{b   - a};
				//const utils::math::vec2f p_a{point - a};
				//return utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a);

				//previous version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
				const vec2f delta{b - a};
				const auto ret{((point.x() - a.x()) * delta.x() + (point.y() - a.y()) * delta.y()) / (delta.x() * delta.x() + delta.y() * delta.y())};
				if constexpr (clamp_a) { if (ret < 0.f) { return 0.f; } }
				if constexpr (clamp_b) { if (ret > 1.f) { return 1.f; } }
				return ret;
				}

			utils_gpu_available constexpr float some_significant_name_ive_yet_to_figure_out(const concepts::point auto& point) const noexcept
				{
				//signed distance from line in proportion to the distance between a and b, idk, i'm not a math guy
				//enough alone to get the sign for side, but needs to be divided by (a-b).length to get the signed distance
				const float ret{((b.x() - a.x()) * (point.y() - a.y())) - ((point.x() - a.x()) * (b.y() - a.y()))};
				return -ret;
				}
			};

		template <storage::type storage_type> 
		struct line : ab<storage_type>
			{
			using ab<storage_type>::ab;
			inline static constexpr geometry::ends ends{geometry::ends::create::infinite()};

			utils_gpu_available constexpr float length2() const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return utils::math::constants::finf; }

			using ab<storage_type>::projected_percent;
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& point) const noexcept
				{
				return projected_percent<false, false>(point);
				}
			};

		template <storage::type storage_type> 
		struct ray : ab<storage_type>
			{
			using ab<storage_type>::ab;
			inline static constexpr geometry::ends ends{geometry::ends::create::open(true, false)};

			utils_gpu_available constexpr float length2() const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return utils::math::constants::finf; }

			using ab<storage_type>::projected_percent;
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& point) const noexcept
				{
				return projected_percent<true, false>(point);
				}
			};

		template <storage::type storage_type> 
		struct segment : ab<storage_type>
			{
			using ab<storage_type>::ab;
			inline static constexpr geometry::ends ends{geometry::ends::create::open(true, true)};

			using ab<storage_type>::projected_percent;
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& point) const noexcept
				{
				return projected_percent<true, true>(point);
				}
			};
		}

	namespace concepts
		{
		template <typename T> concept ab      = std::derived_from    <T, shape::generic::ab     <T::storage_type>>;
		template <typename T> concept line    = ab<T> && std::same_as<T, shape::generic::line   <T::storage_type>>;
		template <typename T> concept ray     = ab<T> && std::same_as<T, shape::generic::ray    <T::storage_type>>;
		template <typename T> concept segment = ab<T> && std::same_as<T, shape::generic::segment<T::storage_type>>;
		}
	
	namespace owner 
		{
		using ab      = shape::generic::ab     <storage::type::create::owner()>;
		using line    = shape::generic::line   <storage::type::create::owner()>;
		using ray     = shape::generic::ray    <storage::type::create::owner()>;
		using segment = shape::generic::segment<storage::type::create::owner()>;
		}
	namespace observer
		{
		using ab      = shape::generic::ab     <storage::type::create::observer()>;
		using line    = shape::generic::line   <storage::type::create::observer()>;
		using ray     = shape::generic::ray    <storage::type::create::observer()>;
		using segment = shape::generic::segment<storage::type::create::observer()>;
		}
	namespace const_observer
		{
		using ab      = shape::generic::ab     <storage::type::create::const_observer()>;
		using line    = shape::generic::line   <storage::type::create::const_observer()>;
		using ray     = shape::generic::ray    <storage::type::create::const_observer()>;
		using segment = shape::generic::segment<storage::type::create::const_observer()>;
		}
	}