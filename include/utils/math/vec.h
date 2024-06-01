#pragma once

#include <array>
#include <cmath>
#include <functional> //reference_wrapper

#include "math.h"
#include "../memory.h"
#include "../details/vec/common.h"
#include "../details/vec/output.h"
#include "../details/vec/memberwise_operators.h"

#include "../compilation/warnings.h"

namespace utils::math
	{
	template <typename T, size_t size>
	class vec;
	
	//fast typenames
	template <typename T, size_t size> 
	using vecref = vec<std::reference_wrapper<T>, size>;
	
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

		template<class T, size_t size, typename derived_T>
		class vec_sized_specialization {};

		#define utils_vec_hell_let_loose
	
		#ifdef utils_vec_hell_let_loose
			// While akshually UB, this usage of unions is explicitly supported by gcc and according to various posts it's used in MS headers, which would imply it's supported by MSVC as well.
			// The advantage is not only simplicity, but also that all operations are supported as you would naively expect them to.
			// For instance, offsetof(y) works as expected, while it doesn't with the other versions.

		template <typename T, size_t size>
		struct vec_named { std::array<T, size> array; };

		utils_disable_warnings_begin
			utils_disable_warning_msvc(4201)
			utils_disable_warning_clang("-Wgnu-anonymous-struct")
			utils_disable_warning_clang("-Wnested-anon-types")
			utils_disable_warning_gcc("-Wpedantic")

			template<typename T> struct vec_named<T, 1> { union { std::array<T, 1> array; struct { T x         ; }; }; };
			template<typename T> struct vec_named<T, 2> { union { std::array<T, 2> array; struct { T x, y      ; }; }; };
			template<typename T> struct vec_named<T, 3> { union { std::array<T, 3> array; struct { T x, y, z   ; }; }; };
			template<typename T> struct vec_named<T, 4> { union { std::array<T, 4> array; struct { T x, y, z, w; }; }; };
		utils_disable_warnings_end

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

		struct pair_sizes_t
			{
			size_t a{0};
			size_t b{0};
			size_t min{0};
			};

		template <typename a_T, typename b_T>
		consteval pair_sizes_t pair_sizes(const a_T& a, const b_T& b) noexcept
			{
			return
				{
				.a{a_T::static_size},
				.b{b_T::static_size},
				.min{utils::math::min(a_T::static_size, b_T::static_size)}
				};
			}

		template <typename a_T, typename b_T>
		consteval pair_sizes_t pair_sizes() noexcept
			{
			return
				{
				.a{a_T::static_size},
				.b{b_T::static_size},
				.min{utils::math::min(a_T::static_size, b_T::static_size)}
				};
			}
		}

	template<typename T, size_t SIZE>
	class utils_oop_empty_bases vec:
		public details::vec_named<T, SIZE>,
		public utils::details::vec::common<T, SIZE, vec>,
		public utils::details::vec::memberwise_operators<utils::details::vec::common<T, SIZE, vec>>,
		public utils::details::vec::output<details::vec_name, vec<T, SIZE>>,
		public details::vec_sized_specialization<T, SIZE, vec<T, SIZE>>
		{
		template<class T, size_t SIZE, typename derived_T>
		friend class details::vec_sized_specialization;

		protected:
			//using common_t = typename utils::details::vec::common<T, SIZE, vec>;
			//using self_t = vec<T, SIZE>;

		public:
			using common_t = typename utils::details::vec::common<T, SIZE, vec>;
			using self_t = vec<T, SIZE>;

			using common_t::derived;

			inline static constexpr const size_t static_size{SIZE};
			using value_type              = typename common_t::value_type            ;
			using size_type               = typename common_t::size_type             ;
			using difference_type         = typename common_t::difference_type       ;
			using reference               = typename common_t::reference             ;
			using const_reference         = typename common_t::const_reference       ;
			using pointer                 = typename common_t::pointer               ;
			using const_pointer           = typename common_t::const_pointer         ;
			using iterator                = typename common_t::iterator              ;
			using const_iterator          = typename common_t::const_iterator        ;
			using reverse_iterator        = typename common_t::reverse_iterator      ;
			using const_reverse_iterator  = typename common_t::const_reverse_iterator;
			using nonref_derived_t        = typename common_t::nonref_derived_t      ;
			using nonref_value_type       = typename common_t::nonref_value_type     ;

#pragma region constructors
			using details::vec_sized_specialization<T, SIZE, self_t>::vec_sized_specialization;
			
			utils_gpu_available constexpr vec() noexcept : vec{value_type{0}} {}

			//special case for vec of references
			template <std::same_as<utils::remove_cvref_t<value_type>>... Args>
				requires(utils::concepts::reference<value_type> && sizeof...(Args) == SIZE)
			utils_gpu_available constexpr vec(Args&... args) noexcept : details::vec_named<T, SIZE>{.array{args...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) >= SIZE)
			utils_gpu_available constexpr vec(const Args&... args) noexcept : details::vec_named<T, SIZE>{.array{static_cast<value_type>(args)...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) < SIZE)
			utils_gpu_available constexpr vec(const Args&... args) noexcept : details::vec_named<T, SIZE>{.array{static_cast<value_type>(args)...}}
				{
				for (size_t i = sizeof...(Args); i < SIZE; i++)
					{
					if constexpr (sizeof...(Args)) { this->array[i] = this->array[sizeof...(Args) - 1]; }
					else { this->array[i] = T{0}; }
					}
				}
			
			//special case for vec of references
			template <concepts::vec other_t>
				requires
					(
					utils::concepts::reference<value_type> && other_t::static_size == SIZE &&
						(
						((!utils::concepts::reference<typename other_t::value_type>) && std::same_as<typename other_t::value_type, utils::remove_cvref_t<value_type>>)
						||
						(( utils::concepts::reference<typename other_t::value_type>) && std::same_as<typename other_t::value_type, value_type>)
						)
					)
			utils_gpu_available constexpr vec(      other_t& other) noexcept : details::vec_named<T, SIZE>{.array{std::apply([](      auto&... values) -> std::array<value_type, SIZE> { return {                        values ...}; }, other.array)}} {}

			template <concepts::vec other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size == SIZE)
			utils_gpu_available constexpr vec(const other_t& other) noexcept : details::vec_named<T, SIZE>{.array{std::apply([](const auto&... values) -> std::array<value_type, SIZE> { return {static_cast<value_type>(values)...}; }, other.array)}} {}
			
			template <concepts::vec other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size != SIZE && utils::concepts::default_constructible<value_type>)
			utils_gpu_available constexpr vec(const other_t& other, value_type default_value = value_type{0}) noexcept
				{
				size_t i{0};
				for (; i < utils::math::min(SIZE, other_t::static_size); i++)
					{
					this->array[i] = static_cast<value_type>(other[i]);
					}
				for (size_t i = other.size(); i < SIZE; i++)
					{
					if constexpr (other_t::static_size) { this->array[i] = default_value; }
					else { this->array[i] = default_value; }
					}
				}
