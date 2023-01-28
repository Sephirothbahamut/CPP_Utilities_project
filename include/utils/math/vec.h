#pragma once

#include <array>
#include <cmath>

#include "../details/vec/common.h"
#include "../details/vec/output.h"
#include "../details/vec/memberwise_operators.h"
#include "../details/vec/factories.h"

#include "../compilation/warnings.h"

//TODO finish test cases

namespace utils::math
	{
	template <typename T, size_t size>
	struct vec;

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
		inline extern constexpr const char vec_name[]{"vec"};

		template<class T, size_t size, typename DERIVED_T>
		class vec_sized_specialization { public: void from() = delete; };


		template<typename T, size_t SIZE, typename DERIVED_T>
		class vec_crtp :
			public utils::details::vec::array<T, SIZE, DERIVED_T>,
			public utils::details::vec::memberwise_operators<T, DERIVED_T>,
			public utils::details::vec::output<vec_name, DERIVED_T>,
			public utils::details::vec::factories<T, SIZE, DERIVED_T>,
			public vec_sized_specialization<T, SIZE, DERIVED_T>
			{
			public:
				using derived_t = DERIVED_T;

			private:
				constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				constexpr       derived_t& derived()       noexcept { return static_cast<derived_t&>(*this); }
				constexpr const auto     & get_arr() const noexcept { return derived().array; }
				constexpr       auto     & get_arr()       noexcept { return derived().array; }

				using arr_t = std::array<T, SIZE>;

			public:
				inline static constexpr const size_t static_size{std::tuple_size_v<arr_t>};
				using value_type = typename arr_t::value_type;

				constexpr value_type get_length2() const noexcept { value_type ret{0}; for (const auto& element : derived()) { ret += element * element; } return ret; }
				constexpr value_type get_length () const noexcept { return std::sqrt(get_length2()); }

				constexpr derived_t& set_length    (value_type value) noexcept { derived() = normalize() * value; return derived(); }

				__declspec(property(get = get_length, put = set_length)) value_type length;

				constexpr derived_t  normalize     () const noexcept { return get_length() ? derived() / get_length() : derived(); }
				constexpr derived_t& normalize_self()       noexcept { return derived() = normalize(); }

				using utils::details::vec::factories<T, SIZE, DERIVED_T>::from;
				using vec_sized_specialization<T, SIZE, DERIVED_T>      ::from;

	#pragma region distances
				/// <summary> Evaluate distance^2 in the size of this vec. Missing coordinates are considered 0. </summary>
				template <utils::details::vec::concepts::compatible_array<derived_t> T2>
				inline static constexpr value_type distance2(const derived_t& a, const T2& b) noexcept
					{
					value_type ret{0};
					size_t i{0};
					for (; i < std::min({derived_t::static_size, a.size(), b.size()}); i++)
						{
						value_type tmp{a[i] - b[i]};
						ret += tmp * tmp;
						}
					
					     if constexpr (a.size() > b.size()) { for (; i < std::min(derived_t::static_size, a.size()); i++) { ret += a[i] * a[i]; } }
					else if constexpr (a.size() < b.size()) { for (; i < std::min(derived_t::static_size, b.size()); i++) { ret += b[i] * b[i]; } }

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
					return lerp(a, b).normalize() * (utils::math::lerp(a.get_length(), b.get_length(), t));
					}
				inline static constexpr derived_t tlerp_fast(const derived_t& a, const derived_t& b, value_type t) noexcept
					{
					return lerp(a, b).normalize() * std::sqrt(utils::math::lerp(a.get_length2(), b.get_length2(), t));
					}
				inline static constexpr derived_t slerp(const derived_t& a, const derived_t& b, value_type t) noexcept //TODO test
					{
					value_type dot = utils::math::clamp(vec::dot(a, b), -1.0f, 1.0f);
					value_type theta = std::acos(dot) * t;
					derived_t relative_vec = (b - a * dot).normalized();
					return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
					}

	#pragma endregion interpolation
	#pragma region factories
				static derived_t zero    () noexcept requires(static_size >= 1) { return {value_type{ 0}}; }

				static derived_t rr      () noexcept requires(static_size == 1) { return {value_type{ 1}}; }
				static derived_t ll      () noexcept requires(static_size == 1) { return {value_type{-1}}; }
				static derived_t rr      () noexcept requires(static_size >  1) { return {value_type{ 1}, value_type{ 0}}; }
				static derived_t ll      () noexcept requires(static_size >  1) { return {value_type{-1}, value_type{ 0}}; }
				static derived_t right   () noexcept requires(static_size >= 1) { return rr(); }
				static derived_t left    () noexcept requires(static_size >= 1) { return ll(); }

				static derived_t up      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{-1}}; }
				static derived_t dw      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{ 1}}; }
				static derived_t up      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{-1}, value_type{ 0}}; }
				static derived_t dw      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
				static derived_t down    () noexcept requires(static_size >= 2) { return dw(); }

				static derived_t fw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}}; }
				static derived_t bw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}}; }
				static derived_t fw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
				static derived_t bw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}, value_type{ 0}}; }
				static derived_t forward () noexcept requires(static_size >= 3) { return fw(); }
				static derived_t backward() noexcept requires(static_size >= 3) { return bw(); }

	#pragma region factories
				
				template <concepts::vec b_t>
				static value_type dot(const derived_t& a, const b_t& b) noexcept
					requires std::convertible_to<value_type, typename b_t::value_type>
					{
					value_type ret{0}; for (size_t i{0}; i < static_size; i++) 
						{
						ret += a[i] * b[i];
						} 
					return ret;
					}
		
			private:
			};
		}

	#define utils_vec_hell_let_loose
	
	#ifdef utils_vec_hell_let_loose
		// While akshually UB, this usage of unions is explicitly supported by gcc and according to various posts it's used in MS headers, which would imply it's supported by MSVC as well.
		// The advantage is not only simplicity, but also that all operations are supported as you would naively expect them to.
		// For instance, offsetof(y) works as expected, while it doesn't with the other versions.

		template <typename T, size_t size>
		struct vec : details::vec_crtp<T, size, vec<T, size>> { std::array<T, size> array; };

		utils_disable_warnings_begin
			utils_disable_warning_clang("-Wgnu-anonymous-struct")
			utils_disable_warning_clang("-Wnested-anon-types")
			utils_disable_warning_gcc("-Wpedantic")

			template<typename T> struct vec<T, 1> : details::vec_crtp<T, 1, vec<T, 1>> { union { std::array<T, 1> array; struct {T x         ; }; }; };
			template<typename T> struct vec<T, 2> : details::vec_crtp<T, 2, vec<T, 2>> { union { std::array<T, 2> array; struct {T x, y      ; }; }; };
			template<typename T> struct vec<T, 3> : details::vec_crtp<T, 3, vec<T, 3>> { union { std::array<T, 3> array; struct {T x, y, z   ; }; }; };
			template<typename T> struct vec<T, 4> : details::vec_crtp<T, 4, vec<T, 4>> { union { std::array<T, 4> array; struct {T x, y, z, w; }; }; };

		utils_disable_warnings_end

	#elif defined utils_vec_standard_compliant
		// Standard compliant version but doesn't behave as expected in some cases like the previously mentioned offsetof(y)

		template <typename T, size_t size>
		struct vec : details::vec_crtp<T, size, vec<T, size>> { std::array<T, size> array; };

		//Note: use for field with [[no_unique_address]]
		template <class T, class DERIVED_T, size_t index>
		struct array_element_alias 
			{
			auto operator=(std::convertible_to<T> auto&& b) -> T& { return reinterpret_cast<DERIVED_T*>(this)->data[index] = static_cast<decltype(b)&&>(b); }
	
			operator T&() { return reinterpret_cast<DERIVED_T*>(this)->data[index]; }
	
			operator T const&() const { return reinterpret_cast<DERIVED_T*>(this)->data[index]; }
			};
	
		template<typename T> 
		struct vec1 
			{
			[[no_unique_address]] array_element_alias<T, vec1<T>, 0> x{};
			std::array<T, 1> array;
			};
		template<typename T> 
		struct vec2 
			{
			[[no_unique_address]] array_element_alias<T, vec2<T>, 0> x{};
			[[no_unique_address]] array_element_alias<T, vec2<T>, 1> y{};
			std::array<T, 2> array;
			};
		template<typename T> 
		struct vec3 
			{
			[[no_unique_address]] array_element_alias<T, vec2<T>, 0> x{};
			[[no_unique_address]] array_element_alias<T, vec2<T>, 1> y{};
			[[no_unique_address]] array_element_alias<T, vec2<T>, 2> z{};
			std::array<T, 3> array;
			};
		template<typename T> 
		struct vec4 
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
		struct vec : details::vec_crtp<T, size, vec<T, size>> 
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