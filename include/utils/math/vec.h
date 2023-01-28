#pragma once

#include <array>
#include <cmath>
#include <functional> //reference_wrapper

#include "../memory.h"
#include "math.h"
#include "../details/vec/common.h"
#include "../details/vec/output.h"
#include "../details/vec/memberwise_operators.h"

#include "../compilation/warnings.h"

//TODO finish test cases

namespace utils::math
	{
	template <typename T, size_t size>
	struct vec;
	
	//fast typenames
	template <typename T, size_t size> 
	using vecref = vec<utils::reference<T>, size>;
	
	template <size_t size> using vec_i      = vec   <int          , size>;
	template <size_t size> using vec_i8     = vec   <int8_t       , size>;
	template <size_t size> using vec_i16    = vec   <int16_t      , size>;
	template <size_t size> using vec_i32    = vec   <int32_t      , size>;
	template <size_t size> using vec_i64    = vec   <int64_t      , size>;
	template <size_t size> using vec_u      = vec   <unsigned     , size>;
	template <size_t size> using vec_u8     = vec   <uint8_t      , size>;
	template <size_t size> using vec_u16    = vec   <uint16_t     , size>;
	template <size_t size> using vec_u32    = vec   <uint32_t     , size>;
	template <size_t size> using vec_u64    = vec   <uint64_t     , size>;
	template <size_t size> using vec_s      = vec   <size_t       , size>;
	template <size_t size> using vec_f      = vec   <float        , size>;
	template <size_t size> using vec_d      = vec   <double       , size>;
	template <size_t size> using vec_l      = vec   <long         , size>;
	template <size_t size> using vec_ul     = vec   <unsigned long, size>;
	template <size_t size> using vecref_i   = vecref<int          , size>;
	template <size_t size> using vecref_i8  = vecref<int8_t       , size>;
	template <size_t size> using vecref_i16 = vecref<int16_t      , size>;
	template <size_t size> using vecref_i32 = vecref<int32_t      , size>;
	template <size_t size> using vecref_i64 = vecref<int64_t      , size>;
	template <size_t size> using vecref_u   = vecref<unsigned     , size>;
	template <size_t size> using vecref_u8  = vecref<uint8_t      , size>;
	template <size_t size> using vecref_u16 = vecref<uint16_t     , size>;
	template <size_t size> using vecref_u32 = vecref<uint32_t     , size>;
	template <size_t size> using vecref_u64 = vecref<uint64_t     , size>;
	template <size_t size> using vecref_s   = vecref<size_t       , size>;
	template <size_t size> using vecref_f   = vecref<float        , size>;
	template <size_t size> using vecref_d   = vecref<double       , size>;
	template <size_t size> using vecref_l   = vecref<long         , size>;
	template <size_t size> using vecref_ul  = vecref<unsigned long, size>;
	
	namespace concepts
		{
		template <typename T>
		concept vec = std::derived_from<T, utils::math::vec<typename T::value_type, T::static_size>>;
		}
	namespace details
		{
		inline extern constexpr const char vec_name[]{"vec"};

		template<class T, size_t size, typename DERIVED_T>
		class vec_sized_specialization {};

		#define utils_vec_hell_let_loose
	
		#ifdef utils_vec_hell_let_loose
			// While akshually UB, this usage of unions is explicitly supported by gcc and according to various posts it's used in MS headers, which would imply it's supported by MSVC as well.
			// The advantage is not only simplicity, but also that all operations are supported as you would naively expect them to.
			// For instance, offsetof(y) works as expected, while it doesn't with the other versions.

		template <typename T, size_t size>
		struct vec_named { std::array<T, size> array; };

		utils_disable_warnings_begin
			utils_disable_warning_clang("-Wgnu-anonymous-struct")
			utils_disable_warning_clang("-Wnested-anon-types")
			utils_disable_warning_gcc("-Wpedantic")

			template<typename T> struct vec_named<T, 1> { union { std::array<T, 1> array; struct { T x         ; }; }; };
			template<typename T> struct vec_named<T, 2> { union { std::array<T, 2> array; struct { T x, y      ; }; }; };
			template<typename T> struct vec_named<T, 3> { union { std::array<T, 3> array; struct { T x, y, z   ; }; }; };
			template<typename T> struct vec_named<T, 4> { union { std::array<T, 4> array; struct { T x, y, z, w; }; }; };
		utils_disable_warnings_end

		#elif defined utils_vec_standard_compliant
			// Standard compliant version but doesn't behave as expected in some cases like the previously mentioned offsetof(y)

			template <typename T, size_t size>
			struct vec_named { std::array<T, size> array; };

			//Note: use for field with [[no_unique_address]]
			template <class T, class DERIVED_T, size_t index>
			struct array_element_alias 
				{
				auto operator=(std::convertible_to<T> auto&& b) -> T& { return reinterpret_cast<DERIVED_T*>(this)->data[index] = static_cast<decltype(b)&&>(b); }
	
				operator T&() { return reinterpret_cast<DERIVED_T*>(this)->data[index]; }
	
				operator T const&() const { return reinterpret_cast<DERIVED_T*>(this)->data[index]; }
				};
	
			template<typename T> 
			struct vec_named<T, 1>
				{
				[[no_unique_address]] array_element_alias<T, vec1<T>, 0> x{};
				std::array<T, 1> array;
				};
			template<typename T>
			struct vec_named<T, 2>
				{
				[[no_unique_address]] array_element_alias<T, vec2<T>, 0> x{};
				[[no_unique_address]] array_element_alias<T, vec2<T>, 1> y{};
				std::array<T, 2> array;
				};
			template<typename T>
			struct vec_named<T, 3>
				{
				[[no_unique_address]] array_element_alias<T, vec2<T>, 0> x{};
				[[no_unique_address]] array_element_alias<T, vec2<T>, 1> y{};
				[[no_unique_address]] array_element_alias<T, vec2<T>, 2> z{};
				std::array<T, 3> array;
				};
			template<typename T>
			struct vec_named<T, 4>
				{
				[[no_unique_address]] array_element_alias<T, vec2<T>, 0> x{};
				[[no_unique_address]] array_element_alias<T, vec2<T>, 1> y{};
				[[no_unique_address]] array_element_alias<T, vec2<T>, 2> z{};
				[[no_unique_address]] array_element_alias<T, vec2<T>, 3> w{};
				std::array<T, 4> array;
				};

		#elif defined utils_vec_ms_properties
			// This version uses MSVC's properties language extension, which is also supported by clang, but not supported by gcc.
			// Still doesn't behave as expected in some cases like the previously mentioned offsetof(y)

			template <typename T, size_t size>
			struct vec_named
				{
				std::array<T, size> array;

	#pragma region fields
					  T& get_x(              )       noexcept requires(size >= 1) { return (*this)[0]; }
				const T& get_x(              ) const noexcept requires(size >= 1) { return (*this)[0]; }
					  T& set_x(const T& value)       noexcept requires(size >= 1) { return (*this)[0] = value; }
				  
				__declspec(property(get = get_x, put = set_x)) T x;

					  T& get_y(              )       noexcept requires(size >= 2) { return (*this)[1]; }
				const T& get_y(              ) const noexcept requires(size >= 2) { return (*this)[1]; }
					  T& set_y(const T& value)       noexcept requires(size >= 2) { return (*this)[1] = value; }

				__declspec(property(get = get_y, put = set_y)) T y;

					  T& get_z(              )       noexcept requires(size >= 3) { return (*this)[2]; }
				const T& get_z(              ) const noexcept requires(size >= 3) { return (*this)[2]; }
					  T& set_z(const T& value)       noexcept requires(size >= 3) { return (*this)[2] = value; }

				__declspec(property(get = get_z, put = set_z)) T z;

					  T& get_w(              )       noexcept requires(size >= 4) { return (*this)[3]; }
				const T& get_w(              ) const noexcept requires(size >= 4) { return (*this)[3]; }
					  T& set_w(const T& value)       noexcept requires(size >= 4) { return (*this)[3] = value; }

				__declspec(property(get = get_w, put = set_w)) T w;
	#pragma endregion fields
				};
		#endif
		}

	template<typename T, size_t size>
	class vec:
		public details::vec_named<T, size>,
		public utils::details::vec::common<T, size, vec<T, size>>,
		public utils::details::vec::memberwise_operators<T, size, vec, vec<T, size>>,
		public utils::details::vec::output<details::vec_name, vec<T, size>>,
		public details::vec_sized_specialization<T, size, vec<T, size>>
		{
		public:
			using derived_t = vec<T, size>;

		private:
			using arr_t = std::array<T, size>;

		public:
			inline static constexpr const size_t static_size{size};
			using value_type = typename arr_t::value_type;

#pragma region constructors
			using details::vec_sized_specialization<T, size, derived_t>::vec_sized_specialization;
			
			//special case for vec of references
			template <std::same_as<typename value_type::value_type>... Args>
				requires(utils::concepts::reference<value_type> && sizeof...(Args) == static_size)
			constexpr vec(Args&... args) : details::vec_named<T, size>{.array{args...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) >= static_size)
			constexpr vec(const Args&... args) : details::vec_named<T, size>{.array{static_cast<value_type>(args)...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) < static_size)
			constexpr vec(const Args&... args) : details::vec_named<T, size>{.array{static_cast<value_type>(args)...}} 
				{
				for (size_t i = sizeof...(Args); i < static_size; i++)
					{
					if constexpr (sizeof...(Args)) { this->array[i] = this->array[sizeof...(Args) - 1]; }
					else { this->array[i] = T{0}; }
					}
				}
			
			//special case for vec of references
			template <concepts::vec other_t>
				requires(utils::concepts::reference<value_type> && std::same_as<typename other_t::value_type, typename value_type::value_type> && other_t::static_size == static_size)
			constexpr vec(other_t& other) : details::vec_named<T, size>{.array{std::apply([](auto&... values) { return std::array<value_type, size>{values...}; }, other)}} {}

			template <concepts::vec other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size == static_size)
			constexpr vec(const other_t& other) : details::vec_named<T, size>{.array{std::apply([](const auto&... values) { return std::array<value_type, size>{static_cast<value_type>(values)...}; }, other.array)}} {}
			
			template <concepts::vec other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size != static_size && utils::concepts::default_constructible<value_type>)
			constexpr vec(const other_t& other, value_type default_value = value_type{0})
				{
				size_t i{0};
				for (; i < std::min(static_size, other_t::static_size); i++)
					{
					this->array[i] = static_cast<value_type>(other[i]);
					}
				for (size_t i = other.size(); i < static_size; i++)
					{
					if constexpr (other_t::static_size) { this->array[i] = default_value; }
					else { this->array[i] = default_value; }
					}
				}
