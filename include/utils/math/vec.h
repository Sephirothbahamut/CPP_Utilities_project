#pragma once
#ifndef utils_vec_old
#include <cmath>
#include <array>
#include <numbers>
#include <ostream>
#include <algorithm>

#include "../custom_operators.h"
#include "math.h"
#include "../containers/array.h"

#include "angle.h"

#ifdef COUT_CONTAINERS
#include "../cout_utilities.h"
#endif

namespace utils::math
	{
	template <typename T, size_t size> class vec;
	}

#ifdef COUT_CONTAINERS
namespace utils::cout
	{
	template <typename T, size_t size>
	inline std::ostream& operator<<(std::ostream& os, const utils::math::vec<T, size>& vec) noexcept
		{
		namespace ccu = cout::support;
		return os << ccu::type << "vec" << utils::math::vec<T, size>::size << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::brace << ")";
		}
	}
#endif

namespace utils::math
	{
	template <template<typename> class To, typename T, class From>
	inline static To<T> vec_cast(From from) { return {static_cast<T>(from.x), static_cast<T>(from.y)}; }

#ifdef utils_vec_partial_specialization
	namespace details
		{
		template<class derived_self_t, class T, size_t size>
		class vec_impl;
		}
#endif //utils_vec_partial_specialization

	template <size_t size> using veci   = vec<int     , size>;
	template <size_t size> using veci8  = vec<int8_t  , size>;
	template <size_t size> using veci16 = vec<int16_t , size>;
	template <size_t size> using veci32 = vec<int32_t , size>;
	template <size_t size> using veci64 = vec<int64_t , size>;

	template <size_t size> using vecu   = vec<unsigned, size>;
	template <size_t size> using vecu8  = vec<uint8_t , size>;
	template <size_t size> using vecu16 = vec<uint16_t, size>;
	template <size_t size> using vecu32 = vec<uint32_t, size>;
	template <size_t size> using vecu64 = vec<uint64_t, size>;

	template <size_t size> using vecs   = vec<size_t  , size>;
	template <size_t size> using vecf   = vec<float   , size>;
	template <size_t size> using vecd   = vec<double  , size>;

	template <typename T, size_t size>
	class vec 
#ifdef utils_vec_partial_specialization
		: public details::vec_impl<vec<T, size>, T, size>
#endif //utils_vec_partial_specialization
		{
		protected:
#ifdef utils_vec_partial_specialization
			template<class derived_self_t, class T, size_t size>
			friend class details::vec_impl;

			using details::vec_impl<vec<T, size>, T, size>::vec_impl;
#endif //utils_vec_partial_specialization
			std::array<T, size> _data;

		public:
			//TODO add AT functions and call AT in [] in debug mode
			      T& operator[](size_t index)       noexcept { return _data[index]; }
			const T& operator[](size_t index) const noexcept { return _data[index]; }

		public:
			inline static constexpr size_t size = size;
			vec() = default;

			template <typename other_t, size_t other_size>
			vec(vec<other_t, other_size> other)           noexcept { containers::array::copy(_data, other._data); } //TODO test


			T magnitude2                () const noexcept { T ret{0}; for (const auto& element : _data) { ret += element * element; } return ret; }
			T magnitude                 () const noexcept { return std::sqrt(magnitude2()); }
			vec<T, size>  normalize     () const noexcept { return magnitude() ? *this / magnitude() : *this; }

			vec<T, size> abs  (                )                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::abs(_data[i]); } return ret; }
			vec<T, size> min  (vec<T, size> min)                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::min  (_data[i], min._data[i]              ); } return ret; }
			vec<T, size> max  (vec<T, size> max)                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::max  (_data[i], max._data[i]              ); } return ret; }
			vec<T, size> clamp(vec<T, size> min, vec<T, size> max) const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::clamp(_data[i], min._data[i], max._data[i]); } return ret; }

			vec<T, size>& abs_self      (                                  ) noexcept { return *this = abs      (        ); }
			vec<T, size>& normalize_self(                                  ) noexcept { return *this = normalize(        ); }
			vec<T, size>& min_self      (vec<T, size> min                  ) noexcept { return *this = this->min(min     ); }
			vec<T, size>& max_self      (vec<T, size> max                  ) noexcept { return *this = this->max(max     ); }
			vec<T, size>& clamp_self    (vec<T, size> min, vec<T, size> max) noexcept { return *this = clamp    (min, max); }

			static T dot      (const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] * b[i]; } return ret; }
			static T distance2(const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] - b[i]; } return ret; }
			static T distance (const vec<T, size>& a, const vec<T, size>& b) noexcept { return std::sqrt(distance2(a, b)); }

			template <typename T, size_t size> vec<T, size>& operator= (const T n) noexcept { *this = *this.normalize() * n; return *this; }

			vec<T, size>& operator= (const T n) noexcept { *this = normalize() * n; return *this; }
			// OTHER
			
			static vec<T, size> lerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				vec<T, size> ret;
				for (size_t i{0}; i < size; i++) { ret[i] = utils::lerp(a[i], b[i], t); }
				return ret;
				}
			static vec<T, size> slerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				return lerp(a, b).normalize() * (utils::lerp(a.magnitude(), b.magnitude(), t));
				}
			static vec<T, size> tlerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				return lerp(a, b).normalize() * std::sqrt(utils::lerp(a.magnitude2(), b.magnitude2(), t));
				}
			static vec<T, size> slerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept //TODO test
				{
				T dot = std::clamp(vec::dot(a, b), -1.0f, 1.0f);
				T theta = std::acos(dot) * t;
				vec<T, size> relative_vec = (b - a * dot).normalized();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

#pragma region vec2 specialization
#ifdef utils_vec_concept_sfinae
			vec(T x, T y)                            noexcept requires(size == 2) : _data{x, y} {};
			vec(math::angle::deg angle, T magnitude) noexcept requires(size == 2) : _data{angle.cos() * magnitude, angle.sin() * magnitude} {} //TODO test
			vec(math::angle::rad angle, T magnitude) noexcept requires(size == 2) : _data{angle.cos() * magnitude, angle.sin() * magnitude} {} //TODO test
			template <float full_angle_value>
			vec(math::angle::base_angle<full_angle_value> angle) noexcept requires(size == 2) : x{angle.cos()}, y{angle.sin()} {} //TODO test
			//template <typename other_t>
			//vec2(vec2<other_t> other)           noexcept : x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) } {} //TODO test
			//vec2(const vec2<T>& other) noexcept : x{other.x}, y{other.y} {} //TODO test


			static vec<T, 2> rr()    noexcept requires(size >= 2) { return {T{ 1}, T{ 0}}; }
			static vec<T, 2> ll()    noexcept requires(size >= 2) { return {T{-1}, T{ 0}}; }
			static vec<T, 2> up()    noexcept requires(size >= 2) { return {T{ 0}, T{-1}}; }
			static vec<T, 2> dw()    noexcept requires(size >= 2) { return {T{ 0}, T{ 1}}; }
			static vec<T, 2> right() noexcept requires(size >= 2) { return rr(); }
			static vec<T, 2> left()  noexcept requires(size >= 2) { return ll(); }
			static vec<T, 2> down()  noexcept requires(size >= 2) { return dw(); }
			static vec<T, 2> zero()  noexcept requires(size >= 2) { return {}; }

			      T& get_x(              )       noexcept requires(size >= 1) { return this->_data[0]; }
			const T& get_x(              ) const noexcept requires(size >= 1) { return this->_data[0]; }
				  T& set_x(const T& value)       noexcept requires(size >= 1) { return this->_data[0] = value; }
				  
			__declspec(property(get = get_x, put = set_x)) T x;
			__declspec(property(get = get_x, put = set_x)) T r;

			      T& get_y(              )       noexcept requires(size >= 2) { return this->_data[1]; }
			const T& get_y(              ) const noexcept requires(size >= 2) { return this->_data[1]; }
			      T& set_y(const T& value)       noexcept requires(size >= 2) { return this->_data[1] = value; }

			__declspec(property(get = get_y, put = set_y)) T y;
			__declspec(property(get = get_y, put = set_y)) T g;

			      T& get_z(              )       noexcept requires(size >= 3) { return this->_data[2]; }
			const T& get_z(              ) const noexcept requires(size >= 3) { return this->_data[2]; }
			      T& set_z(const T& value)       noexcept requires(size >= 3) { return this->_data[2] = value; }

			__declspec(property(get = get_z, put = set_z)) T z;
			__declspec(property(get = get_z, put = set_z)) T b;

			      T& get_w(              )       noexcept requires(size >= 4) { return this->_data[3]; }
			const T& get_w(              ) const noexcept requires(size >= 4) { return this->_data[3]; }
			      T& set_w(const T& value)       noexcept requires(size >= 4) { return this->_data[3] = value; }

			__declspec(property(get = get_w, put = set_w)) T w;
			__declspec(property(get = get_w, put = set_w)) T a;

			// CASTS
			template <template<typename> class To, typename T_t>
			inline To<T_t> vec_cast() requires(size >= 2) { return {static_cast<T_t>(x), static_cast<T_t>(y)}; }

			template <float full_angle_value = 360.f>
			math::angle::base_angle<full_angle_value> angle() const noexcept requires(size >= 2) { return math::angle::base_angle<full_angle_value>::atan2(y, x); }

			template <typename other_t>
			operator vec<other_t, 2>() const noexcept { return {static_cast<other_t>(x), static_cast<other_t>(y)}; }
			
			template <float full_angle_value> vec<T, 2>& operator= (const utils::math::angle::base_angle<full_angle_value> angle)       noexcept requires(size >= 2) { return *this = {angle.cos() * this->magnitude(), angle.sin() * this->magnitude()}; }
			// OTHER
			vec<T, 2> perpendicular_right()            const noexcept requires(size >= 2) { return {y, -x}; }
			vec<T, 2> perpendicular_left()             const noexcept requires(size >= 2) { return {-y,  x}; }
			vec<T, 2> perpendicular_clockwise()        const noexcept requires(size >= 2) { return perpendicular_right(); }
			vec<T, 2> perpendicular_counterclockwise() const noexcept requires(size >= 2) { return perpendicular_left(); }
#endif //utils_vec_concept_sfinae
#pragma endregion vec2 specialization
		};
	
	// OPERATORS
	template <typename T, size_t size> vec<T, size>  operator-(const vec<T, size>& v) noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = -v[i]; }; return ret; }

	// VEC & SCALAR OPERATORS
	template <typename T, size_t size> vec<T, size>  operator++(vec<T, size>& v) noexcept { return *this += T(1); }
	template <typename T, size_t size> vec<T, size>  operator--(vec<T, size>& v) noexcept { return *this -= T(1); }

	template <typename T, size_t size> vec<T, size>  operator+ (const vec<T, size>& v, const T n) noexcept { return {v.normalize() * (v.magnitude() + n)}; }
	template <typename T, size_t size> vec<T, size>  operator- (const vec<T, size>& v, const T n) noexcept { return {v.normalize() * (v.magnitude() - n)}; }
	template <typename T, size_t size> vec<T, size>  operator* (const vec<T, size>& v, const T n) noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = v[i] * n; }; return ret; }
	template <typename T, size_t size> vec<T, size>  operator/ (const vec<T, size>& v, const T n) noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = v[i] / n; }; return ret; }

	template <typename T, size_t size> vec<T, size>& operator+=(      vec<T, size>& v, const T n) noexcept { return v = v + n; }
	template <typename T, size_t size> vec<T, size>& operator-=(      vec<T, size>& v, const T n) noexcept { return v = v - n; }
	template <typename T, size_t size> vec<T, size>& operator*=(      vec<T, size>& v, const T n) noexcept { return v = v * n; }
	template <typename T, size_t size> vec<T, size>& operator/=(      vec<T, size>& v, const T n) noexcept { return v = v / n; }

	// VEC & VEC OPERATORS
	//TODO vec&vec operators when other has a different size
	template <typename T, size_t size> vec<T, size>  operator+ (const vec<T, size>& v, const vec<T, size>& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] + oth[i]; }; return ret; }
	template <typename T, size_t size> vec<T, size>  operator- (const vec<T, size>& v, const vec<T, size>& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] - oth[i]; }; return ret; }
	template <typename T, size_t size> vec<T, size>  operator* (const vec<T, size>& v, const vec<T, size>& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] * oth[i]; }; return ret; }
	template <typename T, size_t size> vec<T, size>  operator/ (const vec<T, size>& v, const vec<T, size>& oth) noexcept { auto ret{v}; for (size_t i{0}; i < size; i++) { ret[i] = v[i] / oth[i]; }; return ret; }

	template <typename T, size_t size> vec<T, size>& operator+=(      vec<T, size>& v, const vec<T, size>& oth) noexcept { return v = v + oth; }
	template <typename T, size_t size> vec<T, size>& operator-=(      vec<T, size>& v, const vec<T, size>& oth) noexcept { return v = v - oth; }
	template <typename T, size_t size> vec<T, size>& operator*=(      vec<T, size>& v, const vec<T, size>& oth) noexcept { return v = v * oth; }
	template <typename T, size_t size> vec<T, size>& operator/=(      vec<T, size>& v, const vec<T, size>& oth) noexcept { return v = v / oth; }
	
	template <typename T, size_t size> bool          operator==(const vec<T, size>& v, const vec<T, size>& oth) noexcept { for (size_t i{0}; i < size; i++) { if (v[i] != oth[i]) { return false; } }; return true; }
	template <typename T, size_t size> bool          operator!=(const vec<T, size>& v, const vec<T, size>& oth) noexcept { return !(*this == oth); }

