#pragma once

#include "../containers/memberwise_operators_array.h"

//TODO finish test cases

namespace utils::math
	{
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

	namespace concepts
		{
		template <typename T>
		concept vec = std::derived_from<T, utils::math::vec<typename T::value_type, T::static_size>>;
		}
	namespace details
		{
		template<class derived_self_t, class T, size_t size>
		class vec_impl {};
		}

	template <typename T, size_t SIZE>
	class vec : 
		public utils::containers::memberwise_operators::arr<T, SIZE>,
		public details::vec_impl<vec<T, SIZE>, T, SIZE>
		{
		template<class derived_self_t, class T, size_t SIZE>
		friend class details::vec_impl;

		public:
			using value_type = utils::containers::memberwise_operators::arr<T, SIZE>::value_type;
			inline static constexpr size_t static_size{utils::containers::memberwise_operators::arr<T, SIZE>::static_size};

			vec() : utils::containers::memberwise_operators::arr<T, SIZE>{value_type{ 0}} {}
			using details::vec_impl<vec<value_type, static_size>, value_type, static_size>::vec_impl;
			using utils::containers::memberwise_operators::arr<value_type, static_size>::arr;

			value_type get_length2               () const noexcept { value_type ret{0}; for (const auto& element : (*this)) { ret += element * element; } return ret; }
			value_type get_length                () const noexcept { return std::sqrt(get_length2()); }

			vec<value_type, static_size>& set_length    (value_type value) noexcept { *this = normalize() * value; return *this; }

			__declspec(property(get = get_length, put = set_length)) value_type length;

			vec<value_type, static_size>  normalize     () const noexcept { return get_length() ? *this / get_length() : *this; }
			vec<value_type, static_size>& normalize_self()       noexcept { return *this = normalize(); }

			static value_type distance2(const vec<value_type, static_size>& a, const vec<value_type, static_size>& b) noexcept { value_type ret{0}; for (size_t i{0}; i < static_size; i++) { ret += a[i] - b[i]; } return ret; }
			static value_type distance (const vec<value_type, static_size>& a, const vec<value_type, static_size>& b) noexcept { return std::sqrt(distance2(a, b)); }

			static vec<value_type, static_size> slerp_fast(const vec<value_type, static_size>& a, const vec<value_type, static_size>& b, value_type t) noexcept
				{
				return lerp(a, b).normalize() * (utils::math::lerp(a.get_length(), b.get_length(), t));
				}
			static vec<value_type, static_size> tlerp_fast(const vec<value_type, static_size>& a, const vec<value_type, static_size>& b, value_type t) noexcept
				{
				return lerp(a, b).normalize() * std::sqrt(utils::math::lerp(a.get_length2(), b.get_length2(), t));
				}
			static vec<value_type, static_size> slerp(const vec<value_type, static_size>& a, const vec<value_type, static_size>& b, value_type t) noexcept //TODO test
				{
				value_type dot = utils::math::clamp(vec::dot(a, b), -1.0f, 1.0f);
				value_type theta = std::acos(dot) * t;
				vec<value_type, static_size> relative_vec = (b - a * dot).normalized();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

#pragma region factories
			static vec<value_type, static_size> zero    () noexcept requires(static_size >= 1) { return {value_type{ 0}}; }

			static vec<value_type, static_size> rr      () noexcept requires(static_size == 1) { return {value_type{ 1}}; }
			static vec<value_type, static_size> ll      () noexcept requires(static_size == 1) { return {value_type{-1}}; }
			static vec<value_type, static_size> rr      () noexcept requires(static_size >  1) { return {value_type{ 1}, value_type{ 0}}; }
			static vec<value_type, static_size> ll      () noexcept requires(static_size >  1) { return {value_type{-1}, value_type{ 0}}; }
			static vec<value_type, static_size> right   () noexcept requires(static_size >= 1) { return rr(); }
			static vec<value_type, static_size> left    () noexcept requires(static_size >= 1) { return ll(); }

			static vec<value_type, static_size> up      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{-1}}; }
			static vec<value_type, static_size> dw      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{ 1}}; }
			static vec<value_type, static_size> up      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			static vec<value_type, static_size> dw      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			static vec<value_type, static_size> down    () noexcept requires(static_size >= 2) { return dw(); }

			static vec<value_type, static_size> fw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}}; }
			static vec<value_type, static_size> bw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}}; }
			static vec<value_type, static_size> fw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			static vec<value_type, static_size> bw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			static vec<value_type, static_size> forward () noexcept requires(static_size >= 3) { return fw(); }
			static vec<value_type, static_size> backward() noexcept requires(static_size >= 3) { return bw(); }

