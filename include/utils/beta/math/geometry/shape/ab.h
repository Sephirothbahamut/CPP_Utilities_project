#pragma once

#include "../details/base_types.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type>
		struct ab
			{
			public:
				using self_t = ab<storage_type>;
				inline static constexpr storage::type static_storage_type{storage_type};
				using value_type = storage::single<static_storage_type, shape::point>;
				using nonref_value_type = utils::remove_reference_t<value_type>;
				using nonref_self_t = ab<storage::type::owner>;
				
				std::array<value_type, 2> container;

				template <storage::type other_storage_type>
				ab(const ab<other_storage_type>& other) noexcept requires(!utils::concepts::reference<value_type>) : container{other.a(), other.b()} {}

				template <storage::type other_storage_type>
				ab(ab<other_storage_type>& other) noexcept 
					requires(utils::concepts::reference<value_type> && !utils::concepts::const_reference<typename ab<other_storage_type>::value_type>) 
					: container{other.a(), other.b()} {}

				utils_gpu_available const nonref_value_type& operator[](size_t index) const noexcept { return static_cast<const nonref_value_type&>(container[index]); }
				utils_gpu_available       nonref_value_type& operator[](size_t index)       noexcept { return static_cast<      nonref_value_type&>(container[index]); }
				
				utils_gpu_available constexpr const nonref_value_type& a() const noexcept { return static_cast<const nonref_value_type&>(container[0]); }
				utils_gpu_available constexpr       nonref_value_type& a()       noexcept { return static_cast<      nonref_value_type&>(container[0]); }
				utils_gpu_available constexpr const nonref_value_type& b() const noexcept { return static_cast<const nonref_value_type&>(container[1]); }
				utils_gpu_available constexpr       nonref_value_type& b()       noexcept { return static_cast<      nonref_value_type&>(container[1]); }

				utils_gpu_available constexpr const nonref_value_type& operator[](const size_t& index) const noexcept { return index == 0 ? a() : b(); }
				utils_gpu_available constexpr       nonref_value_type& operator[](const size_t& index)       noexcept { return index == 0 ? a() : b(); }

				utils_gpu_available constexpr const nonref_value_type& closest_vertex(const concepts::point auto& other) const noexcept { return shape::point::distance2(other, a()) < shape::point::distance2(other, b()) ? a() : b(); }
				utils_gpu_available constexpr       nonref_value_type& closest_vertex(      concepts::point auto& other)       noexcept { return shape::point::distance2(other, a()) < shape::point::distance2(other, b()) ? a() : b(); }

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
				utils_gpu_available constexpr float projected_percent(const concepts::point auto& other) const noexcept
					{
					//from shadertoy version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
					//const utils::math::vec2f b_a{b()   - a()};
					//const utils::math::vec2f p_a{other - a()};
					//return utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a);

					//previous version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
					//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a()-point-and-a()-line-segment-in-c/
					const vec2f delta{b() - a()};
					return ((other.x - a().x) * delta.x + (other.y - a().y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);
					}

				utils_gpu_available constexpr float some_significant_name_ive_yet_to_figure_out(const concepts::point auto& other) const noexcept
					{
					//signed distance from line in proportion to the distance between a() and b(), idk, i'm not a() math guy
					//enough alone to get the sign for side, but needs to be divided by (a()-b()).length to get the signed distance
					const float ret{((b().x - a().x) * (other.y - a().y)) - ((other.x - a().x) * (b().y - a().y))};
					return -ret;
					}

				template <bool clamp_a, bool clamp_b>
				utils_gpu_available constexpr vec2f closest_point_custom_clamp(const concepts::point auto& other) noexcept
					{
					const vec2f delta{b() - a()};
					const float t{projected_percent(other)};
					if constexpr (clamp_a) { if (t < 0.f) { return a(); } }
					if constexpr (clamp_b) { if (t > 1.f) { return b(); } }
					return {a().x + t * delta.x, a().y + t * delta.y};
					}
				
				
				utils_gpu_available constexpr nonref_self_t scale    (const float                    & scaling    ) const noexcept { nonref_self_t ret{*this}; return ret.scale_self    (scaling    ); }
				utils_gpu_available constexpr nonref_self_t rotate   (const angle::base<float, 360.f>& rotation   ) const noexcept { nonref_self_t ret{*this}; return ret.rotate_self   (rotation   ); }
				utils_gpu_available constexpr nonref_self_t translate(const vec2f                    & translation) const noexcept { nonref_self_t ret{*this}; return ret.translate_self(translation); }
				utils_gpu_available constexpr nonref_self_t transform(const utils::math::transform2  & transform  ) const noexcept { nonref_self_t ret{*this}; return ret.transform_self(transform  ); }

				utils_gpu_available constexpr self_t& scale_self    (const float                    & scaling    ) noexcept requires(!utils::concepts::const_value<value_type>) { a().scale_self    (scaling    ); b().scale_self    (scaling    ); return *this; }
				utils_gpu_available constexpr self_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept requires(!utils::concepts::const_value<value_type>) { a().rotate_self   (rotation   ); b().rotate_self   (rotation   ); return *this; }
				utils_gpu_available constexpr self_t& translate_self(const vec2f                    & translation) noexcept requires(!utils::concepts::const_value<value_type>) { a().translate_self(translation); b().translate_self(translation); return *this; }
				utils_gpu_available constexpr self_t& transform_self(const utils::math::transform2  & transform  ) noexcept requires(!utils::concepts::const_value<value_type>) { a().transform_self(transform  ); b().transform_self(transform  ); return *this; }

				utils_gpu_available constexpr shape::aabb bounding_box() const noexcept;
			};

		template <storage::type storage_type> 
		struct line : ab<storage_type>
			{
			inline static constexpr geometry::ends static_ends{geometry::ends::create::infinite()};

			utils_gpu_available constexpr float length2() const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return utils::math::constants::finf; }
			};

		template <storage::type storage_type> 
		struct ray : ab<storage_type>
			{
			inline static constexpr geometry::ends static_ends{geometry::ends::create::open(true, false)};

			utils_gpu_available constexpr float length2() const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float length () const noexcept { return utils::math::constants::finf; }
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& other) const noexcept
				{
				return std::max(0.f, ab<storage_type>::projected_percent(other));
				}
			};

		template <storage::type storage_type> 
		struct segment : ab<storage_type>
			{
			utils_gpu_available constexpr float projected_percent(const concepts::point auto& other) const noexcept
				{
				return std::clamp(ab<storage_type>::projected_percent(other), 0.f, 1.f);
				}
			};
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
		using ab      = shape::generic::ab     <storage::type::owner>;
		using line    = shape::generic::line   <storage::type::owner>;
		using ray     = shape::generic::ray    <storage::type::owner>;
		using segment = shape::generic::segment<storage::type::owner>;
		}
	namespace observer
		{
		using ab      = shape::generic::ab     <storage::type::observer>;
		using line    = shape::generic::line   <storage::type::observer>;
		using ray     = shape::generic::ray    <storage::type::observer>;
		using segment = shape::generic::segment<storage::type::observer>;
		}
	namespace const_observer
		{
		using ab      = shape::generic::ab     <storage::type::const_observer>;
		using line    = shape::generic::line   <storage::type::const_observer>;
		using ray     = shape::generic::ray    <storage::type::const_observer>;
		using segment = shape::generic::segment<storage::type::const_observer>;
		}
	}