#ifdef utils_vec_concept_sfinae
	// VEC & ANGLE OPERATIONS
	template <typename T, float full_angle_value> vec<T, 2>  operator+ (const vec<T, 2>& v, const utils::math::angle::base_angle<full_angle_value> angle) noexcept { return {v.x * angle.cos() - v.y * angle.sin(), v.x * angle.sin() + v.y * angle.cos()}; }
	template <typename T, float full_angle_value> vec<T, 2>  operator- (const vec<T, 2>& v, const utils::math::angle::base_angle<full_angle_value> angle) noexcept { return {v.x * angle.cos() - v.y * angle.sin(), v.x * angle.sin() + v.y * angle.cos()}; }
	template <typename T, float full_angle_value> vec<T, 2>& operator+=(      vec<T, 2>& v, const utils::math::angle::base_angle<full_angle_value> angle) noexcept { return v = v + angle; }
	template <typename T, float full_angle_value> vec<T, 2>& operator-=(      vec<T, 2>& v, const utils::math::angle::base_angle<full_angle_value> angle) noexcept { return v = v - angle; }
#endif //utils_vec_concept_sfinae
	template <typename T, size_t size>
	vec<T, size> abs(const vec<T, size> vec) noexcept { return vec.abs(); }

	namespace operators
		{
		inline constexpr struct _dot
			{
			template <typename T, size_t size>
			class _inner;

			template <typename T, size_t size>
			inline friend _inner<T, size> operator<(vec<T, size> lhs, _dot proxy) noexcept { return {lhs}; }

			template <typename T, size_t size>
			class _inner
				{
				public:
					T operator>(vec<T, size> rhs) const noexcept { return vec<T, size>::dot(lhs, rhs); }
					_inner(vec<T, size> lhs) noexcept : lhs{lhs} {}
				private:
					vec<T, size> lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <typename T, size_t size>
			class _inner;

			template <typename T, size_t size>
			inline friend _inner<T, size> operator<(vec<T, size> lhs, _cross proxy) noexcept { return {lhs}; }

			template <typename T, size_t size>
			class _inner
				{
				public:
					vec<T, size> operator>(vec<T, size> rhs) const noexcept { return lhs * rhs; }
					_inner(vec<T, size> lhs) noexcept : lhs{lhs} {}
				private:
					vec<T, size> lhs;
				};

			} cross;
		}
	}
