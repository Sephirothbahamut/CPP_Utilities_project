#pragma once

#include "../vec.h"

namespace utils::math
	{
	namespace details
		{
		template<class derived_self_t, class T, size_t size>
		class vec_impl {};
		}

	template <typename T, size_t size>
	class vec;

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
	class vec : 
		public utils::vec<T, size, vec<T, size>>,
		public details::vec_impl<vec<T, size>, T, size>
		{
			template<class derived_self_t, class T, size_t size>
			friend class details::vec_impl;

		public:
			using details::vec_impl<vec<T, size>, T, size>::vec_impl;
			using utils::vec<T, size, vec<T, size>>::vec;

			T get_length2               () const noexcept { T ret{0}; for (const auto& element : utils::vec<T, size, vec<T, size>>::_data) { ret += element * element; } return ret; }
			T get_length                () const noexcept { return std::sqrt(get_length2()); }

			vec<T, size>& set_length    (T value) noexcept { *this = normalize() * value; return *this; }

			__declspec(property(get = get_length, put = set_length)) T length;

			vec<T, size>  normalize     () const noexcept { return get_length() ? *this / get_length() : *this; }
			vec<T, size>& normalize_self()       noexcept { return *this = normalize(); }

			static T distance2(const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] - b[i]; } return ret; }
			static T distance (const vec<T, size>& a, const vec<T, size>& b) noexcept { return std::sqrt(distance2(a, b)); }

			static vec<T, size> slerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				return lerp(a, b).normalize() * (utils::lerp(a.get_length(), b.get_length(), t));
				}
			static vec<T, size> tlerp_fast(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept
				{
				return lerp(a, b).normalize() * std::sqrt(utils::lerp(a.get_length2(), b.get_length2(), t));
				}
			static vec<T, size> slerp(const vec<T, size>& a, const vec<T, size>& b, T t) noexcept //TODO test
				{
				T dot = std::clamp(vec::dot(a, b), -1.0f, 1.0f);
				T theta = std::acos(dot) * t;
				vec<T, size> relative_vec = (b - a * dot).normalized();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

			static vec<T, 2> zero()     noexcept requires(size >= 1) { return {}; }

			static vec<T, 2> rr()       noexcept requires(size >= 1) { return {T{ 1}, T{ 0}}; }
			static vec<T, 2> ll()       noexcept requires(size >= 1) { return {T{-1}, T{ 0}}; }
			static vec<T, 2> right()    noexcept requires(size >= 1) { return rr(); }
			static vec<T, 2> left()     noexcept requires(size >= 1) { return ll(); }

			static vec<T, 2> up()       noexcept requires(size >= 2) { return {T{ 0}, T{-1}}; }
			static vec<T, 2> dw()       noexcept requires(size >= 2) { return {T{ 0}, T{ 1}}; }
			static vec<T, 2> down()     noexcept requires(size >= 2) { return dw(); }

			static vec<T, 2> fw()       noexcept requires(size >= 3) { return {T{ 0}, T{0}, T{ 1}}; }
			static vec<T, 2> bw()       noexcept requires(size >= 3) { return {T{ 0}, T{0}, T{-1}}; }
			static vec<T, 2> forward()  noexcept requires(size >= 3) { return fw(); }
			static vec<T, 2> backward() noexcept requires(size >= 3) { return bw(); }

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
			

		};

	}

	namespace utils
		{
		template <typename T, size_t size>
		inline math::vec<T, size> lerp(const math::vec<T, size>& a, const math::vec<T, size>& b, float t) { return math::vec<T, size>::lerp(a, b, t); }

		template <typename T, size_t size>
		inline math::vec<T, size> clamp(const math::vec<T, size>& in, const math::vec<T, size>& min, const math::vec<T, size>& max) { return in.clamp(min, max); }

		template <typename T, size_t size>
		inline math::vec<T, size> min(const math::vec<T, size>& in, const math::vec<T, size>& min) { return in.min(min); }

		template <typename T, size_t size>
		inline math::vec<T, size> max(const math::vec<T, size>& in, const math::vec<T, size>& max) { return in.max(max); }
		}