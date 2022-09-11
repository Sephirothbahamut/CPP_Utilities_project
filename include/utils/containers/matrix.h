#pragma once

#include <vector>
#include <stdexcept>
#include <ranges>
#include <initializer_list>

#include "../math/vec2.h"

//TODO write test cases

namespace utils::containers
	{
	enum matrix_memory { width_first, height_first };

	template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
	class matrix;
	
	template <size_t width, size_t height> using mati   = matrix<int     , width, height>;
	template <size_t width, size_t height> using mati8  = matrix<int8_t  , width, height>;
	template <size_t width, size_t height> using mati16 = matrix<int16_t , width, height>;
	template <size_t width, size_t height> using mati32 = matrix<int32_t , width, height>;
	template <size_t width, size_t height> using mati64 = matrix<int64_t , width, height>;
	template <size_t width, size_t height> using matu   = matrix<unsigned, width, height>;
	template <size_t width, size_t height> using matu8  = matrix<uint8_t , width, height>;
	template <size_t width, size_t height> using matu16 = matrix<uint16_t, width, height>;
	template <size_t width, size_t height> using matu32 = matrix<uint32_t, width, height>;
	template <size_t width, size_t height> using matu64 = matrix<uint64_t, width, height>;
	template <size_t width, size_t height> using mats   = matrix<size_t  , width, height>;
	template <size_t width, size_t height> using matf   = matrix<float   , width, height>;
	template <size_t width, size_t height> using matd   = matrix<double  , width, height>;
	
	template <typename T> using mat2x2 = matrix<T, 2, 2>;
	using mat2x2i   = mat2x2<int     >;
	using mat2x2i8  = mat2x2<int8_t  >;
	using mat2x2i16 = mat2x2<int16_t >;
	using mat2x2i32 = mat2x2<int32_t >;
	using mat2x2i64 = mat2x2<int64_t >;
	using mat2x2u   = mat2x2<unsigned>;
	using mat2x2u8  = mat2x2<uint8_t >;
	using mat2x2u16 = mat2x2<uint16_t>;
	using mat2x2u32 = mat2x2<uint32_t>;
	using mat2x2u64 = mat2x2<uint64_t>;
	using mat2x2s   = mat2x2<size_t  >;
	using mat2x2f   = mat2x2<float   >;
	using mat2x2d   = mat2x2<double  >;
	
	template <typename T> using mat3x3 = matrix<T, 3, 3>;
	using mat3x3i   = mat3x3<int     >;
	using mat3x3i8  = mat3x3<int8_t  >;
	using mat3x3i16 = mat3x3<int16_t >;
	using mat3x3i32 = mat3x3<int32_t >;
	using mat3x3i64 = mat3x3<int64_t >;
	using mat3x3u   = mat3x3<unsigned>;
	using mat3x3u8  = mat3x3<uint8_t >;
	using mat3x3u16 = mat3x3<uint16_t>;
	using mat3x3u32 = mat3x3<uint32_t>;
	using mat3x3u64 = mat3x3<uint64_t>;
	using mat3x3s   = mat3x3<size_t  >;
	using mat3x3f   = mat3x3<float   >;
	using mat3x3d   = mat3x3<double  >;
	
	template <typename T> using mat4x4 = matrix<T, 4, 4>;
	using mat4x4i   = mat4x4<int     >;
	using mat4x4i8  = mat4x4<int8_t  >;
	using mat4x4i16 = mat4x4<int16_t >;
	using mat4x4i32 = mat4x4<int32_t >;
	using mat4x4i64 = mat4x4<int64_t >;
	using mat4x4u   = mat4x4<unsigned>;
	using mat4x4u8  = mat4x4<uint8_t >;
	using mat4x4u16 = mat4x4<uint16_t>;
	using mat4x4u32 = mat4x4<uint32_t>;
	using mat4x4u64 = mat4x4<uint64_t>;
	using mat4x4s   = mat4x4<size_t  >;
	using mat4x4f   = mat4x4<float   >;
	using mat4x4d   = mat4x4<double  >;

	namespace concepts
		{
		template <typename T>
		concept matrix = std::derived_from<std::remove_cvref_t<T>, utils::containers::matrix<typename T::value_type, T::static_width, T::static_height, T::memory_layout>>;;
		}

	template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
	class matrix
		{
		using inner_container_t = std::array<T, WIDTH * HEIGHT>;

		public:
			using value_type             = inner_container_t::value_type;
			using size_type              = inner_container_t::size_type;
			using reference              = inner_container_t::reference;
			using const_reference        = inner_container_t::const_reference;
			using pointer                = inner_container_t::pointer;
			using const_pointer          = inner_container_t::const_pointer;
			using iterator               = inner_container_t::iterator;
			using const_iterator         = inner_container_t::const_iterator;
			using reverse_iterator       = inner_container_t::reverse_iterator;
			using const_reverse_iterator = inner_container_t::const_reverse_iterator;

			using coords_type = utils::math::vec<size_type, 2>;
			inline static constexpr matrix_memory memory_layout = MEMORY_LAYOUT;
			inline static constexpr size_t static_width {WIDTH };
			inline static constexpr size_t static_height{HEIGHT};
			inline static constexpr size_t static_size  {WIDTH * HEIGHT};

			matrix() { std::fill(begin(), end(), value_type{}); };
			matrix(const value_type& default_value) { std::fill(begin(), end(), default_value); }

			const size_type width () const noexcept { return static_width ; }
			const size_type height() const noexcept { return static_height; }
			const size_type size  () const noexcept { return static_size  ; }
			const bool      empty () const noexcept { return !size()      ; }

			const_pointer   data  () const noexcept { return _arr.data(); }
			      pointer   data  ()       noexcept { return _arr.data(); }

			const inner_container_t& get_array() const noexcept { return _arr; }
			      inner_container_t& get_array()       noexcept { return _arr; }

			size_type get_index(size_type x, size_type y) const noexcept
				{
				if constexpr (memory_layout == matrix_memory::width_first) { return x + (y * static_width); }
				else { return y + (x * static_height); }
				}
			size_type   get_x     (size_type index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index % width(); } else { return index / height(); } } //TODO test
			size_type   get_y     (size_type index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index / width(); } else { return index % height(); } } //TODO test
			coords_type get_coords(size_type index) const noexcept { return {get_x(index), get_y(index)}; }
			
			const_reference operator[](size_type i)              const noexcept { return _arr[i                            ]; }
			      reference operator[](size_type i)                    noexcept { return _arr[i                            ]; }
			const_reference operator[](coords_type coords)       const noexcept { return _arr[get_index(coords.x, coords.y)]; }
			      reference operator[](coords_type coords)             noexcept { return _arr[get_index(coords.x, coords.y)]; }
			const_reference at        (size_type i)              const          { if (i >= size()                            ) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[](i     ); }
			      reference at        (size_type i)                             { if (i >= size()                            ) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[](i     ); }
			const_reference at        (size_type x, size_type y) const          { if (x >= static_width || y >= static_height) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			      reference at        (size_type x, size_type y)                { if (x >= static_width || y >= static_height) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			const_reference at        (coords_type coords)       const          { return at(coords.x, coords.y); }
			      reference at        (coords_type coords)                      { return at(coords.x, coords.y); }

			// Arithmetic

			/*static T cross(const matrix_dyn& a, const matrix_dyn& b) noexcept { }//TODO
			static T dot(const matrix_dyn& a, const matrix_dyn& b) noexcept {}//TODO
			static T dot(const matrix_dyn& a, const matrix_dyn& b) noexcept {}//TODO*/
				  
			const auto begin  () const noexcept { return _arr.begin  (); }
			      auto begin  ()       noexcept { return _arr.begin  (); }
			const auto end    () const noexcept { return _arr.end    (); }
			      auto end    ()       noexcept { return _arr.end    (); }
			const auto cbegin () const noexcept { return _arr.cbegin (); }
			      auto cbegin ()       noexcept { return _arr.cbegin (); }
			const auto cend   () const noexcept { return _arr.cend   (); }
			      auto cend   ()       noexcept { return _arr.cend   (); }
			const auto rbegin () const noexcept { return _arr.rbegin (); }
			      auto rbegin ()       noexcept { return _arr.rbegin (); }
			const auto rend   () const noexcept { return _arr.rend   (); }
			      auto rend   ()       noexcept { return _arr.rend   (); }
			const auto crbegin() const noexcept { return _arr.crbegin(); }
			      auto crbegin()       noexcept { return _arr.crbegin(); }
			const auto crend  () const noexcept { return _arr.crend  (); }
			      auto crend  ()       noexcept { return _arr.crend  (); }
				  
			//auto rows(                                      ) const { return std::ranges::view::ints(0    , static_height) | std::ranges::view::transform(to_row()); }
			//auto rows(                                      )       { return std::ranges::view::ints(0    , static_height) | std::ranges::view::transform(to_row()); }
			//auto cols(                                      ) const { return std::ranges::view::ints(0    , static_width ) | std::ranges::view::transform(to_col()); }
			//auto cols(                                      )       { return std::ranges::view::ints(0    , static_width ) | std::ranges::view::transform(to_col()); }
			//auto rows(size_t start, size_t end              ) const { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_row()); }
			//auto rows(size_t start, size_t end              )       { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_row()); }
			//auto cols(size_t start, size_t end              ) const { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_col()); }
			//auto cols(size_t start, size_t end              )       { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_col()); }
			//auto rows(std::initializer_list<size_t> indices ) const { return indices | std::ranges::view::transform(to_row()); }
			//auto rows(std::initializer_list<size_t> indices )       { return indices | std::ranges::view::transform(to_row()); }
			//auto cols(std::initializer_list<size_t> indices ) const { return indices | std::ranges::view::transform(to_col()); }
			//auto cols(std::initializer_list<size_t> indices )       { return indices | std::ranges::view::transform(to_col()); }

		private:
			std::array<T, static_width * static_height> _arr;

			//auto to_row() { return [this](size_t i) { return data | std::ranges::view::drop(i * col_count) | std::ranges::view::take(col_count); }; };
			//auto to_col() { return [this](size_t i) { return data | std::ranges::view::drop(i) | std::ranges::view::stride(col_count); }; };
		};

		

#pragma region operators
	template <concepts::matrix matrix_t> bool operator==(const matrix_t& a, const matrix_t& b) noexcept { return a._arr == b._arr; }
	template <concepts::matrix matrix_t> bool operator!=(const matrix_t& a, const matrix_t& b) noexcept { return !(a == b); }

	
	// OPERATORS
	template <concepts::matrix matrix_t> matrix_t  operator- (const matrix_t& m) noexcept { matrix_t ret; for (size_t i{ 0 }; i < m.size(); i++) { ret[i] = -m[i]; }; return ret; }
// 
	// VEC & SCALAR OPERATORS

	template <concepts::matrix matrix_t> matrix_t  operator++(const matrix_t& m) noexcept { return (*this) += typename matrix_t::value_type{1}; }
	template <concepts::matrix matrix_t> matrix_t  operator--(const matrix_t& m) noexcept { return (*this) -= typename matrix_t::value_type{1}; }
	
	template <concepts::matrix matrix_t> matrix_t  operator+ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] + b; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator- (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] - b; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator* (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] * b; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator/ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] / b; }; return ret; }
	
	template <concepts::matrix matrix_t> matrix_t& operator+=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a + b; }
	template <concepts::matrix matrix_t> matrix_t& operator-=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a - b; }
	template <concepts::matrix matrix_t> matrix_t& operator*=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a * b; }
	template <concepts::matrix matrix_t> matrix_t& operator/=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a / b; }

	// SCALAR & VEC OPERATORS
	template <concepts::matrix matrix_t> matrix_t  operator- (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { matrix_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a - b[i]; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator/ (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { matrix_t ret; for (size_t i{0}; i < a.size(); i++) { ret[i] = a / b[i]; }; return ret; }

	// VEC & VEC OPERATORS
	//TODO differently sized b operand
	template <concepts::matrix matrix_t> matrix_t  operator+ (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] + b[i]; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator- (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] - b[i]; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator* (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] * b[i]; }; return ret; }
	template <concepts::matrix matrix_t> matrix_t  operator/ (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] / b[i]; }; return ret; }
	
	template <concepts::matrix matrix_t> matrix_t& operator+=(      matrix_t& a, const matrix_t& b) noexcept { return a = a + b; }
	template <concepts::matrix matrix_t> matrix_t& operator-=(      matrix_t& a, const matrix_t& b) noexcept { return a = a - b; }
	template <concepts::matrix matrix_t> matrix_t& operator*=(      matrix_t& a, const matrix_t& b) noexcept { return a = a * b; }
	template <concepts::matrix matrix_t> matrix_t& operator/=(      matrix_t& a, const matrix_t& b) noexcept { return a = a / b; }
