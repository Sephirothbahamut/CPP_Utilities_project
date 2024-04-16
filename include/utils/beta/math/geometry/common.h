#pragma once

#include <stdexcept>
#include "../../../oop/crtp.h"
#include "../../../math/vec2.h"
#include "../../../math/constants.h"
#include "../../../math/transform2.h"

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
		public:
			constexpr side() = default; // No need to specify utils_gpu_available for defaults
			utils_gpu_available constexpr side(float value) : _value{value < math::constants::epsilonf ? -1.f : value > math::constants::epsilonf ? 1.f : 0.f }  {}

			struct create
				{
				create() = delete;
				
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
	
	struct signed_distance_t
		{
		float value{std::numeric_limits<float>::infinity()};
		utils_gpu_available constexpr float             absolute () const noexcept { return math::abs(value); }
		utils_gpu_available constexpr side              side     () const noexcept { return          {value}; }
		utils_gpu_available constexpr signed_distance_t operator-() const noexcept { return          {-value}; }
		auto operator<=>(const signed_distance_t& other) const noexcept { return value <=> other.value; }
		auto operator== (const signed_distance_t& other) const noexcept { return value ==  other.value; }
		};

	struct closest_point_with_distance_t  
		{ 
		vec2f closest; 
		signed_distance_t distance;

		utils_gpu_available static constexpr closest_point_with_distance_t pick_closest(const closest_point_with_distance_t& a, const closest_point_with_distance_t& b) noexcept
			{
			return a.distance.absolute() < b.distance.absolute() ? a : b;
			}
		};

	struct closest_pair_with_distance_t 
		{
		vec2f closest_in_source; 
		vec2f closest_in_target; 
		signed_distance_t distance;
		utils_gpu_available constexpr closest_pair_with_distance_t operator-() const noexcept { return {closest_in_target, closest_in_source, -distance}; }
		};

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
			template <typename T> concept point    = oop::concepts::derived_from_crtp<T, interface::point   > && any<T>;
			template <typename T> concept line     = oop::concepts::derived_from_crtp<T, interface::line    > && any<T>;
			template <typename T> concept ray      = oop::concepts::derived_from_crtp<T, interface::ray     > && any<T>;
			template <typename T> concept segment  = oop::concepts::derived_from_crtp<T, interface::segment > && any<T>;
			template <typename T> concept polyline = oop::concepts::derived_from_crtp<T, interface::polyline> && any<T>;
			template <typename T> concept circle   = oop::concepts::derived_from_crtp<T, interface::circle  > && any<T>;
			template <typename T> concept capsule  = oop::concepts::derived_from_crtp<T, interface::capsule > && any<T>;
			template <typename T> concept bezier   = oop::concepts::derived_from_crtp<T, interface::bezier  > && any<T>;
			template <typename T> concept spline   = oop::concepts::derived_from_crtp<T, interface::spline  > && any<T>;
			}
		namespace details
			{
			template <typename derived_t>
			struct base : oop::crtp<derived_t>
				{
				protected:
					using crtp = oop::crtp<derived_t>;
					

				public:
					utils_gpu_available constexpr side                          side_of              (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr geometry::signed_distance_t   distance             (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr vec2f                         closest_point_to     (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr closest_point_with_distance_t closest_with_distance(const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr closest_pair_with_distance_t  closest_pair         (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr vec2f                         vector_to            (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr bool                          intersects           (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr std::optional<vec2f>          intersection_with    (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr bool                          contains             (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
					utils_gpu_available constexpr bool                          collides_with        (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
		
					utils_gpu_available constexpr derived_t  scale         (const float      & scaling    ) const noexcept { auto ret{crtp::derived()}; return ret.scale_self    (scaling    ); }
					utils_gpu_available constexpr derived_t  rotate        (const angle::radf& rotation   ) const noexcept { auto ret{crtp::derived()}; return ret.rotate_self   *(rotation   ); }
					utils_gpu_available constexpr derived_t  translate     (const vec2f      & translation) const noexcept { auto ret{crtp::derived()}; return ret.translate_self(translation); }
					utils_gpu_available constexpr derived_t  transform     (const transform2 & transform  ) const noexcept { auto ret{crtp::derived()}; return ret.transform_self(transform  ); }

					utils_gpu_available constexpr derived_t& scale_self    (const float      & scaling    )       noexcept { crtp::unimplemented_interface(); return *this; }
					utils_gpu_available constexpr derived_t& rotate_self   (const angle::radf& rotation   )       noexcept { crtp::unimplemented_interface(); return *this; }
					utils_gpu_available constexpr derived_t& translate_self(const vec2f      & translation)       noexcept { crtp::unimplemented_interface(); return *this; }
					utils_gpu_available constexpr derived_t& transform_self(const transform2 & transform  )       noexcept { crtp::unimplemented_interface(); return *this; }
				};
			}
		}
	}