#pragma region factories
#pragma region fields
			      value_type& get_x(                       )       noexcept requires(static_size >= 1) { return (*this)[0]; }
			const value_type& get_x(                       ) const noexcept requires(static_size >= 1) { return (*this)[0]; }
			      value_type& set_x(const value_type& value)       noexcept requires(static_size >= 1) { return (*this)[0] = value; }
				  
			__declspec(property(get = get_x, put = set_x)) value_type x;

			      value_type& get_y(                       )       noexcept requires(static_size >= 2) { return (*this)[1]; }
			const value_type& get_y(                       ) const noexcept requires(static_size >= 2) { return (*this)[1]; }
			      value_type& set_y(const value_type& value)       noexcept requires(static_size >= 2) { return (*this)[1] = value; }

			__declspec(property(get = get_y, put = set_y)) value_type y;

			      value_type& get_z(                       )       noexcept requires(static_size >= 3) { return (*this)[2]; }
			const value_type& get_z(                       ) const noexcept requires(static_size >= 3) { return (*this)[2]; }
			      value_type& set_z(const value_type& value)       noexcept requires(static_size >= 3) { return (*this)[2] = value; }

			__declspec(property(get = get_z, put = set_z)) value_type z;

			      value_type& get_w(                       )       noexcept requires(static_size >= 4) { return (*this)[3]; }
			const value_type& get_w(                       ) const noexcept requires(static_size >= 4) { return (*this)[3]; }
			      value_type& set_w(const value_type& value)       noexcept requires(static_size >= 4) { return (*this)[3] = value; }

			__declspec(property(get = get_w, put = set_w)) value_type w;
#pragma endregion fields

			template <concepts::vec b_t>
			static value_type dot(const vec<value_type, static_size>& a, const b_t& b) noexcept
				requires std::convertible_to<decltype(a[0] * b[0]), value_type>
				{
				value_type ret{0}; for (size_t i{0}; i < static_size; i++) 
					{
					ret += a[i] * b[i];
					} return ret;
				}
		};

	namespace operators
		{
		inline constexpr struct _dot
			{
			template <concepts::vec a_t>
			class _inner;

			template <concepts::vec a_t>
			inline friend _inner<a_t> operator<(const a_t& lhs, _dot proxy) noexcept { return {lhs}; }

			template <concepts::vec a_t>
			class _inner
				{
				public:
					template <concepts::vec b_t>
					a_t::value_type operator>(const b_t& rhs) const noexcept  { return a_t::dot(lhs, rhs); }
					_inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <concepts::vec a_t>
			class _inner;

			template <concepts::vec a_t>
			inline friend _inner<a_t> operator<(const a_t& lhs, _cross proxy) noexcept { return {lhs}; }

			template <concepts::vec a_t>
			class _inner
				{
				public:
					template <concepts::vec b_t>
					a_t operator>(const b_t& rhs) const noexcept { return lhs * rhs; }
					_inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} cross;
		}
	}

namespace utils::output
	{
	namespace typeless
		{
		template <typename T, size_t SIZE>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::vec<T, SIZE>& container)
			{
			return utils::output::typeless::operator<<(os, static_cast<const std::array<T, SIZE>&>(container));
			}
		}

	template <typename T, size_t SIZE>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::math::vec<T, SIZE>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "vec" << SIZE << typeid(T).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}