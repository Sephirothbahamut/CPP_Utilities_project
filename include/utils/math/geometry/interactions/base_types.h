#pragma once

#include "../../../concepts.h"
#include "../../../compilation/gpu.h"

#include "../../angle.h"
#include "../../transform2.h"

#include "../shape/point.h"
#include "../shape/aabb.h"

namespace utils::math::geometry::interactions::return_types
	{
	using bounding_box = shape::aabb;

	class side
		{
		public:
			constexpr side() = default; // No need to specify utils_gpu_available for defaults
			utils_gpu_available constexpr side(float value) : _value{value < -math::constants::epsilonf ? -1.f : value > math::constants::epsilonf ? 1.f : 0.f} {}

			struct create : ::utils::oop::non_constructible
				{
				utils_gpu_available static consteval side left      () noexcept { return {-1.f}; }
				utils_gpu_available static consteval side right     () noexcept { return { 1.f}; }
				utils_gpu_available static consteval side coincident() noexcept { return { 0.f}; }

				utils_gpu_available static consteval side inside () noexcept { return left (); }
				utils_gpu_available static consteval side outside() noexcept { return right(); }
				};

			utils_gpu_available constexpr bool is_left      (float epsilon = 0.f) const noexcept { return _value <  -epsilon; }
			utils_gpu_available constexpr bool is_coincident(float epsilon = 0.f) const noexcept { return _value >= -epsilon && _value <= epsilon; }
			utils_gpu_available constexpr bool is_right     (float epsilon = 0.f) const noexcept { return _value >   epsilon; }

			utils_gpu_available constexpr bool is_outside(float epsilon = 0.f) const noexcept { return  is_right  (epsilon); }
			utils_gpu_available constexpr bool is_inside (float epsilon = 0.f) const noexcept { return !is_outside(epsilon); }

			utils_gpu_available constexpr bool is_exactly_left      () const noexcept { return is_left(0.f); }
			utils_gpu_available constexpr bool is_exactly_coincident() const noexcept { return is_coincident(0.f); }
			utils_gpu_available constexpr bool is_exactly_right     () const noexcept { return is_right(0.f); }
			utils_gpu_available constexpr bool is_approx_left       () const noexcept { return is_left(utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_coincident () const noexcept { return is_coincident(utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_right      () const noexcept { return is_right(utils::math::constants::epsilonf); }

			utils_gpu_available constexpr bool is_exactly_inside () const noexcept { return is_inside (0.f); }
			utils_gpu_available constexpr bool is_exactly_outside() const noexcept { return is_outside(0.f); }
			utils_gpu_available constexpr bool is_approx_inside  () const noexcept { return is_inside (utils::math::constants::epsilonf); }
			utils_gpu_available constexpr bool is_approx_outside () const noexcept { return is_outside(utils::math::constants::epsilonf); }


			utils_gpu_available constexpr float value    () const noexcept { return   _value; }
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
	struct closest_with_signed_distance
		{
		T closest{};
		signed_distance distance{utils::math::constants::finf};

		utils_gpu_available static constexpr closest_with_signed_distance<T> pick_closest(const closest_with_signed_distance<T>& a, const closest_with_signed_distance<T>& b) noexcept
			{
			return a.distance.absolute() < b.distance.absolute() ? a : b;
			}
		utils_gpu_available constexpr closest_with_signed_distance<T>& set_to_closest(const closest_with_signed_distance<T>& other) noexcept
			{
			if (other.distance.absolute() < distance.absolute()) { (*this) = other; }
			return *this;
			}
		};
	template <typename T>
	struct closest_with_distance
		{
		T closest{};
		float distance{utils::math::constants::finf};

		utils_gpu_available static constexpr closest_with_distance<T> pick_closest(const closest_with_signed_distance<T>& a, const closest_with_signed_distance<T>& b) noexcept
			{
			return a.distance < b.distance ? a : b;
			}
		utils_gpu_available constexpr closest_with_distance<T>& set_to_closest(const closest_with_signed_distance<T>& other) noexcept
			{
			if (other.distance < distance) { (*this) = other; }
			return *this;
			}
		};

	template <typename T>
	struct closest_pair_with_distance
		{
		T closest_in_source;
		T closest_in_target;
		signed_distance distance;
		utils_gpu_available constexpr closest_pair_with_distance operator-() const noexcept { return {closest_in_target, closest_in_source, -distance}; }
		};

	using closest_point_with_distance        = closest_with_distance       <shape::point>;
	using closest_point_with_signed_distance = closest_with_signed_distance<shape::point>;
	using closest_points_with_distance       = closest_pair_with_distance  <shape::point>;
	}


namespace utils::math::geometry::interactions
	{
	template <shape::concepts::any T> utils_gpu_available constexpr auto scale    (const T& shape, const float                    & scaling    ) noexcept { typename T::nonref_self_t ret{shape}; return ret.scale_self    (scaling    ); }
	template <shape::concepts::any T> utils_gpu_available constexpr auto rotate   (const T& shape, const angle::base<float, 360.f>& rotation   ) noexcept { typename T::nonref_self_t ret{shape}; return ret.rotate_self   (rotation   ); }
	template <shape::concepts::any T> utils_gpu_available constexpr auto translate(const T& shape, const vec2f                    & translation) noexcept { typename T::nonref_self_t ret{shape}; return ret.translate_self(translation); }
	template <shape::concepts::any T> utils_gpu_available constexpr auto transform(const T& shape, const utils::math::transform2  & transform  ) noexcept 
		{
		return translate(rotate(scale(shape, transform.scaling), transform.rotation), transform.translation);
		}
	
	utils_gpu_available constexpr auto& transform_self(shape::concepts::any auto& shape, const utils::math::transform2& transform) noexcept
		{
		return translate_self(rotate_self(scale_self(shape, transform.scaling), transform.rotation), transform.translation);
		}
	}