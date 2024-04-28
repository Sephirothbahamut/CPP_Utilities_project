#pragma once

#include "../details/base_types.h"

namespace utils::math::geometry::shape::interface
	{
	template <typename derived_T, ends_t ends>
	struct ab : utils::math::geometry::shape::details::base<derived_T, shapes_enum::ab>
		{
		protected:
			using derived_t = derived_T;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<derived_t&>(*this); }

		public:
			inline static constexpr ends_t static_ends = ends;
		
			utils_gpu_available constexpr float length2() const noexcept { return static_ends.is_finite() ? vec2f::distance2(derived().a, derived().b) : utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return static_ends.is_finite() ? vec2f::distance (derived().a, derived().b) : utils::math::constants::finf; }

			utils_gpu_available constexpr const vec2f& operator[](const size_t& index) const noexcept { return index == 0 ? derived().a : derived().b; }
			utils_gpu_available constexpr       vec2f& operator[](const size_t& index)       noexcept { return index == 0 ? derived().a : derived().b; }

			/// <summary> Vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f a_to_b() const noexcept { return derived().b - derived().a; }
			/// <summary> Unit vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f forward() const noexcept { return a_to_b().normalize(); }
			/// <summary> Unit vector perpendicular on the left from a to b. </summary>
			utils_gpu_available constexpr vec2f perpendicular_right() const noexcept { return forward().perpendicular_right(); }
			/// <summary> Unit vector perpendicular on the right from a to b. </summary>
			utils_gpu_available constexpr vec2f perpendicular_left () const noexcept { return forward().perpendicular_left (); }

			utils_gpu_available constexpr vec2f closest_vertex(const concepts::point auto& other) const noexcept { return other.distance(derived().a) < other.distance_min(derived().b) ? derived().a : derived().b; }
			
			/// <summary> Projecting point to the line that goes through a-b, at what percentage of the segment a-b it lies. < 0 is before a, > 1 is after b, proportionally to the a-b distance </summary>
			utils_gpu_available constexpr float projected_percent(const concepts::point    auto& other) const noexcept
				{
				//from shadertoy version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//const utils::math::vec2f b_a{derived().b   - derived().a};
				//const utils::math::vec2f p_a{other - derived().a};
				//return utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a);

				//previous version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-derived().a-point-and-derived().a-line-segment-in-c/
				const vec2f delta{derived().b - derived().a};
				return ((other.x - derived().a.x) * delta.x + (other.y - derived().a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);
				}

			utils_gpu_available constexpr float some_significant_name_ive_yet_to_figure_out(const concepts::point auto& other) const noexcept
				{
				//signed distance from line in proportion to the distance between derived().a and derived().b, idk, i'm not a math guy
				//enough alone to get the sign for side, but needs to be divided by (derived().a-derived().b).length to get the signed distance
				const float ret{((derived().b.x - derived().a.x) * (other.y - derived().a.y)) - ((other.x - derived().a.x) * (derived().b.y - derived().a.y))};
				return ret;
				}

			template <bool clamp_a, bool clamp_b>
			utils_gpu_available constexpr vec2f closest_point_custom_clamp(const concepts::point auto& other) noexcept
				{
				const vec2f delta{derived().b - derived().a};
				const float t{projected_percent(other)};
				if constexpr (clamp_a) { if (t < 0.f) { return derived().a; } }
				if constexpr (clamp_b) { if (t > 1.f) { return derived().b; } }
				return {derived().a.x + t * delta.x, derived().a.y + t * delta.y};
				}
		};
	}

namespace utils::math::geometry::shape
	{
	template <ends_t ends>
	struct ab : utils::math::geometry::shape::interface::ab<ab<ends>, ends>
		{
		utils_gpu_available constexpr ab(const ::utils::math::vec2f& a, const ::utils::math::vec2f& b) : a{a}, b{b} {}
		utils_gpu_available constexpr ab() = default;

		::utils::math::vec2f a;
		::utils::math::vec2f b;
		};

	using line    = ab<ends_t::create::open(false, false)>;
	using ray     = ab<ends_t::create::open(true , false)>;
	using segment = ab<ends_t::create::open(true , true )>;

	namespace view
		{
		template <ends_t ends>
		struct ab : utils::math::geometry::shape::interface::ab<ab<ends>, ends>
			{
			utils_gpu_available constexpr ab(const ::utils::math::vecref2f& a, const ::utils::math::vecref2f& b) : a{a}, b{b} {}
			utils_gpu_available constexpr ab() = default;

			::utils::math::vecref2f a;
			::utils::math::vecref2f b;
			};
		
		using line    = ab<ends_t::create::open(false, false)>;
		using ray     = ab<ends_t::create::open(true , false)>;
		using segment = ab<ends_t::create::open(true , true )>;
		}
	}