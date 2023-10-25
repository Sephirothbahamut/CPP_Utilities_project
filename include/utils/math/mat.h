#pragma once

#include "math.h"
#include "../memory.h"
#include "../details/vec/common.h"
#include "../details/vec/output.h"
#include "../details/vec/memberwise_operators.h"
#include "../matrix_interface.h"

// TODO cross and dot shouldn't use vec's common cross and dot

namespace utils::math
	{
	template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
	class mat;
	
	template <size_t width, size_t height> using mat_i      = mat<int          , width, height>;
	template <size_t width, size_t height> using mat_i8     = mat<int8_t       , width, height>;
	template <size_t width, size_t height> using mat_i16    = mat<int16_t      , width, height>;
	template <size_t width, size_t height> using mat_i32    = mat<int32_t      , width, height>;
	template <size_t width, size_t height> using mat_i64    = mat<int64_t      , width, height>;
	template <size_t width, size_t height> using mat_u      = mat<unsigned     , width, height>;
	template <size_t width, size_t height> using mat_u8     = mat<uint8_t      , width, height>;
	template <size_t width, size_t height> using mat_u16    = mat<uint16_t     , width, height>;
	template <size_t width, size_t height> using mat_u32    = mat<uint32_t     , width, height>;
	template <size_t width, size_t height> using mat_u64    = mat<uint64_t     , width, height>;
	template <size_t width, size_t height> using mat_s      = mat<size_t       , width, height>;
	template <size_t width, size_t height> using mat_f      = mat<float        , width, height>;
	template <size_t width, size_t height> using mat_d      = mat<double       , width, height>;
	template <size_t width, size_t height> using mat_l      = mat<long         , width, height>;
	template <size_t width, size_t height> using mat_ul     = mat<unsigned long, width, height>;
	
	template <typename T>
	using mat2x2 = mat<T, 2, 2>;

	using mat2x2i   = mat2x2<int          >;
	using mat2x2i8  = mat2x2<int8_t       >;
	using mat2x2i16 = mat2x2<int16_t      >;
	using mat2x2i32 = mat2x2<int32_t      >;
	using mat2x2i64 = mat2x2<int64_t      >;
	using mat2x2u   = mat2x2<unsigned     >;
	using mat2x2u8  = mat2x2<uint8_t      >;
	using mat2x2u16 = mat2x2<uint16_t     >;
	using mat2x2u32 = mat2x2<uint32_t     >;
	using mat2x2u64 = mat2x2<uint64_t     >;
	using mat2x2s   = mat2x2<size_t       >;
	using mat2x2f   = mat2x2<float        >;
	using mat2x2d   = mat2x2<double       >;
	using mat2x2l   = mat2x2<long         >;
	using mat2x2ul  = mat2x2<unsigned long>;
	
	template <typename T>
	using mat3x3 = mat<T, 3, 3>;

	using mat3x3i   = mat3x3<int          >;
	using mat3x3i8  = mat3x3<int8_t       >;
	using mat3x3i16 = mat3x3<int16_t      >;
	using mat3x3i32 = mat3x3<int32_t      >;
	using mat3x3i64 = mat3x3<int64_t      >;
	using mat3x3u   = mat3x3<unsigned     >;
	using mat3x3u8  = mat3x3<uint8_t      >;
	using mat3x3u16 = mat3x3<uint16_t     >;
	using mat3x3u32 = mat3x3<uint32_t     >;
	using mat3x3u64 = mat3x3<uint64_t     >;
	using mat3x3s   = mat3x3<size_t       >;
	using mat3x3f   = mat3x3<float        >;
	using mat3x3d   = mat3x3<double       >;
	using mat3x3l   = mat3x3<long         >;
	using mat3x3ul  = mat3x3<unsigned long>;
	
	template <typename T>
	using mat4x4 = mat<T, 4, 4>;

	using mat4x4i   = mat4x4<int          >;
	using mat4x4i8  = mat4x4<int8_t       >;
	using mat4x4i16 = mat4x4<int16_t      >;
	using mat4x4i32 = mat4x4<int32_t      >;
	using mat4x4i64 = mat4x4<int64_t      >;
	using mat4x4u   = mat4x4<unsigned     >;
	using mat4x4u8  = mat4x4<uint8_t      >;
	using mat4x4u16 = mat4x4<uint16_t     >;
	using mat4x4u32 = mat4x4<uint32_t     >;
	using mat4x4u64 = mat4x4<uint64_t     >;
	using mat4x4s   = mat4x4<size_t       >;
	using mat4x4f   = mat4x4<float        >;
	using mat4x4d   = mat4x4<double       >;
	using mat4x4l   = mat4x4<long         >;
	using mat4x4ul  = mat4x4<unsigned long>;

	
	namespace concepts
		{
		template <typename T>
		concept mat = std::derived_from<T, utils::math::mat<typename T::value_type, T::static_sizes.x, T::static_sizes.y, T::memory_layout>>;
		}
	namespace details
		{
		inline extern constexpr const char mat_name[]{"mat"};

		template <typename T, size_t size>
		struct mat_named { std::array<T, size> array; };
		}

	template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
	class mat :
		public matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>,
		public details::mat_named<T, size>,
		public utils::details::vec::common<T, size, mat<T, size>>,
		public utils::details::vec::memberwise_operators<T, size, mat, mat<T, size>>,
		public utils::details::vec::output<details::mat_name, mat<T, size>>
		{
		public:
			using derived_t = mat<T, size>;

		private:
			using arr_t = std::array<T, size>;

		public:
			inline static constexpr const size_t static_size{size};
			using value_type = typename arr_t::value_type;

#pragma region constructors
			using details::vec_sized_specialization<T, size, derived_t>::vec_sized_specialization;
			
			utils_gpu_available constexpr mat() noexcept : mat{value_type{0}} {}

			//special case for mat of references
			template <std::same_as<typename value_type::value_type>... Args>
				requires(utils::concepts::reference<value_type> && sizeof...(Args) == static_size)
			utils_gpu_available constexpr mat(Args&... args) noexcept : details::vec_named<T, size>{.array{args...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) >= static_size)
			utils_gpu_available constexpr mat(const Args&... args) noexcept : details::vec_named<T, size>{.array{static_cast<value_type>(args)...}} {}

			template <std::convertible_to<value_type>... Args>
				requires(sizeof...(Args) < static_size)
			utils_gpu_available constexpr mat(const Args&... args) noexcept : details::vec_named<T, size>{.array{static_cast<value_type>(args)...}}
				{
				for (size_t i = sizeof...(Args); i < static_size; i++)
					{
					if constexpr (sizeof...(Args)) { this->array[i] = this->array[sizeof...(Args) - 1]; }
					else { this->array[i] = T{0}; }
					}
				}
			
			//special case for mat of references
			template <concepts::mat other_t>
				requires(utils::concepts::reference<value_type> && std::same_as<typename other_t::value_type, typename value_type::value_type> && other_t::static_size == static_size)
			utils_gpu_available constexpr mat(      other_t& other) noexcept : details::vec_named<T, size>{.array{std::apply([](      auto&... values) -> std::array<value_type, size> { return {                        values ...}; }, other.array)}} {}

			template <concepts::mat other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size == static_size)
			utils_gpu_available constexpr mat(const other_t& other) noexcept : details::vec_named<T, size>{.array{std::apply([](const auto&... values) -> std::array<value_type, size> { return {static_cast<value_type>(values)...}; }, other.array)}} {}
			
			template <concepts::mat other_t>
				requires(std::convertible_to<typename other_t::value_type, value_type> && other_t::static_size != static_size && utils::concepts::default_constructible<value_type>)
			utils_gpu_available constexpr mat(const other_t& other, value_type default_value = value_type{0}) noexcept
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

			utils_gpu_available constexpr value_type get_length2() const noexcept { value_type ret{0}; for (const auto& element : this->array) { ret += element * element; } return ret; }
			utils_gpu_available constexpr value_type get_length () const noexcept { return std::sqrt(get_length2()); }

			utils_gpu_available constexpr derived_t& set_length    (value_type value) noexcept { *this = normalize() * value; return *this; }

			__declspec(property(get = get_length, put = set_length)) value_type length;

			utils_gpu_available constexpr derived_t  normalize     () const noexcept { return get_length() ? *this / get_length() : *this; }
			utils_gpu_available constexpr derived_t& normalize_self()       noexcept { return *this = normalize(); }


#pragma region distances
			/// <summary> Evaluate distance^2 in the size of this mat. Missing coordinates are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			utils_gpu_available static constexpr value_type distance2(const derived_t& a, const T2& b) noexcept
				{
				value_type ret{0};
				size_t i{0};
				for (; i < std::min({static_size, a.size(), b.size()}); i++)
					{
					value_type tmp{a[i] - b[i]};
					ret += tmp * tmp;
					}
					
				     if /*constepxr*/ (a.size() > b.size()) { for (; i < std::min(static_size, a.size()); i++) { ret += a[i] * a[i]; } } //TODO check why no conxtexpr
				else if /*constepxr*/ (a.size() < b.size()) { for (; i < std::min(static_size, b.size()); i++) { ret += b[i] * b[i]; } } //TODO check why no conxtexpr

				return ret;
				}

			/// <summary> Evaluate distance^2 in all the axes of the smaller mat. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			utils_gpu_available static constexpr value_type distance2_shared(const derived_t& a, const T2& b) noexcept
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

			/// <summary> Evaluate distance^2 in all the axes of the larger mat. Missing coordinates for the smaller one are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			utils_gpu_available static constexpr value_type distance2_complete(const derived_t& a, const T2& b) noexcept
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
			/// <summary> Evaluate distance in the size of this mat. Missing coordinates are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			utils_gpu_available static constexpr value_type distance(const derived_t& a, const T2& b) noexcept { return std::sqrt(distance2(a, b)); }

			/// <summary> Evaluate distance in all the axes of the smaller mat. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			utils_gpu_available static constexpr value_type distance_shared(const derived_t& a, const T2& b) noexcept { return std::sqrt(distance_shared2(a, b)); }

			/// <summary> Evaluate distance in all the axes of the larger mat. Missing coordinates for the smaller one are considered 0. </summary>
			template <utils::details::vec::concepts::compatible_array<derived_t> T2>
			utils_gpu_available static constexpr value_type distance_complete(const derived_t& a, const T2& b) noexcept { return std::sqrt(distance_complete2(a, b)); }

#pragma endregion distances
#pragma region interpolation

			utils_gpu_available static constexpr derived_t slerp_fast(const derived_t& a, const derived_t& b, value_type t) noexcept
				{
				return utils::math::lerp(a, b, t).normalize() * (utils::math::lerp(a.get_length(), b.get_length(), t));
				}
			utils_gpu_available static constexpr derived_t tlerp_fast(const derived_t& a, const derived_t& b, value_type t) noexcept
				{
				return utils::math::lerp(a, b, t).normalize() * std::sqrt(utils::math::lerp(a.get_length2(), b.get_length2(), t));
				}
			utils_gpu_available static constexpr derived_t slerp(const derived_t& a, const derived_t& b, value_type t) noexcept //TODO test
				{
				value_type dot = utils::math::clamp(mat::dot(a, b), -1.0f, 1.0f);
				value_type theta = std::acos(dot) * t;
				derived_t relative_vec = (b - a * dot).normalize();
				return ((a * std::cos(theta)) + (relative_vec * std::sin(theta)));
				}

#pragma endregion interpolation
#pragma region factories
			utils_gpu_available static constexpr derived_t zero    () noexcept requires(static_size >= 1) { return {value_type{ 0}}; }
			
			utils_gpu_available static constexpr derived_t rr      () noexcept requires(static_size == 1) { return {value_type{ 1}}; }
			utils_gpu_available static constexpr derived_t ll      () noexcept requires(static_size == 1) { return {value_type{-1}}; }
			utils_gpu_available static constexpr derived_t rr      () noexcept requires(static_size >  1) { return {value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr derived_t ll      () noexcept requires(static_size >  1) { return {value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr derived_t right   () noexcept requires(static_size >= 1) { return rr(); }
			utils_gpu_available static constexpr derived_t left    () noexcept requires(static_size >= 1) { return ll(); }
			
			utils_gpu_available static constexpr derived_t up      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{-1}}; }
			utils_gpu_available static constexpr derived_t dw      () noexcept requires(static_size == 2) { return {value_type{ 0}, value_type{ 1}}; }
			utils_gpu_available static constexpr derived_t up      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr derived_t dw      () noexcept requires(static_size >  2) { return {value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr derived_t down    () noexcept requires(static_size >= 2) { return dw(); }
			
			utils_gpu_available static constexpr derived_t fw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}}; }
			utils_gpu_available static constexpr derived_t bw      () noexcept requires(static_size == 3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}}; }
			utils_gpu_available static constexpr derived_t fw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{ 1}, value_type{ 0}}; }
			utils_gpu_available static constexpr derived_t bw      () noexcept requires(static_size >  3) { return {value_type{ 0}, value_type{ 0}, value_type{-1}, value_type{ 0}}; }
			utils_gpu_available static constexpr derived_t forward () noexcept requires(static_size >= 3) { return fw(); }
			utils_gpu_available static constexpr derived_t backward() noexcept requires(static_size >= 3) { return bw(); }

#pragma region factories
				
			template <concepts::mat b_t>
			utils_gpu_available static constexpr value_type dot(const derived_t& a, const b_t& b) noexcept
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
			template <concepts::mat a_t>
			class _inner;

			template <concepts::mat a_t>
			utils_gpu_available inline friend _inner<a_t> operator<(const a_t& lhs, _dot proxy) noexcept { return {lhs}; }

			template <concepts::mat a_t>
			class _inner
				{
				public:
					template <concepts::mat b_t>
					typename a_t::value_type operator>(const b_t& rhs) const noexcept  { return a_t::dot(lhs, rhs); }
					utils_gpu_available _inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} dot;

		inline constexpr struct _cross
			{
			template <concepts::mat a_t>
			class _inner;

			template <concepts::mat a_t>
			utils_gpu_available inline friend _inner<a_t> operator<(const a_t& lhs, _cross proxy) noexcept { return {lhs}; }

			template <concepts::mat a_t>
			class _inner
				{
				public:
					template <concepts::mat b_t>
					a_t operator>(const b_t& rhs) const noexcept { return lhs * rhs; }
					utils_gpu_available _inner(const a_t& lhs) noexcept : lhs{lhs} {}
				private:
					const a_t& lhs;
				};

			} cross;
		}
	}