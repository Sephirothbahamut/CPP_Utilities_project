#pragma once

#include <vector>
#include <stdexcept>


namespace utils
	{
	enum matrix_memory { width_first, height_first };

	template <typename T, matrix_memory MEMORY = matrix_memory::width_first >
	class matrix_dyn
		{
		public:
			matrix_dyn(size_t width, size_t height) : _width(width), _height(height), _arr(width * height) {}

			using coords_t = std::pair<size_t, size_t>;

			const size_t width()  const noexcept { return _width; }
			const size_t height() const noexcept { return _height; }
			const size_t size()   const noexcept { return _width * _height; }

			const T* data() const noexcept { return _arr.data(); }
			      T* data()       noexcept { return _arr.data(); }

			const std::vector<T>& get_vector() const noexcept { return _arr; }
			      std::vector<T>& get_vector()       noexcept { return _arr; }

			size_t get_index(size_t x, size_t y) const noexcept
				{
				if constexpr (MEMORY == matrix_memory::width_first) { return x + (y * _width); }
				else { return y + (x * _height); }
				}
			size_t get_x(size_t index) const noexcept { if constexpr (MEMORY == matrix_memory::width_first) { return index % width(); } else { return index / height(); } } //TODO test
			size_t get_y(size_t index) const noexcept { if constexpr (MEMORY == matrix_memory::width_first) { return index / width(); } else { return index % height(); } } //TODO test
			coords_t get_coords(size_t index) const noexcept { return {get_x(index), get_y(index)}; }
			
			const T& operator[](size_t i)        const noexcept { return _arr[i]; }
			      T& operator[](size_t i)              noexcept { return _arr[i]; }
			const T& operator[](coords_t coords) const noexcept { return _arr[get_index(coords.first, coords.second)]; }
			      T& operator[](coords_t coords)       noexcept { return _arr[get_index(coords.first, coords.second)]; }
			const T& at(size_t i)                const          { if (i >= size())                 { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[](i); }
			      T& at(size_t i)                               { if (i >= size())                 { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[](i); }
			const T& at(size_t x, size_t y)      const          { if (x >= _width || y >= _height) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			      T& at(size_t x, size_t y)                     { if (x >= _width || y >= _height) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			const T& at(coords_t coords)         const          { return at(coords.first, coords.second); }
			      T& at(coords_t coords)                        { return at(coords.first, coords.second); }

			// Arithmetic

			/*static T cross(const matrix_dyn& a, const matrix_dyn& b) noexcept { }//TODO
			static T dot(const matrix_dyn& a, const matrix_dyn& b) noexcept {}//TODO
			static T dot(const matrix_dyn& a, const matrix_dyn& b) noexcept {}//TODO*/

			const auto begin()   const noexcept { return _arr.begin(); }
			      auto begin()         noexcept { return _arr.begin(); }
			const auto end()     const noexcept { return _arr.end(); }
			      auto end()           noexcept { return _arr.end(); }
			const auto cbegin()  const noexcept { return _arr.cbegin(); }
			      auto cbegin()        noexcept { return _arr.cbegin(); }
			const auto cend()    const noexcept { return _arr.cend(); }
			      auto cend()          noexcept { return _arr.cend(); }
			const auto rbegin()  const noexcept { return _arr.rbegin(); }
			      auto rbegin()        noexcept { return _arr.rbegin(); }
			const auto rend()    const noexcept { return _arr.rend(); }
			      auto rend()          noexcept { return _arr.rend(); }
			const auto crbegin() const noexcept { return _arr.crbegin(); }
			      auto crbegin()       noexcept { return _arr.crbegin(); }
			const auto crend()   const noexcept { return _arr.crend(); }
			      auto crend()         noexcept { return _arr.crend(); }

#ifdef UTILS_COUT
			friend std::ostream& cout::operator<<(std::ostream& os, const matrix_dyn<T, MEMORY>& la);
#endif
		private:
			size_t _width;
			size_t _height;
			std::vector<T> _arr;

#ifdef UTILS_COUT
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
#endif
		};
	}