#pragma once

#include "../details/base_types.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type, geometry::ends ends>
		struct ab
			{
			public:
				using self_t = ab<storage_type, ends>;
				inline static constexpr storage::type static_storage_type{storage_type};
				inline static constexpr geometry::ends static_ends{ends};
				using vertex_type = storage::single<static_storage_type, shape::point>;
				vertex_type a;
				vertex_type b;

				utils_gpu_available constexpr float length2() const noexcept { return static_ends.is_finite() ? shape::point::distance2(vec2f{a}, vec2f{b}) : utils::math::constants::finf; }
				utils_gpu_available constexpr float length () const noexcept { return static_ends.is_finite() ? shape::point::distance (vec2f{a}, vec2f{b}) : utils::math::constants::finf; }

				utils_gpu_available constexpr vec2f        operator[](const size_t& index) const noexcept { return index == 0 ? a : b; }
				utils_gpu_available constexpr vertex_type& operator[](const size_t& index)       noexcept { return index == 0 ? a : b; }

				/// <summary> Vector from a towards b. </summary>
				utils_gpu_available constexpr vec2f a_to_b() const noexcept { return b - a; }
				/// <summary> Unit vector from a towards b. </summary>
				utils_gpu_available constexpr vec2f forward() const noexcept { return a_to_b().normalize(); }
				/// <summary> Unit vector perpendicular on the left from a to b. </summary>
				utils_gpu_available constexpr vec2f perpendicular_right() const noexcept { return forward().perpendicular_right(); }
				/// <summary> Unit vector perpendicular on the right from a to b. </summary>
				utils_gpu_available constexpr vec2f perpendicular_left () const noexcept { return forward().perpendicular_left (); }

				utils_gpu_available constexpr vec2f        closest_vertex(const concepts::point auto& other) const noexcept { return other.distance(a) < other.distance_min(b) ? a : b; }
				utils_gpu_available constexpr vertex_type& closest_vertex(const concepts::point auto& other) const noexcept { return other.distance(a) < other.distance_min(b) ? a : b; }
			
				/// <summary> Projecting point to the line that goes through a-b, at what percentage of the segment a-b it lies. < 0 is before a, > 1 is after b, proportionally to the a-b distance </summary>
				utils_gpu_available constexpr float projected_percent(const concepts::point    auto& other) const noexcept
					{
					//from shadertoy version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
					//const utils::math::vec2f b_a{b   - a};
					//const utils::math::vec2f p_a{other - a};
					//return utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a);

					//previous version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
					//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
					const vec2f delta{b - a};
					return ((other.x - a.x) * delta.x + (other.y - a.y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);
					}

				utils_gpu_available constexpr float some_significant_name_ive_yet_to_figure_out(const concepts::point auto& other) const noexcept
					{
					//signed distance from line in proportion to the distance between a and b, idk, i'm not a math guy
					//enough alone to get the sign for side, but needs to be divided by (a-b).length to get the signed distance
					const float ret{((b.x - a.x) * (other.y - a.y)) - ((other.x - a.x) * (b.y - a.y))};
					return -ret;
					}

				template <bool clamp_a, bool clamp_b>
				utils_gpu_available constexpr vec2f closest_point_custom_clamp(const concepts::point auto& other) noexcept
					{
					const vec2f delta{b - a};
					const float t{projected_percent(other)};
					if constexpr (clamp_a) { if (t < 0.f) { return a; } }
					if constexpr (clamp_b) { if (t > 1.f) { return b; } }
					return {a.x + t * delta.x, a.y + t * delta.y};
					}
				
				#pragma region geometry shape methods
					utils_gpu_available constexpr self_t  scale         (const float                    & scaling    ) const noexcept { auto ret{*this}; return ret.scale_self    (scaling    ); }
					utils_gpu_available constexpr self_t  rotate        (const angle::base<float, 360.f>& rotation   ) const noexcept { auto ret{*this}; return ret.rotate_self   (rotation   ); }
					utils_gpu_available constexpr self_t  translate     (const vec<float, 2>            & translation) const noexcept { auto ret{*this}; return ret.translate_self(translation); }
					utils_gpu_available constexpr self_t  transform     (const utils::math::transform2  & transform  ) const noexcept { auto ret{*this}; return ret.transform_self(transform  ); }

					utils_gpu_available constexpr self_t& scale_self    (const float                    & scaling    ) noexcept;
					utils_gpu_available constexpr self_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept;
					utils_gpu_available constexpr self_t& translate_self(const vec<float, 2>            & translation) noexcept;
					utils_gpu_available constexpr self_t& transform_self(const utils::math::transform2  & transform  ) noexcept;

					utils_gpu_available constexpr rect<float> bounding_box() const noexcept;
				#pragma endregion geometry shape methods
			};

		template <storage::type storage_type> using line    = ab<storage_type, ends::create::open(false, false)>;
		template <storage::type storage_type> using ray     = ab<storage_type, ends::create::open(true , false)>;
		template <storage::type storage_type> using segment = ab<storage_type, ends::create::open(true , true )>;
		}

	namespace concepts
		{
		template <typename T> concept ab      = std::same_as<T, shape::generic::ab<T::static_storage_type, T::static_ends>>;
		template <typename T> concept line    = ab<T> && (T::static_ends == ends::create::open(false, false));
		template <typename T> concept ray     = ab<T> && (T::static_ends == ends::create::open(true , false));
		template <typename T> concept segment = ab<T> && (T::static_ends == ends::create::open(true , true ));
		}

	namespace owner 
		{
		template <ends ends>
		using ab = shape::generic::ab<storage::type::owner, ends>; 
		using line    = ab<ends::create::open(false, false)>;
		using ray     = ab<ends::create::open(true , false)>;
		using segment = ab<ends::create::open(true , true )>;
		}
	namespace observer
		{
		template <ends ends>
		using ab = shape::generic::ab<storage::type::observer, ends>; 
		using line    = ab<ends::create::open(false, false)>;
		using ray     = ab<ends::create::open(true , false)>;
		using segment = ab<ends::create::open(true , true )>;
		}
	namespace const_observer
		{
		template <ends ends>
		using ab = shape::generic::ab<storage::type::const_observer, ends>; 
		using line    = ab<ends::create::open(false, false)>;
		using ray     = ab<ends::create::open(true , false)>;
		using segment = ab<ends::create::open(true , true )>;
		}
	}