#pragma endregion operators
	}

namespace utils::output
	{
	namespace typeless
		{
		template <typename T, size_t WIDTH, size_t HEIGHT, utils::containers::matrix_memory MEMORY_LAYOUT>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::containers::matrix<T, WIDTH, HEIGHT, MEMORY_LAYOUT>& container) noexcept
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";

			if (!container.empty())
				{
				for (size_t y = 0; y < container.height() - 1; y++)
					{
					for (size_t x = 0; x < container.width() - 1; x++)
						{
						os << ucc::value << container[{x, y}] << ucc::separ << ", ";
						}
					os << ucc::value << container[{container.width() - 1, y}] << " | ";
					}
				
				for (size_t x = 0; x < container.width() - 1; x++)
					{
					os << ucc::value << container[{x, container.height() - 1}] << ucc::separ << ", ";
					}
				os << ucc::value << container[{container.width() - 1, container.height() - 1}];
				}
			return os << ucc::brace << "]";
			}
		}

	template <typename T, size_t WIDTH, size_t HEIGHT, utils::containers::matrix_memory MEMORY_LAYOUT>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::containers::matrix<T, WIDTH, HEIGHT, MEMORY_LAYOUT>& container) noexcept
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "mat" << container.width() << "x" << container.height() << typeid(T).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}