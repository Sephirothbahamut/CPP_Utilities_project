#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "base_types.h"
#include "../shape/ab.h"
#include "../shape/point.h"

namespace utils::math::geometry
	{
	template <bool 

	namespace storage
		{
		enum class type
			{
			owner, observer, const_observer
			};

		template <type storage, typename T>
		using single = std::conditional_t
			<
			storage == type::owner,
			T,
			std::conditional_t
			/**/<
			/**/storage == type::observer,
			/**/std::reference_wrapper<T>,
			/**/std::reference_wrapper<const T>
			/**/>
			>;

		template <type storage, typename T, size_t extent = std::dynamic_extent>
		using multiple = std::conditional_t
			<
			storage == type::owner,
			std::conditional_t
				<
				extent == std::dynamic_extent,
				std::vector<T>,
				std::array <T, extent>
				>,
			std::conditional_t
			/**/<
			/**/storage == type::observer,
			/**/std::span<      T, extent>,
			/**/std::span<const T, extent>
			/**/>
			>;

		template <typename T>
		inline static consteval type get_type()
			{
			if constexpr (std::same_as<typename T::value_type, std::reference_wrapper<typename T::value_type::value_type>>)
				{
				if constexpr (std::is_const<typename T::value_type::value_type>)
					{
					return type::const_observer;
					}
				else { return type::observer; }
				}
			else { return type::owner; }
			}
		}

	struct ends
		{
		struct create : ::utils::oop::non_constructible
			{
			utils_gpu_available static consteval ends open    (bool finite_a = false, bool finite_b = false) noexcept { return ends{.finite_a{finite_a}, .finite_b{finite_b}, .open{true }}; }
			utils_gpu_available static consteval ends infinite(                                            ) noexcept { return ends{.finite_a{false   }, .finite_b{false   }, .open{true }}; }
			utils_gpu_available static consteval ends closed  (                                            ) noexcept { return ends{.finite_a{true    }, .finite_b{true    }, .open{false}}; }
			};

		bool finite_a;
		bool finite_b;
		bool open;

		utils_gpu_available inline consteval bool operator==(const ends& other) const noexcept = default;

		utils_gpu_available inline consteval bool is_open      () const noexcept { return open; }
		utils_gpu_available inline consteval bool is_closed    () const noexcept { return !is_open(); }
		utils_gpu_available inline consteval bool is_a_infinite() const noexcept { return is_open() && !finite_a; }
		utils_gpu_available inline consteval bool is_b_infinite() const noexcept { return is_open() && !finite_b; }
		utils_gpu_available inline consteval bool is_a_finite  () const noexcept { return is_open() &&  finite_a; }
		utils_gpu_available inline consteval bool is_b_finite  () const noexcept { return is_open() &&  finite_b; }
		utils_gpu_available inline consteval bool is_finite    () const noexcept { return is_closed() || (finite_a && finite_b); }
		utils_gpu_available inline consteval bool is_infinite  () const noexcept { return !is_finite(); }
		};

	enum class shape_id
		{
		point,
		aabb,

		circle,
		capsule,

		ab,
		line,    //ab specialization
		ray,     //ab specialization
		segment, //ab specialization

		polyline,
		polygon, //polyline specialization

		bezier,
		spline,

		polycurve,
		polycurve_closed //polycurve specialization
		};

	namespace shape
		{
		namespace details {}
		namespace generic {}
		namespace owner {}
		namespace view {}

		using namespace owner;
		}

	class side
		{
		public:
			constexpr side() = default; // No need to specify utils_gpu_available for defaults
			utils_gpu_available constexpr side(float value) : _value{value < -math::constants::epsilonf ? -1.f : value > math::constants::epsilonf ? 1.f : 0.f }  {}

			struct create : ::utils::oop::non_constructible
				{
				utils_gpu_available static consteval side left      () noexcept { return {-1.f}; }
				utils_gpu_available static consteval side right     () noexcept { return { 1.f}; }
				utils_gpu_available static consteval side coincident() noexcept { return { 0.f}; }

				utils_gpu_available static consteval side inside    () noexcept { return left (); }
				utils_gpu_available static consteval side outside   () noexcept { return right(); }
				};
			
			utils_gpu_available constexpr bool is_left      (float epsilon = 0.f) const noexcept { return _value <  -epsilon; }
			utils_gpu_available constexpr bool is_coincident(float epsilon = 0.f) const noexcept { return _value >= -epsilon && _value <= epsilon; }
			utils_gpu_available constexpr bool is_right     (float epsilon = 0.f) const noexcept { return _value >   epsilon; }

			utils_gpu_available constexpr bool is_outside   (float epsilon = 0.f) const noexcept { return  is_right  (epsilon); }
			utils_gpu_available constexpr bool is_inside    (float epsilon = 0.f) const noexcept { return !is_outside(epsilon); }

			utils_gpu_available constexpr bool is_exactly_left      () const noexcept { return is_left      (0.f); }
			utils_gpu_available constexpr bool is_exactly_coincident() const noexcept { return is_coincident(0.f); }
			utils_gpu_available constexpr bool is_exactly_right     () const noexcept { return is_right     (0.f); }
			utils_gpu_available constexpr bool is_approx_left       () const noexcept { return is_left      (utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_coincident () const noexcept { return is_coincident(utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_right      () const noexcept { return is_right     (utils::math::constants::epsilonf); }
			
			utils_gpu_available constexpr bool is_exactly_inside () const noexcept { return is_inside (0.f); }
			utils_gpu_available constexpr bool is_exactly_outside() const noexcept { return is_outside(0.f); }
			utils_gpu_available constexpr bool is_approx_inside  () const noexcept { return is_inside (utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_outside () const noexcept { return is_outside(utils::math::constants::epsilonf); }


			utils_gpu_available constexpr float value    () const noexcept { return   _value ; }
			utils_gpu_available constexpr side  operator-() const noexcept { return {-_value}; }

		private:
			float _value{0.f};
		};
	utils_gpu_available constexpr float  operator* (const float& f, const side& side) noexcept { return f * side.value(); }
	utils_gpu_available constexpr float& operator*=(      float& f, const side& side) noexcept { return f = f * side.value(); }
	utils_gpu_available constexpr bool   operator==(      float& f, const side& side) noexcept { return (math::sign(f) == math::sign(side.value())) || (f == 0.f && side.value() == 0.f); }
	
	struct signed_distance
		{
		float value{std::numeric_limits<float>::infinity()};
		utils_gpu_available constexpr float           absolute () const noexcept { return math::abs( value); }
		utils_gpu_available constexpr side            side     () const noexcept { return          { value}; }
		utils_gpu_available constexpr signed_distance operator-() const noexcept { return          {-value}; }
		utils_gpu_available constexpr auto operator<=>(const signed_distance& other) const noexcept { return value <=> other.value; }
		utils_gpu_available constexpr auto operator== (const signed_distance& other) const noexcept { return value ==  other.value; }
		};

	template <typename T>
	struct closest_with_distance
		{ 
		T closest{};
		signed_distance distance{utils::math::constants::finf};

		utils_gpu_available static constexpr closest_with_distance<T> pick_closest(const closest_with_distance<T>& a, const closest_with_distance<T>& b) noexcept
			{
			return a.distance.absolute() < b.distance.absolute() ? a : b;
			}
		utils_gpu_available static constexpr closest_with_distance<T> pick_closest_raw(const closest_with_distance<T>& a, const closest_with_distance<T>& b) noexcept
			{
			return a.distance.value < b.distance.value ? a : b;
			}
		};
	
	using closest_point_with_distance = closest_with_distance<vec2f>;

	template <typename T>
	struct closest_pair_with_distance 
		{
		T closest_in_source; 
		T closest_in_target; 
		signed_distance distance;
		utils_gpu_available constexpr closest_pair_with_distance operator-() const noexcept { return {closest_in_target, closest_in_source, -distance}; }
		};

	using closest_points_with_distance = closest_pair_with_distance<vec2f>;
	}