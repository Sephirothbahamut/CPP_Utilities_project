#pragma once

#include <stdexcept>
#include "../../../oop/crtp.h"
#include "../../../math/vec2.h"
#include "../../../math/constants.h"

namespace utils::math::geometry
	{
	enum class shapes_enum { point, circle, capsule, line, half_line, segment, polyline, bezier, composite };

	//struct data_buffer
	//	{
	//	std::vector<float> data;
	//
	//	struct shape_data { shapes_enum shape_id; size_t begin; size_t end; };
	//	std::vector<shape_data> shapes;
	//	std::vector<shape_data> composite_components;
	//	};

	class side
		{
		friend class geometry::side;
		public:
			utils_gpu_available constexpr side() = default;
			utils_gpu_available constexpr side(float value) : _value{value < math::constants::epsilonf ? -1.f : value > math::constants::epsilonf ? 1.f : 0.f }  {}

			utils_gpu_available inline static consteval side left      () noexcept { return {-1.f}; }
			utils_gpu_available inline static consteval side right     () noexcept { return { 1.f}; }
			utils_gpu_available inline static consteval side coincident() noexcept { return { 0.f}; }
			
			utils_gpu_available inline static consteval side in () noexcept { return left (); }
			utils_gpu_available inline static consteval side out() noexcept { return right(); }

			utils_gpu_available constexpr float value() const noexcept { return _value; }
		private:
			float _value{0.f};
		};
	utils_gpu_available constexpr float  operator* (const float& f, const side& side) noexcept { return f * side.value(); }
	utils_gpu_available constexpr float& operator*=(      float& f, const side& side) noexcept { return f = f * side.value(); }
	utils_gpu_available constexpr bool   operator==(      float& f, const side& side) noexcept { return (std::signbit(f) == std::signbit(side.value())) || (f == 0.f && side.value() == 0.f); }

	struct signed_distance
		{
		float value;
		utils_gpu_available constexpr float distance() const noexcept { return std::abs(value); }
		utils_gpu_available constexpr side  side    () const noexcept { return {value}; }
		};

	struct closest_point_distance_t { vec2f closest; signed_distance signed_distance; };
	struct closest_points_distance_t { vec2f closest_in_source; vec2f closest_in_target; signed_distance distance; };

	namespace shape
		{
		namespace details
			{
			template <typename derived_t> struct base;
			}
		namespace interface
			{
			template <typename derived_t> struct point    ;
			template <typename derived_t> struct line     ;
			template <typename derived_t> struct ray      ;
			template <typename derived_t> struct segment  ;
			template <typename derived_t> struct polyline ;
			template <typename derived_t> struct circle   ;
			template <typename derived_t> struct capsule  ;
			template <typename derived_t> struct bezier   ;
			template <typename derived_t> struct spline   ;
			}
		namespace concepts
			{
			template <typename T> concept any      = oop::concepts::derived_from_crtp<T, details  ::base    >;
			template <typename T> concept point    = oop::concepts::derived_from_crtp<T, interface::point   >;
			template <typename T> concept line     = oop::concepts::derived_from_crtp<T, interface::line    >;
			template <typename T> concept ray      = oop::concepts::derived_from_crtp<T, interface::ray     >;
			template <typename T> concept segment  = oop::concepts::derived_from_crtp<T, interface::segment >;
			template <typename T> concept polyline = oop::concepts::derived_from_crtp<T, interface::polyline>;
			template <typename T> concept circle   = oop::concepts::derived_from_crtp<T, interface::circle  >;
			template <typename T> concept capsule  = oop::concepts::derived_from_crtp<T, interface::capsule >;
			template <typename T> concept bezier   = oop::concepts::derived_from_crtp<T, interface::bezier  >;
			template <typename T> concept spline   = oop::concepts::derived_from_crtp<T, interface::spline  >;
			}
		namespace details
			{
			template <typename derived_t>
			struct base : oop::crtp<derived_d>
				{
				protected:
					using crtp = oop::crtp<derived_d>;

				public:
					utils_gpu_available constexpr side                      side_of                (const concepts::any auto& other) noexcept { return crtp::derived().signed_distance_to(other).side    (); };
					utils_gpu_available constexpr float                     distance_to            (const concepts::any auto& other) noexcept { return crtp::derived().signed_distance_to(other).distance(); };
					utils_gpu_available constexpr signed_distance           signed_distance_to     (const concepts::any auto& other) noexcept { return {crtp::derived().distance_to(other) * crtp::derived().side_of(other)}; };
					utils_gpu_available constexpr vec2f                     closest_point_to       (const concepts::any auto& other) noexcept { return crtp::derived().closest_and_distance(other).closest ; };
					utils_gpu_available constexpr closest_point_distance_t  closest_and_distance_to(const concepts::any auto& other) noexcept { return {crtp::derived().closest_point_to(other), crtp::derived().signed_distance(other)}; };
					utils_gpu_available constexpr closest_points_distance_t closest_pair           (const concepts::any auto& other) noexcept { return {crtp::derived().closest_point_to(other), other.closest_point_to(crtp::derived()), crtp::derived().signed_distance(other)}; };
					utils_gpu_available constexpr vec2f                     vector_to              (const concepts::any auto& other) noexcept { return other.closest_point_to(crtp::derived()) - crtp::derived().closest_point_to(other); };
					utils_gpu_available constexpr bool                      intersects             (const concepts::any auto& other) noexcept { return crtp::derived().intersection(other).has_value(); };
					utils_gpu_available constexpr std::optional<vec2f>      intersection           (const concepts::any auto& other) noexcept { return crtp::derived().intersects(other) ? crtp::derived().closest_point_to(other) : std::nullopt; };
					utils_gpu_available constexpr bool                      contains               (const concepts::any auto& other) noexcept { return other.distance_signed(crtp::derived()) <= 0; };
					utils_gpu_available constexpr bool                      collides_with          (const concepts::any auto& other) noexcept { return crtp::derived().intersects(other) || crtp::derived().contains(other); };
		
					utils_gpu_available constexpr derived_t  scale         (const float      & scaling    ) noexcept { auto ret{crtp::derived()}; return ret.scale_crtp    ::derived()(scaling    ); }
					utils_gpu_available constexpr derived_t  rotate        (const angle::radf& rotation   ) noexcept { auto ret{crtp::derived()}; return ret.rotate_crtp   ::derived()(rotation   ); }
					utils_gpu_available constexpr derived_t  translate     (const vec2f      & translation) noexcept { auto ret{crtp::derived()}; return ret.translate_crtp::derived()(translation); }
					utils_gpu_available constexpr derived_t  transform     (const transform2 & transform  ) noexcept { auto ret{crtp::derived()}; return ret.transform_crtp::derived()(transform  ); }
					utils_gpu_available constexpr derived_t& scale_self    (const float      & scaling    ) noexcept { crtp::unimplemented_interface(); return *this; }
					utils_gpu_available constexpr derived_t& rotate_self   (const angle::radf& rotation   ) noexcept { crtp::unimplemented_interface(); return *this; }
					utils_gpu_available constexpr derived_t& translate_self(const vec2f      & translation) noexcept { crtp::unimplemented_interface(); return *this; }
					utils_gpu_available constexpr derived_t& transform_self(const transform2 & transform  ) noexcept { crtp::unimplemented_interface(); return *this; }
				};
			}
		}
	}