namespace utils
	{
	template <typename T, size_t size>
	inline math::vec<T, size> lerp(math::vec<T, size> a, math::vec<T, size> b, float t) { return math::vec<T, size>::lerp(a, b, t); }
	template <typename T, size_t size>
	inline math::vec<T, size> clamp(math::vec<T, size> in, math::vec<T, size> min, math::vec<T, size> max) { return in.clamp(min, max); }
	template <typename T, size_t size>
	inline math::vec<T, size> min(math::vec<T, size> in, math::vec<T, size> min) { return in.min(min); }
	template <typename T, size_t size>
	inline math::vec<T, size> max(math::vec<T, size> in, math::vec<T, size> max) { return in.max(max); }
	}
#endif

//#pragma once
//#include <cmath>
//#include <array>
//#include <numbers>
//#include <ostream>
//#include <algorithm>
//
//#include "../custom_operators.h"
//#include "math.h"
//#include "../containers/array.h"
//
//#include "angle.h"
//
//#ifdef COUT_CONTAINERS
//#include "../cout_utilities.h"
//#endif
//
//namespace utils::math
//	{
//	template <typename T, size_t size> class vec;
//
//	}
//
//#ifdef COUT_CONTAINERS
//namespace utils::cout
//	{
//	template <typename T, size_t size>
//	inline std::ostream& operator<<(std::ostream& os, const utils::math::vec<T, size>& vec) noexcept
//		{
//		namespace ccu = cout::support;
//		return os << ccu::type << "vec" << T::size << ccu::brace << "(" << ccu::value << vec.x << ccu::separ << ", " << ccu::value << vec.y << ccu::brace << ")";
//		}
//	}
//#endif
//
//namespace utils::math
//	{
//	template <size_t size> using veci   = vec<int     , size>;
//	template <size_t size> using veci8  = vec<int8_t  , size>;
//	template <size_t size> using veci16 = vec<int16_t , size>;
//	template <size_t size> using veci32 = vec<int32_t , size>;
//	template <size_t size> using veci64 = vec<int64_t , size>;
//
//	template <size_t size> using vecu   = vec<unsigned, size>;
//	template <size_t size> using vecu8  = vec<uint8_t , size>;
//	template <size_t size> using vecu16 = vec<uint16_t, size>;
//	template <size_t size> using vecu32 = vec<uint32_t, size>;
//	template <size_t size> using vecu64 = vec<uint64_t, size>;
//
//	template <size_t size> using vecs   = vec<size_t  , size>;
//	template <size_t size> using vecf   = vec<float   , size>;
//	template <size_t size> using vecd   = vec<double  , size>;
//
//	template <typename T, size_t size>
//	class vec
//		{
//		protected:
//			std::array<T, size> _data;
//
//		public:
//			//TODO add AT functions and call AT in [] in debug mode
//			      T& operator[](size_t index)       noexcept { return _data[index]; }
//			const T& operator[](size_t index) const noexcept { return _data[index]; }
//
//		public:
//			inline static constexpr size_t size = size;
//			vec() = default;
//
//			template <typename other_t, size_t other_size>
//			vec(vec<other_t, other_size> other)           noexcept { containers::array::copy(_data, other._data); } //TODO test
//
//
//			T magnitude2                () const noexcept { T ret{0}; for (const auto& element : _data) { ret += element * element; } return ret; }
//			T magnitude                 () const noexcept { return std::sqrt(magnitude2()); }
//			vec<T, size>  normalize     () const noexcept { return magnitude() ? *this / magnitude() : *this; }
//
//			vec<T, size>  abs           () const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::abs(_data[i]); } return ret; }
//
//			vec<T, size> min  (vec<T, size> min)                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::min  (_data[i], min._data[i]              ); } return ret; }
//			vec<T, size> max  (vec<T, size> max)                   const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::max  (_data[i], max._data[i]              ); } return ret; }
//			vec<T, size> clamp(vec<T, size> min, vec<T, size> max) const noexcept { vec<T, size> ret; for (size_t i{0}; i < size; i++) { ret[i] = std::clamp(_data[i], min._data[i], max._data[i]); } return ret; }
//
//			vec<T, size>& abs_self      ()                               noexcept { return *this = abs      (        ); }
//			vec<T, size>& normalize_self()                               noexcept { return *this = normalize(        ); }
//			vec<T, size>& min_self  (vec<T, size> min)                   noexcept { return *this = this->min(min     ); }
//			vec<T, size>& max_self  (vec<T, size> max)                   noexcept { return *this = this->max(max     ); }
//			vec<T, size>& clamp_self(vec<T, size> min, vec<T, size> max) noexcept { return *this = clamp    (min, max); }
//
//			// OPERATORS
//			vec<T, size>  operator-() const noexcept { auto ret{*this}; for (const T& element : ret._data) { element = -element; }; return ret; }
//
//			// VEC & SCALAR OPERATORS
//			vec<T, size>  operator++() noexcept { return *this += T(1); }
//			vec<T, size>  operator--() noexcept { return *this -= T(1); }
//
//			vec<T, size>  operator+ (const T n) const noexcept { return {normalize() * (magnitude() + n)}; }
//			vec<T, size>  operator- (const T n) const noexcept { return {normalize() * (magnitude() - n)}; }
//			vec<T, size>  operator* (const T n) const noexcept { auto ret{*this}; for (const T& element : ret._data) { element *= n; }; return ret; }
//			vec<T, size>  operator/ (const T n) const noexcept { auto ret{*this}; for (const T& element : ret._data) { element /= n; }; return ret; }
//
//			vec<T, size>& operator+=(const T n)       noexcept { return *this = *this + n; }
//			vec<T, size>& operator-=(const T n)       noexcept { return *this = *this - n; }
//			vec<T, size>& operator*=(const T n)       noexcept { return *this = *this * n; }
//			vec<T, size>& operator/=(const T n)       noexcept { return *this = *this / n; }
//			vec<T, size>& operator= (const T n)       noexcept { *this = normalize() * n; return *this; }
//
//			// VEC & VEC OPERATORS
//			//TODO vec&vec operators when other has a different size
//			vec<T, size>  operator+ (const vec<T, size>& oth) const noexcept { auto ret{*this}; for (size_t i{0}; i < size; i++) { ret[i] = _data[i] + oth._data[i]; }; return ret; }
//			vec<T, size>  operator- (const vec<T, size>& oth) const noexcept { auto ret{*this}; for (size_t i{0}; i < size; i++) { ret[i] = _data[i] - oth._data[i]; }; return ret; }
//			vec<T, size>  operator* (const vec<T, size>& oth) const noexcept { auto ret{*this}; for (size_t i{0}; i < size; i++) { ret[i] = _data[i] * oth._data[i]; }; return ret; }
//			vec<T, size>  operator/ (const vec<T, size>& oth) const noexcept { auto ret{*this}; for (size_t i{0}; i < size; i++) { ret[i] = _data[i] / oth._data[i]; }; return ret; }
//
//			vec<T, size>& operator+=(const vec<T, size>& oth)       noexcept { return *this = *this + oth; }
//			vec<T, size>& operator-=(const vec<T, size>& oth)       noexcept { return *this = *this - oth; }
//			vec<T, size>& operator*=(const vec<T, size>& oth)       noexcept { return *this = *this * oth; }
//			vec<T, size>& operator/=(const vec<T, size>& oth)       noexcept { return *this = *this / oth; }
//
//			bool     operator==(const vec<T, size>& oth) const noexcept { for (size_t i{0}; i < size; i++) { if (_data[i] != oth._data[i]) { return false; } }; return true; }
//			bool     operator!=(const vec<T, size>& oth) const noexcept { return !(*this == oth); }
//
//			static T dot      (const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] * b[i]; } return ret; }
//			static T distance2(const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] - b[i]; } return ret; }
//			static T distance (const vec<T, size>& a, const vec<T, size>& b) noexcept { return std::sqrt(distance2(a, b)); }
//
//			// OTHER
//			
//			static vec<T, size> lerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
//				{
//				vec<T, size> ret;
//				for (size_t i{0}; i < size; i++) { ret[i] = utils::lerp(a[i], b[i], t); }
//				return ret;
//				}
//			static vec<T, size> slerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
//				{
//				return lerp(a, b).normalize() * (utils::lerp(a.magnitude(), b.magnitude(), t));
//				}
//			static vec<T, size> tlerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
//				{
//				return lerp(a, b).normalize() * std::sqrt(utils::lerp(a.magnitude2(), b.magnitude2(), t));
//				}
//			static vec<T, size> slerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept //TODO test
//				{
//				T dot = std::clamp(vec::dot(a, b), -1.0f, 1.0f);
//				T theta = std::acos(dot) * t;
//				vec<T, size> relative_vec = (b - a * dot).normalized();
//				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
//				}
//		};
//
//	template <typename T, size_t size>
//	vec<T, size> abs(const vec<T, size> vec) noexcept { return vec.abs(); }
//
//	namespace operators
//		{
//		inline constexpr struct _dot
//			{
//			template <typename T, size_t size>
//			class _inner;
//
//			template <typename T, size_t size>
//			inline friend _inner<T, size> operator<(vec<T, size> lhs, _dot proxy) noexcept { return {lhs}; }
//
//			template <typename T, size_t size>
//			class _inner
//				{
//				public:
//					T operator>(vec<T, size> rhs) const noexcept { return vec<T, size>::dot(lhs, rhs); }
//					_inner(vec<T, size> lhs) noexcept : lhs{lhs} {}
//				private:
//					vec<T, size> lhs;
//				};
//
//			} dot;
//
//		inline constexpr struct _cross
//			{
//			template <typename T, size_t size>
//			class _inner;
//
//			template <typename T, size_t size>
//			inline friend _inner<T, size> operator<(vec<T, size> lhs, _cross proxy) noexcept { return {lhs}; }
//
//			template <typename T, size_t size>
//			class _inner
//				{
//				public:
//					vec<T, size> operator>(vec<T, size> rhs) const noexcept { return lhs * rhs; }
//					_inner(vec<T, size> lhs) noexcept : lhs{lhs} {}
//				private:
//					vec<T, size> lhs;
//				};
//
//			} cross;
//		}
//	}
//namespace utils
//	{
//	template <typename T, size_t size>
//	inline math::vec<T, size> lerp(math::vec<T, size> a, math::vec<T, size> b, float t) { return math::vec<T, size>::lerp(a, b, t); }
//	template <typename T, size_t size>
//	inline math::vec<T, size> clamp(math::vec<T, size> in, math::vec<T, size> min, math::vec<T, size> max) { return math::vec<T, size>::clamp(in, min, max); }
//	template <typename T, size_t size>
//	inline math::vec<T, size> min(math::vec<T, size> in, math::vec<T, size> min) { return math::vec<T, size>::min(in, min); }
//	template <typename T, size_t size>
//	inline math::vec<T, size> max(math::vec<T, size> in, math::vec<T, size> max) { return math::vec<T, size>::max(in, max); }
//	}