#pragma endregion constructors

			utils_gpu_available constexpr value_type get_length2() const noexcept { value_type ret{0}; for (const auto& element : this->array) { ret += element * element; } return ret; }
			utils_gpu_available constexpr value_type get_length () const noexcept { return std::sqrt(get_length2()); }

			utils_gpu_available constexpr self_t& set_length    (value_type value) noexcept { *this = normalize() * value; return *this; }

			__declspec(property(get = get_length, put = set_length)) value_type length;

			utils_gpu_available constexpr self_t  normalize     () const noexcept { return get_length() ? *this / get_length() : *this; }
			utils_gpu_available constexpr self_t& normalize_self()       noexcept { return *this = normalize(); }


#pragma region distances
			/// <summary> Evaluate distance^2 in the size of this vec. Missing coordinates are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<self_t> T2>
			utils_gpu_available static constexpr value_type distance2(const self_t& a, const T2& b) noexcept
				{
				constexpr auto sizes{details::pair_sizes<self_t, T2>()};

				value_type ret{0};
				size_t i{0};
				for (; i < sizes.min; i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}
					
				     if constexpr (sizes.a > sizes.b) { for (; i < utils::math::min(SIZE, sizes.a); i++) { ret += a[i] * a[i]; } }
				else if constexpr (sizes.a < sizes.b) { for (; i < utils::math::min(SIZE, sizes.b); i++) { ret += b[i] * b[i]; } }

				return ret;
				}

			/// <summary> Evaluate distance^2 in all the axes of the smaller vec. </summary>
			template <utils::details::vec::concepts::compatible_array<self_t> T2>
			utils_gpu_available static constexpr value_type distance2_shared(const self_t& a, const T2& b) noexcept
				{
				constexpr auto sizes{details::pair_sizes<self_t, T2>()};

				value_type ret{0};
				size_t i{0};

				for (; i < sizes.min; i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}

				return ret;
				}

			/// <summary> Evaluate distance^2 in all the axes of the larger vec. Missing coordinates for the smaller one are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<self_t> T2>
			utils_gpu_available static constexpr value_type distance2_complete(const self_t& a, const T2& b) noexcept
				{
				constexpr auto sizes{details::pair_sizes<self_t, T2>()};

				value_type ret{0};
				size_t i{0};

				for (; i < sizes.min; i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}

				     if constexpr (sizes.a > sizes.b) { for (; i < sizes.a; i++) { ret += a[i] * a[i]; } }
				else if constexpr (sizes.a < sizes.b) { for (; i < sizes.b; i++) { ret += b[i] * b[i]; } }

				return ret;
				}
			/// <summary> Evaluate distance in the size of this vec. Missing coordinates are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<self_t> T2>
			utils_gpu_available static constexpr value_type distance(const self_t& a, const T2& b) noexcept { return std::sqrt(distance2(a, b)); }

			/// <summary> Evaluate distance in all the axes of the smaller vec. </summary>
			template <utils::details::vec::concepts::compatible_array<self_t> T2>
			utils_gpu_available static constexpr value_type distance_shared(const self_t& a, const T2& b) noexcept { return std::sqrt(distance_shared2(a, b)); }

			/// <summary> Evaluate distance in all the axes of the larger vec. Missing coordinates for the smaller one are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<self_t> T2>
			utils_gpu_available static constexpr value_type distance_complete(const self_t& a, const T2& b) noexcept { return std::sqrt(distance_complete2(a, b)); }

