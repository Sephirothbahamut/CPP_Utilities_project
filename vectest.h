#pragma once
#include <concepts>
#include <algorithm>
#include <cmath>
#include <array>


namespace utils
	{
	template <typename T>
	inline T lerp(T a, T b, float t) { return (a * (1.f - t)) + (b * t); }

	template <template<typename> class To, typename T, class From>
	inline static To<T> vec_cast(From from) { return {static_cast<T>(from.x), static_cast<T>(from.y)}; }

	template <typename T, size_t SIZE, typename LEAF_T = void>
	class vec
		{
		public:
			inline static constexpr size_t size = SIZE;
			using leaf_t = LEAF_T;
			using root_t = vec<T, size, leaf_t>;

			vec() = default;

			template<std::same_as<T> ...Ts>
				requires(sizeof...(Ts) == size)
			vec(Ts ...values) : _data{values...} {}

			//template <typename other_t, size_t other_size>
			//vec(vec<other_t, other_size> other)           noexcept { containers::array::copy(_data, other._data); } 

			std::array<T, size> _data;

			//TODO add AT functions and call AT in [] in debug mode
			T& operator[](size_t index)       noexcept { return _data[index]; }
			const T& operator[](size_t index) const noexcept { return _data[index]; }

#pragma region operators
#pragma region on root
			bool operator==(const root_t& oth) const noexcept { for (size_t i{0}; i < size; i++) { if (_data[i] != oth[i]) { return false; } }; return true; }
			bool operator!=(const root_t& oth) const noexcept { return !(*this == oth); }
			static T dot(const root_t& a, const root_t& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] * b[i]; } return ret; }

#pragma endregion on root
#pragma region on leaf
			// OPERATORS
			leaf_t  operator-() const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = -static_cast<const leaf_t&>(*this)[i]; }; return ret; }

			// VEC & SCALAR OPERATORS
			leaf_t  operator++()       noexcept { return static_cast<leaf_t&>(*this) += T(1); }
			leaf_t  operator--()       noexcept { return static_cast<leaf_t&>(*this) -= T(1); }

			//leaf_t  operator+ (const T n) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] + n; }; return ret; }
			//leaf_t  operator- (const T n) const noexcept { leaf_t ret; for (size_t i{ 0 }; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] - n; }; return ret; }
			leaf_t  operator* (const T n) const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] * n; }; return ret; }
			leaf_t  operator/ (const T n) const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] / n; }; return ret; }

			leaf_t& operator+=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) + n; }
			leaf_t& operator-=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) - n; }
			leaf_t& operator*=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) * n; }
			leaf_t& operator/=(const T n)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) / n; }

			// VEC & VEC OPERATORS
			//TODO vec&vec operators when other has a different size

			leaf_t  operator+ (const leaf_t& oth) const noexcept { auto ret{static_cast<const leaf_t&>(*this)}; for (size_t i{0}; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] + oth[i]; }; return ret; }
			leaf_t  operator- (const leaf_t& oth) const noexcept { auto ret{static_cast<const leaf_t&>(*this)}; for (size_t i{0}; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] - oth[i]; }; return ret; }
			leaf_t  operator* (const leaf_t& oth) const noexcept { auto ret{static_cast<const leaf_t&>(*this)}; for (size_t i{0}; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] * oth[i]; }; return ret; }
			leaf_t  operator/ (const leaf_t& oth) const noexcept { auto ret{static_cast<const leaf_t&>(*this)}; for (size_t i{0}; i < size; i++) { ret[i] = static_cast<const leaf_t&>(*this)[i] / oth[i]; }; return ret; }

			leaf_t& operator+=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) + oth; }
			leaf_t& operator-=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) - oth; }
			leaf_t& operator*=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) * oth; }
			leaf_t& operator/=(const leaf_t& oth)       noexcept { return static_cast<leaf_t&>(*this) = static_cast<leaf_t&>(*this) / oth; }

