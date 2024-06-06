#pragma once

#include "vec.h"
#include "angle.h"

namespace utils::math
	{
	struct transform2;
	template <typename T>
	struct rect;

	template <typename T> 
	using vec2 = vec<T, 2>;
	
	//fast typenames
	template <typename T> 
	using vecref2 = vecref<T, 2>;
	
	using vec2i      = vec2   <int          >;
	using vec2i8     = vec2   <int8_t       >;
	using vec2i16    = vec2   <int16_t      >;
	using vec2i32    = vec2   <int32_t      >;
	using vec2i64    = vec2   <int64_t      >;
	using vec2u      = vec2   <unsigned     >;
	using vec2u8     = vec2   <uint8_t      >;
	using vec2u16    = vec2   <uint16_t     >;
	using vec2u32    = vec2   <uint32_t     >;
	using vec2u64    = vec2   <uint64_t     >;
	using vec2s      = vec2   <size_t       >;
	using vec2f      = vec2   <float        >;
	using vec2d      = vec2   <double       >;
	using vec2l      = vec2   <long         >;
	using vec2ul     = vec2   <unsigned long>;
	using vecref2i   = vecref2<int          >;
	using vecref2i8  = vecref2<int8_t       >;
	using vecref2i16 = vecref2<int16_t      >;
	using vecref2i32 = vecref2<int32_t      >;
	using vecref2i64 = vecref2<int64_t      >;
	using vecref2u   = vecref2<unsigned     >;
	using vecref2u8  = vecref2<uint8_t      >;
	using vecref2u16 = vecref2<uint16_t     >;
	using vecref2u32 = vecref2<uint32_t     >;
	using vecref2u64 = vecref2<uint64_t     >;
	using vecref2s   = vecref2<size_t       >;
	using vecref2f   = vecref2<float        >;
	using vecref2d   = vecref2<double       >;
	using vecref2l   = vecref2<long         >;
	using vecref2ul  = vecref2<unsigned long>;

	namespace details
		{
		template<class T, typename derived_T>
		class utils_oop_empty_bases vec_sized_specialization<T, 2, derived_T>
			{
			private:
				using derived_t = derived_T;
				utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

			public:
				utils_gpu_available  static constexpr derived_t from(const math::angle::concepts::angle auto& angle, T magnitude = 1) noexcept
					{
					auto x{angle.cos() * magnitude};
					auto y{angle.sin() * magnitude};
					return derived_t{x, y};
					}

				template <typename T = float, T f_a_v = 360.f>
				utils_gpu_available constexpr math::angle::base<T, f_a_v> angle() const noexcept { return math::angle::base<T, f_a_v>::atan2(derived().y(), derived().x()); }
				
				// VEC & ANGLE OPERATIONS
				utils_gpu_available constexpr derived_t  operator+ (const math::angle::concepts::angle auto& angle) const noexcept
					{
					return
						{
						derived().x() * angle.cos() - derived().y() * angle.sin(),
						derived().x() * angle.sin() + derived().y() * angle.cos()
						};
					}
				utils_gpu_available constexpr derived_t  operator- (const math::angle::concepts::angle auto& angle) const noexcept
					{
					const auto nngle{-angle};
					return
						{
						derived().x() * nngle.cos() - derived().y() * nngle.sin(),
						derived().x() * nngle.sin() + derived().y() * nngle.cos()
						};
					}

				utils_gpu_available constexpr derived_t& operator+=(const math::angle::concepts::angle auto& angle) noexcept { return derived() = derived() + angle; }
				utils_gpu_available constexpr derived_t& operator-=(const math::angle::concepts::angle auto& angle) noexcept { return derived() = derived() - angle; }
				utils_gpu_available constexpr derived_t& operator= (const math::angle::concepts::angle auto& angle) noexcept
					{
					return derived() = {angle.cos() * derived().magnitude(), angle.sin() * derived().magnitude()}; 
					}

				// OTHER
				utils_gpu_available constexpr derived_t perpendicular_right           () const noexcept { return { derived().y(), -derived().x()}; }
				utils_gpu_available constexpr derived_t perpendicular_left            () const noexcept { return {-derived().y(),  derived().x()}; }
				utils_gpu_available constexpr derived_t perpendicular_clockwise       () const noexcept { return perpendicular_right(); }
				utils_gpu_available constexpr derived_t perpendicular_counterclockwise() const noexcept { return perpendicular_left (); }

				#pragma region geometry shape methods
					utils_gpu_available constexpr derived_t  scale         (const float                    & scaling    ) const noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>) { auto ret{derived()}; return ret.scale_self    (scaling    ); }
					utils_gpu_available constexpr derived_t  rotate        (const angle::base<float, 360.f>& rotation   ) const noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>) { auto ret{derived()}; return ret.rotate_self   (rotation   ); }
					utils_gpu_available constexpr derived_t  translate     (const vec<float, 2>            & translation) const noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>) { auto ret{derived()}; return ret.translate_self(translation); }
					utils_gpu_available constexpr derived_t  transform     (const utils::math::transform2  & transform  ) const noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>) { auto ret{derived()}; return ret.transform_self(transform  ); }

					utils_gpu_available constexpr derived_t& scale_self    (const float                    & scaling    ) noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>);
					utils_gpu_available constexpr derived_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>);
					utils_gpu_available constexpr derived_t& translate_self(const vec<float, 2>            & translation) noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>);
					utils_gpu_available constexpr derived_t& transform_self(const utils::math::transform2  & transform  ) noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>);

					utils_gpu_available constexpr rect<float> bounding_box() const noexcept requires(std::same_as<typename derived_t::nonref_value_type, float>);
				#pragma endregion geometry shape methods
			};
		}
	}