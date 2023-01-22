#pragma once

#include <vector>
#include <ranges>
#include <stdexcept>
#include <initializer_list>

#include "memberwise_operators/par.h"
#include "memberwise_operators/seq.h"
#include "matrix_memory_layout.h"
#include "../math/vec2.h"
#include "../console/colour.h"

//TODO write test cases

namespace utils::containers
	{
	template <typename T, matrix_memory MEMORY_LAYOUT, class ALLOCATOR_TYPE>
	class matrix_dyn;

	namespace concepts
		{
		template <typename T, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first, class ALLOCATOR_TYPE = std::allocator<T>>
		concept matrix_dyn = std::derived_from<std::remove_cvref_t<T>, utils::containers::matrix_dyn<typename T::value_type, T::memory_layout, typename T::allocator_type>>;
		}

	template <typename T, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first, class ALLOCATOR_TYPE = std::allocator<T>>
	class matrix_dyn
		{
		using inner_container_t = std::vector<T, ALLOCATOR_TYPE>;

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

			using allocator_type = ALLOCATOR_TYPE;
			using coords_type = utils::math::vec<size_type, 2>;
			inline static constexpr matrix_memory memory_layout = MEMORY_LAYOUT;

			matrix_dyn(size_type width, size_type height) : _sizes{ width , height }, _arr(_sizes.x* _sizes.y) {} //TODO remove
			matrix_dyn(size_type width, size_type height, const value_type& default_value) : _sizes{ width , height }, _arr(_sizes.x* _sizes.y, default_value) {} //TODO remove
			matrix_dyn(coords_type size) : _sizes{ size.x, size.y }, _arr(_sizes.x* _sizes.y) {}
			matrix_dyn(coords_type size, const value_type& default_value) : _sizes{ size.x, size.y }, _arr(_sizes.x* _sizes.y, default_value) {}
			matrix_dyn(coords_type size, inner_container_t&& values) : _sizes{ size.x, size.y }, _arr(std::move(values)) {}

			const size_type   width () const noexcept { return _sizes.x; }
			const size_type   height() const noexcept { return _sizes.y; }
			const size_type   size  () const noexcept { return _sizes.x * _sizes.y; }
			const coords_type sizes () const noexcept { return _sizes; }
			const bool        empty () const noexcept { return !size(); }

			const_pointer data() const noexcept { return _arr.data(); }
			      pointer data()       noexcept { return _arr.data(); }

			const inner_container_t& get_vector() const noexcept { return _arr; }
			      inner_container_t& get_vector()       noexcept { return _arr; }

			size_type get_index(coords_type coords) const noexcept { return get_index(coords.x, coords.y); }
			size_type get_index(size_type x, size_type y) const noexcept
				{
				if constexpr (memory_layout == matrix_memory::width_first) { return x + (y * _sizes.x); }
				else { return y + (x * _sizes.y); }
				}
			size_type   get_x     (size_type index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index % width(); } else { return index / height(); } } //TODO test
			size_type   get_y     (size_type index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index / width(); } else { return index % height(); } } //TODO test
			coords_type get_coords(size_type index) const noexcept { return { get_x(index), get_y(index) }; }

			const_reference operator[](size_type i)              const noexcept { return _arr[i]; }
			      reference operator[](size_type i)                    noexcept { return _arr[i]; }
			const_reference operator[](coords_type coords)       const noexcept { return _arr[get_index(coords.x, coords.y)]; }
			      reference operator[](coords_type coords)             noexcept { return _arr[get_index(coords.x, coords.y)]; }
			const_reference at(size_type i)              const { if (i >= size()) { throw std::out_of_range{ "Matrix access out of bounds." }; } return operator[](i); }
			      reference at(size_type i)                    { if (i >= size()) { throw std::out_of_range{ "Matrix access out of bounds." }; } return operator[](i); }
			const_reference at(size_type x, size_type y) const { if (x >= width() || y >= height()) { throw std::out_of_range{ "Matrix access out of bounds." }; } return operator[]({ x, y }); }
			      reference at(size_type x, size_type y)       { if (x >= width() || y >= height()) { throw std::out_of_range{ "Matrix access out of bounds." }; } return operator[]({ x, y }); }
			const_reference at(coords_type coords)       const { return at(coords.x, coords.y); }
			      reference at(coords_type coords)             { return at(coords.x, coords.y); }

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

			//TODO test rows and cols
			//auto rows(                                      ) const { return std::ranges::view::ints(0    , height()     ) | std::ranges::view::transform(to_row()); }
			//auto rows(                                      )       { return std::ranges::view::ints(0    , height()     ) | std::ranges::view::transform(to_row()); }
			//auto cols(                                      ) const { return std::ranges::view::ints(0    , width ()     ) | std::ranges::view::transform(to_col()); }
			//auto cols(                                      )       { return std::ranges::view::ints(0    , width ()     ) | std::ranges::view::transform(to_col()); }
			//auto rows(size_t start, size_t end              ) const { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_row()); }
			//auto rows(size_t start, size_t end              )       { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_row()); }
			//auto cols(size_t start, size_t end              ) const { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_col()); }
			//auto cols(size_t start, size_t end              )       { return std::ranges::view::ints(start, end          ) | std::ranges::view::transform(to_col()); }
			//auto rows(std::initializer_list<size_t> indices ) const { return indices | std::ranges::view::transform(to_row()); }
			//auto rows(std::initializer_list<size_t> indices )       { return indices | std::ranges::view::transform(to_row()); }
			//auto cols(std::initializer_list<size_t> indices ) const { return indices | std::ranges::view::transform(to_col()); }
			//auto cols(std::initializer_list<size_t> indices )       { return indices | std::ranges::view::transform(to_col()); }

		private:
			coords_type _sizes;
			std::vector<T, allocator_type> _arr;

			//auto to_row() { return [this](size_t i) { return data | std::ranges::view::drop(i * col_count) | std::ranges::view::take(col_count); }; };
			//auto to_col() { return [this](size_t i) { return data | std::ranges::view::drop(i) | std::ranges::view::stride(col_count); }; };
		};



	//#pragma region operators
	//	template <concepts::matrix_dyn matrix_t> bool operator==(const matrix_t& a, const matrix_t& b) noexcept { return a._arr == b._arr; }
	//	template <concepts::matrix_dyn matrix_t> bool operator!=(const matrix_t& a, const matrix_t& b) noexcept { return !(a == b); }
	//
	//	
	//	// OPERATORS
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& m) noexcept { matrix_t ret{m.sizes()}; for (size_t i{0}; i < m.size(); i++) { ret[i] = -m[i]; }; return ret; }
	//	// 
	//	// VEC & SCALAR OPERATORS
	//
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator++(const matrix_t& m) noexcept { return (*this) += typename matrix_t::value_type{1}; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator--(const matrix_t& m) noexcept { return (*this) -= typename matrix_t::value_type{1}; }
	//	
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator+ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] + b; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] - b; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator* (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] * b; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator/ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] / b; }; return ret; }
	//	
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator+=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a + b; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator-=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a - b; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator*=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a * b; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator/=(      matrix_t& a, const typename matrix_t::value_type& b) noexcept { return a = a / b; }
	//
	//	// SCALAR & VEC OPERATORS
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator- (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { matrix_t ret{b.sizes()}; for (size_t i{0}; i < b.size(); i++) { ret[i] = a - b[i]; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator/ (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { matrix_t ret{b.sizes()}; for (size_t i{0}; i < b.size(); i++) { ret[i] = a / b[i]; }; return ret; }
	//
	//	// VEC & VEC OPERATORS
	//	//TODO differently sized b operand
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator+ (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] + b[i]; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] - b[i]; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator* (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] * b[i]; }; return ret; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t  operator/ (const matrix_t& a, const matrix_t& b) noexcept { matrix_t ret{a.sizes()}; for (size_t i{0}; i < a.size(); i++) { ret[i] = a[i] / b[i]; }; return ret; }
	//	
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator+=(      matrix_t& a, const matrix_t& b) noexcept { return a = a + b; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator-=(      matrix_t& a, const matrix_t& b) noexcept { return a = a - b; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator*=(      matrix_t& a, const matrix_t& b) noexcept { return a = a * b; }
	//	template <concepts::matrix_dyn matrix_t> matrix_t& operator/=(      matrix_t& a, const matrix_t& b) noexcept { return a = a / b; }
	//#pragma endregion operators
	}

namespace utils::containers::memberwise_operators::seq
	{
	// OPERATORS
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& m) noexcept { return { m.sizes(), -m.get_vector() }; }
	// 
	// VEC & SCALAR OPERATORS

	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator++(const matrix_t& m) noexcept { return m + typename matrix_t::value_type{ 1 }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator--(const matrix_t& m) noexcept { return m - typename matrix_t::value_type{ 1 }; }

	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator+ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() + b }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() - b }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator* (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() * b }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator/ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() / b }; }

	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator+=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() += b; return a; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator-=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() -= b; return a; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator*=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() *= b; return a; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator/=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() /= b; return a; }

	// SCALAR & VEC OPERATORS
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator- (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { return { b.sizes(), a - b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator/ (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { return { b.sizes(), a / b.get_vector() }; }

	// VEC & VEC OPERATORS
	//TODO differently sized b operand
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator+ (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() + b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator- (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() - b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator* (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() * b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator/ (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() / b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator+=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() += b.get_vector(); return a; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator-=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() -= b.get_vector(); return a; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator*=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() *= b.get_vector(); return a; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator/=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() /= b.get_vector(); return a; }
	}

namespace utils::containers::memberwise_operators::par
	{
	// OPERATORS
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& m) noexcept { return { m.sizes(), -m.get_vector() }; }
	// 
	// VEC & SCALAR OPERATORS

	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator++(const matrix_t& m) noexcept { return m + typename matrix_t::value_type{1}; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator--(const matrix_t& m) noexcept { return m - typename matrix_t::value_type{1}; }

	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator+ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() + b }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator- (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() - b }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator* (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() * b }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator/ (const matrix_t& a, const typename matrix_t::value_type& b) noexcept { return { a.sizes(), a.get_vector() / b }; }

	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator+=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() += b; return a; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator-=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() -= b; return a; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator*=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() *= b; return a; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t& operator/=(matrix_t& a, const typename matrix_t::value_type& b) noexcept { a.get_vector() /= b; return a; }

	// SCALAR & VEC OPERATORS
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator- (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { return { b.sizes(), a - b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_t> matrix_t  operator/ (const typename matrix_t::value_type& a, const matrix_t& b) noexcept { return { b.sizes(), a / b.get_vector() }; }

	// VEC & VEC OPERATORS
	//TODO differently sized b operand
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator+ (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() + b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator- (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() - b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator* (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() * b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t  operator/ (const matrix_a_t& a, const matrix_b_t& b) noexcept { return { a.sizes(), a.get_vector() / b.get_vector() }; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator+=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() += b.get_vector(); return a; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator-=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() -= b.get_vector(); return a; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator*=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() *= b.get_vector(); return a; }
	template <utils::containers::concepts::matrix_dyn matrix_a_t, utils::containers::concepts::matrix_dyn matrix_b_t> matrix_a_t& operator/=(matrix_a_t& a, const matrix_b_t& b) noexcept { a.get_vector() /= b.get_vector(); return a; }
	}

namespace utils::math
	{
	template <utils::containers::concepts::matrix_dyn a_t>
	a_t abs(const a_t& v) noexcept { a_t ret{ v.sizes() }; for (size_t i{ 0 }; i < a_t::static_size; i++) { ret[i] = std::abs(v[i]); } return ret; }

	template <utils::containers::concepts::matrix_dyn a_t>
	inline a_t lerp(const a_t& a, const a_t& b, float t)
		{
		a_t ret{ a.sizes() };
		for (size_t i{ 0 }; i < a_t::static_size; i++) { ret[i] = utils::math::lerp(a[i], b[i], t); } //TODO decomment
		return ret;
		}

	template <utils::containers::concepts::matrix_dyn a_t>
	inline a_t clamp(const a_t& in, const a_t& min, const a_t& max)
		{
		a_t ret{ in.sizes() };
		for (size_t i = 0; i < in.size(); i++)
			{
			ret[i] = utils::math::clamp(in[i], min[i], max[i]);
			}
		return ret;
		}

	template <utils::containers::concepts::matrix_dyn a_t>
	inline a_t clamp(const a_t& in, const typename a_t::value_type& min, const typename a_t::value_type& max)
		{
		a_t ret{ in.sizes() };
		for (size_t i = 0; i < in.size(); i++)
			{
			ret[i] = utils::math::clamp(in[i], min, max);
			}
		return ret;
		}

	template <utils::containers::concepts::matrix_dyn a_t, utils::containers::concepts::matrix_dyn b_t>
	inline a_t min(const a_t& a, const b_t& b) { return utils::containers::memberwise_operators::operation(a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return utils::math::min(a, b); }); }

	template <utils::containers::concepts::matrix_dyn a_t, utils::containers::concepts::matrix_dyn b_t>
	inline a_t max(const a_t& a, const b_t& b) { return utils::containers::memberwise_operators::operation(a, b, [](const a_t::value_type& a, const b_t::value_type& b) { return utils::math::max(a, b); }); }
	}

namespace utils::output
	{
	namespace typeless
		{
		//TODO test/make work with allocator parameter
		template <typename T, utils::containers::matrix_memory MEMORY_LAYOUT>//, class ALLOCATOR_TYPE>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::containers::matrix_dyn<T, MEMORY_LAYOUT> container)//, ALLOCATOR_TYPE>& container)
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

	template <typename T, utils::containers::matrix_memory MEMORY_LAYOUT, class ALLOCATOR_TYPE>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::containers::matrix_dyn<T, MEMORY_LAYOUT, ALLOCATOR_TYPE>& container)
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "mat" << container.width() << "x" << container.height() << typeid(T).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}