#pragma endregion on leaf
#pragma endregion operators

			leaf_t  abs() const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = std::abs(_data[i]); } return ret; }
			leaf_t  min(leaf_t min) const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = std::min(_data[i], min._data[i]); } return ret; }
			leaf_t  max(leaf_t max) const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = std::max(_data[i], max._data[i]); } return ret; }
			leaf_t  clamp(leaf_t min, leaf_t max) const noexcept { leaf_t ret; for (size_t i{0}; i < size; i++) { ret[i] = std::clamp(_data[i], min._data[i], max._data[i]); } return ret; }

			leaf_t& abs_self()       noexcept { return static_cast<leaf_t&>(*this) = abs(); }
			leaf_t& min_self(leaf_t min)       noexcept { return static_cast<leaf_t&>(*this) = this->min(min); }
			leaf_t& max_self(leaf_t max)       noexcept { return static_cast<leaf_t&>(*this) = this->max(max); }
			leaf_t& clamp_self(leaf_t min, leaf_t max)       noexcept { return static_cast<leaf_t&>(*this) = clamp(min, max); }


			static leaf_t lerp(const leaf_t& a, const leaf_t& b, T t) noexcept
				{
				leaf_t ret;
				for (size_t i{0}; i < size; i++) { ret[i] = utils::lerp(a[i], b[i], t); } //TODO decomment
				return ret;
				}
		};

	namespace math
		{
		namespace details
			{
			template<class derived_self_t, class T, size_t size>
			class vec_impl {};
			}

		template <typename T, size_t size>
		class vec;

		template <size_t size> using veci = vec<int, size>;
		template <size_t size> using veci8 = vec<int8_t, size>;
		template <size_t size> using veci16 = vec<int16_t, size>;
		template <size_t size> using veci32 = vec<int32_t, size>;
		template <size_t size> using veci64 = vec<int64_t, size>;

		template <size_t size> using vecu = vec<unsigned, size>;
		template <size_t size> using vecu8 = vec<uint8_t, size>;
		template <size_t size> using vecu16 = vec<uint16_t, size>;
		template <size_t size> using vecu32 = vec<uint32_t, size>;
		template <size_t size> using vecu64 = vec<uint64_t, size>;

		template <size_t size> using vecs = vec<size_t, size>;
		template <size_t size> using vecf = vec<float, size>;
		template <size_t size> using vecd = vec<double, size>;

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

				T get_length2() const noexcept { T ret{0}; for (const auto& element : utils::vec<T, size, vec<T, size>>::_data) { ret += element * element; } return ret; }
				T get_length() const noexcept { return std::sqrt(get_length2()); }

				vec<T, size>& set_length(T value) noexcept { *this = normalize() * value; return *this; }

				__declspec(property(get = get_length, put = set_length)) T length;

				vec<T, size>  normalize() const noexcept { return get_length() ? *this / get_length() : *this; }
				vec<T, size>& normalize_self()       noexcept { return *this = normalize(); }

				static T distance2(const vec<T, size>& a, const vec<T, size>& b) noexcept { T ret{0}; for (size_t i{0}; i < size; i++) { ret += a[i] - b[i]; } return ret; }
				static T distance(const vec<T, size>& a, const vec<T, size>& b) noexcept { return std::sqrt(distance2(a, b)); }

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

				T& get_x()       noexcept requires(size >= 1) { return this->_data[0]; }
				const T& get_x() const noexcept requires(size >= 1) { return this->_data[0]; }
				T& set_x(const T& value)       noexcept requires(size >= 1) { return this->_data[0] = value; }

				__declspec(property(get = get_x, put = set_x)) T x;
				__declspec(property(get = get_x, put = set_x)) T r;

				T& get_y()       noexcept requires(size >= 2) { return this->_data[1]; }
				const T& get_y() const noexcept requires(size >= 2) { return this->_data[1]; }
				T& set_y(const T& value)       noexcept requires(size >= 2) { return this->_data[1] = value; }

				__declspec(property(get = get_y, put = set_y)) T y;
				__declspec(property(get = get_y, put = set_y)) T g;

				T& get_z()       noexcept requires(size >= 3) { return this->_data[2]; }
				const T& get_z() const noexcept requires(size >= 3) { return this->_data[2]; }
				T& set_z(const T& value)       noexcept requires(size >= 3) { return this->_data[2] = value; }

				__declspec(property(get = get_z, put = set_z)) T z;
				__declspec(property(get = get_z, put = set_z)) T b;

				T& get_w()       noexcept requires(size >= 4) { return this->_data[3]; }
				const T& get_w() const noexcept requires(size >= 4) { return this->_data[3]; }
				T& set_w(const T& value)       noexcept requires(size >= 4) { return this->_data[3] = value; }

				__declspec(property(get = get_w, put = set_w)) T w;
				__declspec(property(get = get_w, put = set_w)) T a;


			};
		template <typename T>
		using vec2 = vec<T, 2>;

		//fast typenames
		using vec2i = vec2<int>;
		using vec2i8 = vec2<int8_t>;
		using vec2i16 = vec2<int16_t>;
		using vec2i32 = vec2<int32_t>;
		using vec2i64 = vec2<int64_t>;

		using vec2u = vec2<unsigned>;
		using vec2u8 = vec2<uint8_t>;
		using vec2u16 = vec2<uint16_t>;
		using vec2u32 = vec2<uint32_t>;
		using vec2u64 = vec2<uint64_t>;

		using vec2s = vec2<size_t>;
		using vec2f = vec2<float>;
		using vec2d = vec2<double>;

		namespace details
			{
			template<class leaf_t, class T>
			class vec_impl<leaf_t, T, 2>
				{
				public:
					//constructors from angle

					// VEC & ANGLE OPERATIONS
					//template <float full_angle_value> leaf_t  operator+ (const utils::math::angle::base_angle<full_angle_value> angle) const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {self.x * angle.cos() - self.y * angle.sin(), self.x * angle.sin() + self.y * angle.cos()}; }
					//template <float full_angle_value> leaf_t  operator- (const utils::math::angle::base_angle<full_angle_value> angle) const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {self.x * angle.cos() - self.y * angle.sin(), self.x * angle.sin() + self.y * angle.cos()}; }
					//template <float full_angle_value> leaf_t& operator+=(const utils::math::angle::base_angle<full_angle_value> angle)       noexcept { auto& self = static_cast<leaf_t&>(*this); return self = self + angle; }
					//template <float full_angle_value> leaf_t& operator-=(const utils::math::angle::base_angle<full_angle_value> angle)       noexcept { auto& self = static_cast<leaf_t&>(*this); return self = self - angle; }
					//template <float full_angle_value> leaf_t& operator= (const utils::math::angle::base_angle<full_angle_value> angle)       noexcept { auto& self = static_cast<leaf_t&>(*this); return self = {angle.cos() * self.magnitude(), angle.sin() * self.magnitude()}; }

					leaf_t operator+ (float angle) const noexcept { auto ret{static_cast<const leaf_t&>(*this)}; return ret; 
					// OTHER
					leaf_t perpendicular_right()            const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {self.y, -self.x}; }
					leaf_t perpendicular_left()             const noexcept { const auto& self = static_cast<const leaf_t&>(*this); return {-self.y,  self.x}; }
					leaf_t perpendicular_clockwise()        const noexcept { return perpendicular_right(); }
					leaf_t perpendicular_counterclockwise() const noexcept { return perpendicular_left(); }
				};
			}
		}

	}