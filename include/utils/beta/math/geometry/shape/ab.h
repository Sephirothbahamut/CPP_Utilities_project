#pragma once

#include "../details/base_types.h"
#include "../../../../storage.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type STORAGE_TYPE>
		struct ab
			{
			inline static constexpr storage::type storage_type = STORAGE_TYPE;

			using self_t = ab<storage_type>;
			using vertex_t = generic::point<storage_type>;
			template <bool is_function_const>
			using vertex_observer = generic::point<storage::type::create::observer(is_function_const)>;
			using nonref_self_t = ab<storage::type::create::owner()>;

			utils_gpu_available constexpr ab(const utils::math::concepts::vec_size<2> auto& a, const utils::math::concepts::vec_size<2> auto& b) noexcept
				requires(storage_type.can_construct_from_const()) : 
				vertex_a{a}, vertex_b{b} {}
			
			utils_gpu_available constexpr ab(utils::math::concepts::vec_size<2> auto& a, utils::math::concepts::vec_size<2> auto& b) noexcept :
				vertex_a{a}, vertex_b{b} {}

			template <storage::type other_storage_type>
			utils_gpu_available constexpr ab(ab<other_storage_type>& other) noexcept
				requires(storage::constness_matching<self_t, ab<other_storage_type>>::compatible_constness) :
				vertex_a{other.a()}, vertex_b{other.b()} {}

			template <storage::type other_storage_type>
			utils_gpu_available constexpr ab(const ab<other_storage_type>& other) noexcept
				requires(storage_type.can_construct_from_const()) :
				vertex_a{other.a()}, vertex_b{other.b()} {}

			vertex_t vertex_a;
			vertex_t vertex_b;

			utils_gpu_available constexpr vertex_observer<true > a() const noexcept { return {vertex_a}; }
			utils_gpu_available constexpr vertex_observer<false> a()       noexcept { return {vertex_a}; }
			utils_gpu_available constexpr vertex_observer<true > b() const noexcept { return {vertex_b}; }
			utils_gpu_available constexpr vertex_observer<false> b()       noexcept { return {vertex_b}; }

			//utils_gpu_available constexpr vertex_observer<true > operator[](const size_t& index) const noexcept { return index == 0 ? a() : b(); }
			//utils_gpu_available constexpr vertex_observer<false> operator[](const size_t& index)       noexcept { return index == 0 ? a() : b(); }

			utils_gpu_available constexpr vertex_observer<true > closest_vertex(const concepts::point auto& other) const noexcept { return shape::point::distance2(other, a()) < shape::point::distance2(other, b()) ? a() : b(); }
			utils_gpu_available constexpr vertex_observer<false> closest_vertex(      concepts::point auto& other)       noexcept { return shape::point::distance2(other, a()) < shape::point::distance2(other, b()) ? a() : b(); }

			utils_gpu_available constexpr float length2() const noexcept { return shape::point::distance2(a(), b()); }
			utils_gpu_available constexpr float length () const noexcept { return shape::point::distance (a(), b()); }

			/// <summary> Vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f a_to_b() const noexcept { return b() - a(); }
			/// <summary> Unit vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f forward() const noexcept { return a_to_b().normalize(); }
			/// <summary> Unit vector perpendicular on the left from a() to b(). </summary>
			utils_gpu_available constexpr vec2f perpendicular_right() const noexcept { return forward().perpendicular_right(); }
			/// <summary> Unit vector perpendicular on the right from a() to b(). </summary>
			utils_gpu_available constexpr vec2f perpendicular_left () const noexcept { return forward().perpendicular_left (); }

			/// <summary> Projecting point to the line that goes through a()-b(), at what percentage of the segment a()-b() it lies. < 0 is before a(), > 1 is after b(), proportionally to the a()-b() distance </summary>
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& point) const noexcept
				{
				//from shadertoy version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//const utils::math::vec2f b_a{b()   - a()};
				//const utils::math::vec2f p_a{point - a()};
				//return utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a);

				//previous version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a()-point-and-a()-line-segment-in-c/
				const vec2f delta{b() - a()};
				return ((point.x - a().x) * delta.x + (point.y - a().y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);
				}

			utils_gpu_available constexpr float some_significant_name_ive_yet_to_figure_out(const concepts::point auto& point) const noexcept
				{
				//signed distance from line in proportion to the distance between a() and b(), idk, i'm not a() math guy
				//enough alone to get the sign for side, but needs to be divided by (a()-b()).length to get the signed distance
				const float ret{((b().x - a().x) * (point.y - a().y)) - ((point.x - a().x) * (b().y - a().y))};
				return -ret;
				}







			template <bool clamp_a = false, bool clamp_b = false>
			utils_gpu_available constexpr vec2f closest_point(const concepts::point auto& point) const noexcept
				{
				const vec2f delta{b() - a()};
				const float t{projected_percent(point)};
				if constexpr (clamp_a) { if (t < 0.f) { return a(); } }
				if constexpr (clamp_b) { if (t > 1.f) { return b(); } }
				return {a().x + t * delta.x, a().y + t * delta.y};
				}

			template <bool clamp_a = false, bool clamp_b = false>
			utils_gpu_available constexpr float minimum_distance(const concepts::point auto& point) const noexcept
				{
				if constexpr (clamp_a || clamp_b)
					{
					const float t{projected_percent(point)};
					if constexpr (clamp_a) { if (t < 0.f) { return vec2f::distance(a(), point); } }
					if constexpr (clamp_b) { if (t > 1.f) { return vec2f::distance(b(), point); } }
					}
				return std::abs(some_significant_name_ive_yet_to_figure_out(point) / a_to_b().get_length());
				}

			template <bool clamp_a = false, bool clamp_b = false>
			utils_gpu_available constexpr signed_distance signed_distance(const concepts::point auto& point) const noexcept
				{
				if constexpr (clamp_a || clamp_b)
					{
					const float t{projected_percent(point)};
					if constexpr (clamp_a) { if (t < 0.f) { return vec2f::distance(a(), point) * side(point); } }
					if constexpr (clamp_b) { if (t > 1.f) { return vec2f::distance(b(), point) * side(point); } }
					}
				return some_significant_name_ive_yet_to_figure_out(point) / a_to_b().get_length();
				}

			utils_gpu_available constexpr side side(const concepts::point auto& point) const noexcept
				{
				return {some_significant_name_ive_yet_to_figure_out(point)};
				}
				
			utils_gpu_available constexpr nonref_self_t scale    (const float                    & scaling    ) const noexcept { nonref_self_t ret{*this}; return ret.scale_self    (scaling    ); }
			utils_gpu_available constexpr nonref_self_t rotate   (const angle::base<float, 360.f>& rotation   ) const noexcept { nonref_self_t ret{*this}; return ret.rotate_self   (rotation   ); }
			utils_gpu_available constexpr nonref_self_t translate(const vec2f                    & translation) const noexcept { nonref_self_t ret{*this}; return ret.translate_self(translation); }
			utils_gpu_available constexpr nonref_self_t transform(const utils::math::transform2  & transform  ) const noexcept { nonref_self_t ret{*this}; return ret.transform_self(transform  ); }

			utils_gpu_available constexpr self_t& scale_self    (const float                    & scaling    ) noexcept requires(!storage_type.is_const()) { a().scale_self    (scaling    ); b().scale_self    (scaling    ); return *this; }
			utils_gpu_available constexpr self_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept requires(!storage_type.is_const()) { a().rotate_self   (rotation   ); b().rotate_self   (rotation   ); return *this; }
			utils_gpu_available constexpr self_t& translate_self(const vec2f                    & translation) noexcept requires(!storage_type.is_const()) { a().translate_self(translation); b().translate_self(translation); return *this; }
			utils_gpu_available constexpr self_t& transform_self(const utils::math::transform2  & transform  ) noexcept requires(!storage_type.is_const()) { a().transform_self(transform  ); b().transform_self(transform  ); return *this; }

			utils_gpu_available constexpr shape::aabb bounding_box() const noexcept;
			};

		template <storage::type storage_type> 
		struct line : ab<storage_type>
			{
			using ab<storage_type>::ab;

			utils_gpu_available constexpr float length2() const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return utils::math::constants::finf; }

			utils_gpu_available constexpr vec2f closest_point(const concepts::point auto& point) const noexcept
				{
				return ab<storage_type>::closest_point<false, false>(point); 
				}
			};

		template <storage::type storage_type> 
		struct ray : ab<storage_type>
			{
			using ab<storage_type>::ab;

			inline static constexpr geometry::ends static_ends{geometry::ends::create::open(true, false)};

			utils_gpu_available constexpr float length2() const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return utils::math::constants::finf; }

			utils_gpu_available constexpr float projected_percent(const concepts::point auto& other) const noexcept
				{
				return std::max(0.f, ab<storage_type>::projected_percent(other));
				}
			utils_gpu_available constexpr vec2f closest_point(const concepts::point auto& point) const noexcept
				{
				return ab<storage_type>::closest_point<true, false>(point);
				}
			};

		template <storage::type storage_type> 
		struct segment : ab<storage_type>
			{
			using ab<storage_type>::ab;

			utils_gpu_available constexpr float projected_percent(const concepts::point auto& other) const noexcept
				{
				return std::clamp(ab<storage_type>::projected_percent(other), 0.f, 1.f);
				}
			utils_gpu_available constexpr vec2f closest_point(const concepts::point auto& point) const noexcept
				{
				return ab<storage_type>::closest_point<true, true>(point);
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