#pragma endregion distances
#pragma region interpolation

			utils_gpu_available static constexpr self_t slerp_fast(const self_t& a, const self_t& b, value_type t) noexcept
				{
				return utils::math::lerp(a, b, t).normalize() * (utils::math::lerp(a.get_length(), b.get_length(), t));
				}
			utils_gpu_available static constexpr self_t tlerp_fast(const self_t& a, const self_t& b, value_type t) noexcept
				{
				return utils::math::lerp(a, b, t).normalize() * std::sqrt(utils::math::lerp(a.get_length2(), b.get_length2(), t));
				}
			utils_gpu_available static constexpr self_t slerp(const self_t& a, const self_t& b, value_type t) noexcept //TODO test
				{
				value_type dot = utils::math::clamp(vec::dot(a, b), -1.0f, 1.0f);
				value_type theta = std::acos(dot) * t;
				self_t relative_vec = (b - a * dot).normalize();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

#pragma endregion interpolation
#pragma region factories
			utils_gpu_available static constexpr self_t zero    () noexcept requires(static_size >= 1) { return {value_type{ 0}}; }
			
			utils_gpu_available static constexpr self_t rr      () noexcept requires(static_size == 1) { return {value_type{ 1}}; }
			utils_gpu_available static constexpr self_t ll      () noexcept requires(static_size == 1) { return {value_type{-1}}; }
			utils_gpu_available static constexpr self_t rr      () noexcept requires(static_size >  1) { return {value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t ll      () noexcept requires(static_size >  1) { return {value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t right   () noexcept requires(static_size >= 1) { return rr(); }
			utils_gpu_available static constexpr self_t left    () noexcept requires(static_size >= 1) { return ll(); }
			
			utils_gpu_available static constexpr self_t up      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{-1}}; }
			utils_gpu_available static constexpr self_t dw      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{ 1}}; }
			utils_gpu_available static constexpr self_t up      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t dw      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t down    () noexcept requires(static_size >= 2) { return dw(); }
			
			utils_gpu_available static constexpr self_t fw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}}; }
			utils_gpu_available static constexpr self_t bw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}}; }
			utils_gpu_available static constexpr self_t fw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t bw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr self_t forward () noexcept requires(static_size >= 3) { return fw(); }
			utils_gpu_available static constexpr self_t backward() noexcept requires(static_size >= 3) { return bw(); }

#pragma region factories
				
			template <concepts::vec b_t>
			utils_gpu_available static constexpr value_type dot(const self_t& a, const b_t& b) noexcept
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
			utils_gpu_available inline friend _inner<a_t> operator<(const a_t& lhs, _dot proxy) noexcept { return {lhs}; }

			template <concepts::vec a_t>
			class _inner
				{
				public:
					template <concepts::vec b_t>
					typename a_t::value_type operator>(const b_t& rhs) const noexcept  { return a_t::dot(lhs, rhs); }
					utils_gpu_available _inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <concepts::vec a_t>
			class _inner;

			template <concepts::vec a_t>
			utils_gpu_available inline friend _inner<a_t> operator<(const a_t& lhs, _cross proxy) noexcept { return {lhs}; }

			template <concepts::vec a_t>
			class _inner
				{
				public:
					template <concepts::vec b_t>
					a_t operator>(const b_t& rhs) const noexcept { return lhs * rhs; }
					utils_gpu_available _inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} cross;
		}
	}