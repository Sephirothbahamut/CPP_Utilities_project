#pragma once
#include <utility>
#include <array>
#include <algorithm>

#include "../cout_utilities.h"
#include "../cout_containers.h"

namespace utils
	{
	enum matrix_memory { width_first, height_first };

	//declare output methods
	template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY = matrix_memory::width_first> class matrix;

	namespace cout
		{
		template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY>
		std::ostream& operator<<(std::ostream& os, const matrix<T, WIDTH, HEIGHT, MEMORY>& container) { return container.out(os); }
		}

	template <typename T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY = matrix_memory::width_first >
	class matrix
		{
		private:
			std::array<T, WIDTH* HEIGHT> _arr;

			std::ostream& out(std::ostream& os) const
				{
				namespace ccu = cout::support;

				ccu::brace(os, "[");
				if constexpr (MEMORY == matrix_memory::width_first)
					{
					for (size_t i = 0; i < size() - 1; i++)
						{
						ccu::value(os, (*this)[i]);
						ccu::separ(os, ", ");
						if (i == width() - 1) { os << "\n "; }
						}
					}
				else
					{
					for (size_t y = 0; y < height() - 1; y++)
						{
						for (size_t x = 0; x < width(); x++)
							{
							ccu::value(os, (*this)[get_index(x, y)]);
							ccu::separ(os, ", ");
							}
						os << "\n ";
						}
					for (size_t x = 0; x < width() - 1; x++)
						{
						ccu::value(os, (*this)[get_index(x, height() - 1)]);
						ccu::separ(os, ", ");
						}

					}
				ccu::value(os, (*this)[size() - 1]);
				ccu::brace(os, "]");
				return os;
				}
		public:
			using coords_t = std::pair<size_t, size_t>;

			using coords_t = std::pair<size_t, size_t>;
			constexpr size_t width()  const noexcept { return WIDTH; }
			constexpr size_t height() const noexcept { return HEIGHT; }
			constexpr size_t size()   const noexcept { return _arr.size(); }

			static size_t get_index(size_t x, size_t y) noexcept
				{
				if constexpr (MEMORY == matrix_memory::width_first) { return x + (y * WIDTH); }
				else { return y + (x * HEIGHT); }
				}

			const T& operator[](size_t i)        const noexcept { return _arr[i]; }
			      T& operator[](size_t i)              noexcept { return _arr[i]; }
			const T& operator[](coords_t coords) const noexcept { return _arr[get_index(coords.first, coords.second)]; }
			      T& operator[](coords_t coords)       noexcept { return _arr[get_index(coords.first, coords.second)]; }
			const T& at(size_t i)        const { if (i >= _arr.size()) { throw "Matrix access out of bounds."; } return operator[](i); }
			      T& at(size_t i)              { if (i >= _arr.size()) { throw "Matrix access out of bounds."; } return operator[](i); }
			const T& at(coords_t coords) const { if (coords.first >= WIDTH || coords.second >= HEIGHT) { throw "Matrix access out of bounds."; } return operator[](coords); }
			      T& at(coords_t coords)       { if (coords.first >= WIDTH || coords.second >= HEIGHT) { throw "Matrix access out of bounds."; } return operator[](coords); }

			// Arithmetic
			constexpr bool is_square() const noexcept { return WIDTH == HEIGHT; }
			T determinant()
				{
				if constexpr (!is_square()) { throw "Trying to find determinant on a non-square matrix."; }
				if constexpr (WIDTH == 2) { return (*this)[{0, 0}] * (*this)[{1, 1}] - (*this)[{1, 0}] * (*this)[{0, 1}]; }
				if constexpr (WIDTH == 3)
					{
					coords_t a = {0, 0}, b = {1, 0}, c = {2, 0}, d = {0, 1}, e = {1, 1}, f = {2, 1}, g = {0, 2}, h = {1, 2}, i = {2, 2};
					return
						(*this)[a] * (*this)[e] * (*this)[i] +
						(*this)[b] * (*this)[f] * (*this)[g] +
						(*this)[c] * (*this)[d] * (*this)[h] -
						(*this)[c] * (*this)[e] * (*this)[g] -
						(*this)[b] * (*this)[d] * (*this)[i] -
						(*this)[a] * (*this)[f] * (*this)[h];
					}
				if constexpr (WIDTH == 4)
					{
					return
						(*this)[{0, 3}] * (*this)[{1, 2}] * (*this)[{2, 1}] * (*this)[{3, 0}] - (*this)[{0, 2}] * (*this)[{1, 3}] * (*this)[{2, 1}] * (*this)[{3, 0}] -
						(*this)[{0, 3}] * (*this)[{1, 1}] * (*this)[{2, 2}] * (*this)[{3, 0}] + (*this)[{0, 1}] * (*this)[{1, 3}] * (*this)[{2, 2}] * (*this)[{3, 0}] +
						(*this)[{0, 2}] * (*this)[{1, 1}] * (*this)[{2, 3}] * (*this)[{3, 0}] - (*this)[{0, 1}] * (*this)[{1, 2}] * (*this)[{2, 3}] * (*this)[{3, 0}] -
						(*this)[{0, 3}] * (*this)[{1, 2}] * (*this)[{2, 0}] * (*this)[{3, 1}] + (*this)[{0, 2}] * (*this)[{1, 3}] * (*this)[{2, 0}] * (*this)[{3, 1}] +
						(*this)[{0, 3}] * (*this)[{1, 0}] * (*this)[{2, 2}] * (*this)[{3, 1}] - (*this)[{0, 0}] * (*this)[{1, 3}] * (*this)[{2, 2}] * (*this)[{3, 1}] -
						(*this)[{0, 2}] * (*this)[{1, 0}] * (*this)[{2, 3}] * (*this)[{3, 1}] + (*this)[{0, 0}] * (*this)[{1, 2}] * (*this)[{2, 3}] * (*this)[{3, 1}] +
						(*this)[{0, 3}] * (*this)[{1, 1}] * (*this)[{2, 0}] * (*this)[{3, 2}] - (*this)[{0, 1}] * (*this)[{1, 3}] * (*this)[{2, 0}] * (*this)[{3, 2}] -
						(*this)[{0, 3}] * (*this)[{1, 0}] * (*this)[{2, 1}] * (*this)[{3, 2}] + (*this)[{0, 0}] * (*this)[{1, 3}] * (*this)[{2, 1}] * (*this)[{3, 2}] +
						(*this)[{0, 1}] * (*this)[{1, 0}] * (*this)[{2, 3}] * (*this)[{3, 2}] - (*this)[{0, 0}] * (*this)[{1, 1}] * (*this)[{2, 3}] * (*this)[{3, 2}] -
						(*this)[{0, 2}] * (*this)[{1, 1}] * (*this)[{2, 0}] * (*this)[{3, 3}] + (*this)[{0, 1}] * (*this)[{1, 2}] * (*this)[{2, 0}] * (*this)[{3, 3}] +
						(*this)[{0, 2}] * (*this)[{1, 0}] * (*this)[{2, 1}] * (*this)[{3, 3}] - (*this)[{0, 0}] * (*this)[{1, 2}] * (*this)[{2, 1}] * (*this)[{3, 3}] -
						(*this)[{0, 1}] * (*this)[{1, 0}] * (*this)[{2, 2}] * (*this)[{3, 3}] + (*this)[{0, 0}] * (*this)[{1, 1}] * (*this)[{2, 2}] * (*this)[{3, 3}];
					}
				//TODO
				throw "Determinant for matrices larger than 4.";
				}


			matrix<T, WIDTH, HEIGHT, MEMORY>  operator+ (const T& oth) const noexcept { return matrix<T, WIDTH, HEIGHT, MEMORY>{*this} += oth; }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator- (const T& oth) const noexcept { return matrix<T, WIDTH, HEIGHT, MEMORY>{*this} -= oth; }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator* (const T& oth) const noexcept { return matrix<T, WIDTH, HEIGHT, MEMORY>{*this} *= oth; }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator/ (const T& oth) const noexcept { return matrix<T, WIDTH, HEIGHT, MEMORY>{*this} /= oth; }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator+=(const T& oth)       noexcept { std::transform(std::begin(_arr), std::end(_arr), std::begin(_arr), [](T value) { return value + oth; }); }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator-=(const T& oth)       noexcept { std::transform(std::begin(_arr), std::end(_arr), std::begin(_arr), [](T value) { return value - oth; }); }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator*=(const T& oth)       noexcept { std::transform(std::begin(_arr), std::end(_arr), std::begin(_arr), [](T value) { return value * oth; }); }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator/=(const T& oth)       noexcept { std::transform(std::begin(_arr), std::end(_arr), std::begin(_arr), [](T value) { return value / oth; }); }
			
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator+ (const matrix<T, WIDTH, HEIGHT, MEMORY>& oth) const noexcept { matrix<T, WIDTH, HEIGHT, MEMORY> ret; std::transform(_arr.begin(), _arr.end(), oth._arr.begin(), ret.begin(), [](T& a_i, const T& b_i) { return a_i + b_i; }); return ret; }
			matrix<T, WIDTH, HEIGHT, MEMORY>  operator- (const matrix<T, WIDTH, HEIGHT, MEMORY>& oth) const noexcept { matrix<T, WIDTH, HEIGHT, MEMORY> ret; std::transform(_arr.begin(), _arr.end(), oth._arr.begin(), ret.begin(), [](T& a_i, const T& b_i) { return a_i + b_i; }); return ret; }
			matrix<T, WIDTH, HEIGHT, MEMORY>& operator+=(const matrix<T, WIDTH, HEIGHT, MEMORY>& oth)       noexcept { std::transform(_arr.begin(), _arr.end(), oth._arr.begin(), _arr.begin(), [](T& a_i, const T& b_i) { return a_i + b_i; }); return *this; }
			matrix<T, WIDTH, HEIGHT, MEMORY>& operator-=(const matrix<T, WIDTH, HEIGHT, MEMORY>& oth)       noexcept { std::transform(_arr.begin(), _arr.end(), oth._arr.begin(), _arr.begin(), [](T& a_i, const T& b_i) { return a_i + b_i; }); return *this; }


			/*static T cross(const matrix& a, const matrix& b) noexcept { }//TODO
			static T dot(const matrix& a, const matrix& b) noexcept {}//TODO
			static T dot(const matrix& a, const matrix& b) noexcept {}//TODO*/

			friend std::ostream& cout::operator<<(std::ostream& os, const matrix<T, WIDTH, HEIGHT, MEMORY>& la);
		};

	//TODO all
	/*
	export template <typename T>
	class Matrix_dyn
		{
		private:
			std::vector<T> _arr;
			size_t _width, _height;


		public:
			Matrix_dyn(size_t width, size_t height)
			Matrix_dyn()
			Matrix& resize(size_t width, size_t height)
			Matrix& add_columns(size_t amount = 1);
			Matrix& add_rows(size_t amount = 1);
			Matrix& remove_columns(size_t amount = 1);
			Matrix& remove_rows(size_t amount = 1);
		};*/
	}