#pragma endregion constructors

			constexpr value_type get_length2() const noexcept { value_type ret{0}; for (const auto& element : this->array) { ret += element * element; } return ret; }
			constexpr value_type get_length () const noexcept { return std::sqrt(get_length2()); }

			constexpr derived_t& set_length    (value_type value) noexcept { *this = normalize() * value; return *this; }

			__declspec(property(get = get_length, put = set_length)) value_type length;

			constexpr derived_t  normalize     () const noexcept { return get_length() ? *this / get_length() : *this; }
			constexpr derived_t& normalize_self()       noexcept { return *this = normalize(); }


#pragma region distances
			/// <summary> Evaluate distance^2 in the size of this vec. Missing coordinates are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			inline static constexpr value_type distance2(const derived_t& a, const T2& b) noexcept
				{
				value_type ret{0};
				size_t i{0};
				for (; i < std::min({static_size, a.size(), b.size()}); i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}
					
					    if constexpr (a.size() > b.size()) { for (; i < std::min(static_size, a.size()); i++) { ret += a[i] * a[i]; } }
				else if constexpr (a.size() < b.size()) { for (; i < std::min(static_size, b.size()); i++) { ret += b[i] * b[i]; } }

				return ret;
				}

			/// <summary> Evaluate distance^2 in all the axes of the smaller vec. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			inline static constexpr value_type distance2_shared(const derived_t& a, const T2& b) noexcept
				{
				value_type ret{0};
				size_t i{0};
				for (; i < std::min(a.size(), b.size()); i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}

				return ret;
				}

			/// <summary> Evaluate distance^2 in all the axes of the larger vec. Missing coordinates for the smaller one are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			inline static constexpr value_type distance2_complete(const derived_t& a, const T2& b) noexcept
				{
				value_type ret{0};
				size_t i{0};
				for (; i < std::min(a.size(), b.size()); i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}

					    if constexpr (a.size() > b.size()) { for (; i < a.size(); i++) { ret += a[i] * a[i]; } }
				else if constexpr (a.size() < b.size()) { for (; i < b.size(); i++) { ret += b[i] * b[i]; } }

				return ret;
				}
			/// <summary> Evaluate distance in the size of this vec. Missing coordinates are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			inline static constexpr value_type distance(const derived_t& a, const T2& b) noexcept { return std::sqrt(distance2(a, b)); }

			/// <summary> Evaluate distance in all the axes of the smaller vec. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			inline static constexpr value_type distance_shared(const derived_t& a, const T2& b) noexcept { return std::sqrt(distance_shared2(a, b)); }

			/// <summary> Evaluate distance in all the axes of the larger vec. Missing coordinates for the smaller one are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			inline static constexpr value_type distance_complete(const derived_t& a, const T2& b) noexcept { return std::sqrt(distance_complete2(a, b)); }

#pragma endregion distances
#pragma region interpolation

			inline static constexpr derived_t slerp_fast(const derived_t& a, const derived_t& b, value_type t) noexcept
				{
				return utils::math::lerp(a, b, t).normalize() * (utils::math::lerp(a.get_length(), b.get_length(), t));
				}
			inline static constexpr derived_t tlerp_fast(const derived_t& a, const derived_t& b, value_type t) noexcept
				{
				return utils::math::lerp(a, b, t).normalize() * std::sqrt(utils::math::lerp(a.get_length2(), b.get_length2(), t));
				}
			inline static constexpr derived_t slerp(const derived_t& a, const derived_t& b, value_type t) noexcept //TODO test
				{
				value_type dot = utils::math::clamp(vec::dot(a, b), -1.0f, 1.0f);
				value_type theta = std::acos(dot) * t;
				derived_t relative_vec = (b - a * dot).normalize();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

#pragma endregion interpolation
#pragma region factories
			static constexpr derived_t zero    () noexcept requires(static_size >= 1) { return {value_type{ 0}}; }

			static constexpr derived_t rr      () noexcept requires(static_size == 1) { return {value_type{ 1}}; }
			static constexpr derived_t ll      () noexcept requires(static_size == 1) { return {value_type{-1}}; }
			static constexpr derived_t rr      () noexcept requires(static_size >  1) { return {value_type{ 1}, value_type{ 0}}; }
			static constexpr derived_t ll      () noexcept requires(static_size >  1) { return {value_type{-1}, value_type{ 0}}; }
			static constexpr derived_t right   () noexcept requires(static_size >= 1) { return rr(); }
			static constexpr derived_t left    () noexcept requires(static_size >= 1) { return ll(); }

			static constexpr derived_t up      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{-1}}; }
			static constexpr derived_t dw      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{ 1}}; }
			static constexpr derived_t up      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			static constexpr derived_t dw      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			static constexpr derived_t down    () noexcept requires(static_size >= 2) { return dw(); }

			static constexpr derived_t fw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}}; }
			static constexpr derived_t bw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}}; }
			static constexpr derived_t fw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			static constexpr derived_t bw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			static constexpr derived_t forward () noexcept requires(static_size >= 3) { return fw(); }
			static constexpr derived_t backward() noexcept requires(static_size >= 3) { return bw(); }

#pragma region factories
				
			template <concepts::vec b_t>
			static constexpr value_type dot(const derived_t& a, const b_t& b) noexcept
				requires std::convertible_to<value_type, typename b_t::value_type>
				{
				value_type ret{0}; for (size_t i{0}; i < static_size; i++) 
					{
					ret += a[i] * b[i];
					} 
				return ret;
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
					typename a_t::value_type operator>(const b_t& rhs) const noexcept  { return a_t::dot(lhs, rhs); }
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