#pragma once

#include <ranges>

#include "math/vec2.h"
#include "compilation/CUDA.h"
#include "compilation/debug.h"

namespace utils
	{
	enum matrix_memory { width_first, height_first };

	namespace details
		{
		template<size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
		class matrix_interface
			{
			public:
				static constexpr matrix_memory memory_layout{MEMORY_LAYOUT};
				static constexpr math::vec2s static_sizes{WIDTH, HEIGHT};
				static constexpr size_t      static_size{static_sizes.x * static_sizes.y};

				utils_cuda_available math::vec2s sizes() const noexcept { return static_sizes; }
				utils_cuda_available size_t      size () const noexcept { return static_size ; }

			private:
			};

		template<matrix_memory MEMORY_LAYOUT>
		class matrix_interface<0, 0, MEMORY_LAYOUT>
			{
			public:
				static constexpr matrix_memory memory_layout{MEMORY_LAYOUT};

				utils_cuda_available matrix_interface(math::vec2s sizes) : _sizes{sizes}, _size{sizes.x * sizes.y} {}

				utils_cuda_available math::vec2s sizes() const noexcept { return _sizes; }
				utils_cuda_available size_t      size () const noexcept { return _size ; }

			private:
				utils_cuda_available math::vec2s _sizes{};
				utils_cuda_available size_t      _size {};
			};
		}

	template<size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
	class matrix_interface : public details::matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>
		{
		public:
			using details::matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>::matrix_interface;

			utils_cuda_available size_t get_index(math::vec2s coords) const noexcept { return get_index(coords.x, coords.y); }
			utils_cuda_available size_t get_index(size_t x, size_t y) const noexcept
				{
				if constexpr (memory_layout == matrix_memory::width_first) { return x + (y * size().x); }
				else { return y + (x * size().y); }
				}
			utils_cuda_available size_t      get_x     (size_t index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index % size().x; } else { return index / size().y; } }
			utils_cuda_available size_t      get_y     (size_t index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index / size().x; } else { return index % size().y; } }
			utils_cuda_available math::vec2s get_coords(size_t index) const noexcept { return {get_x(index), get_y(index)}; }

		private:
		};

	template <std::ranges::random_access_range container_T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
	class matrix_wrapper : public matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>
		{
		using mat_int_t = matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>;
		public:
			utils_cuda_available matrix_wrapper(container_T& container) requires(WIDTH != 0 && HEIGHT != 0) :
				matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>{},
				container{container}
				{
				if constexpr (utils::compilation::debug)
					{
					if (container.size() != mat_int_t::size())
						{
						throw std::length_error{"The size of the provided container doesn't match the size of the matrix you're trying to create."};
						}
					}
				}

			utils_cuda_available matrix_wrapper(utils::math::vec2s sizes, container_T& container) requires(WIDTH == 0 && HEIGHT == 0) :
				matrix_interface<WIDTH, HEIGHT, MEMORY_LAYOUT>{sizes},
				container{container}
				{
				if constexpr (utils::compilation::debug)
					{
					if (container.size() != mat_int_t::size())
						{
						throw std::length_error{"The size of the provided container doesn't match the size of the matrix you're trying to create."};
						}
					}
				}

			using container_t            = container_T;
			using value_type             = container_t::value_type;
			using size_type              = container_t::size_type;
			using reference              = container_t::reference;
			using const_reference        = container_t::const_reference;
			using pointer                = container_t::pointer;
			using const_pointer          = container_t::const_pointer;
			using iterator               = container_t::iterator;
			using const_iterator         = container_t::const_iterator;
			using reverse_iterator       = container_t::reverse_iterator;
			using const_reverse_iterator = container_t::const_reverse_iterator;

			utils_cuda_available const_reference operator[](size_type i)              const noexcept { return _arr[i]; }
			utils_cuda_available       reference operator[](size_type i)                    noexcept { return _arr[i]; }
			utils_cuda_available const_reference operator[](math::vec2s coords)       const noexcept { return _arr[mat_int_t::get_index(coords.x, coords.y)];    }
			utils_cuda_available       reference operator[](math::vec2s coords)             noexcept { return _arr[mat_int_t::get_index(coords.x, coords.y)];    }
			utils_cuda_available const_reference at(size_type i             ) const { if (i >= mat_int_t::size ()                               ) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]( i    ); }
			utils_cuda_available       reference at(size_type i             )       { if (i >= mat_int_t::size ()                               ) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]( i    ); }
			utils_cuda_available const_reference at(size_type x, size_type y) const { if (x >= mat_int_t::sizes().x || y >= mat_int_t::sizes().y) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			utils_cuda_available       reference at(size_type x, size_type y)       { if (x >= mat_int_t::sizes().x || y >= mat_int_t::sizes().y) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
			utils_cuda_available const_reference at(math::vec2s coords      ) const { return at(coords.x, coords.y); }
			utils_cuda_available       reference at(math::vec2s coords      )       { return at(coords.x, coords.y); }

			utils_cuda_available const auto begin  () const noexcept { return container.begin  (); }
			utils_cuda_available       auto begin  ()       noexcept { return container.begin  (); }
			utils_cuda_available const auto end    () const noexcept { return container.end    (); }
			utils_cuda_available       auto end    ()       noexcept { return container.end    (); }
			utils_cuda_available const auto cbegin () const noexcept { return container.cbegin (); }
			utils_cuda_available       auto cbegin ()       noexcept { return container.cbegin (); }
			utils_cuda_available const auto cend   () const noexcept { return container.cend   (); }
			utils_cuda_available       auto cend   ()       noexcept { return container.cend   (); }
			utils_cuda_available const auto rbegin () const noexcept { return container.rbegin (); }
			utils_cuda_available       auto rbegin ()       noexcept { return container.rbegin (); }
			utils_cuda_available const auto rend   () const noexcept { return container.rend   (); }
			utils_cuda_available       auto rend   ()       noexcept { return container.rend   (); }
			utils_cuda_available const auto crbegin() const noexcept { return container.crbegin(); }
			utils_cuda_available       auto crbegin()       noexcept { return container.crbegin(); }
			utils_cuda_available const auto crend  () const noexcept { return container.crend  (); }
			utils_cuda_available       auto crend  ()       noexcept { return container.crend  (); }

		private:
			container_t& container;
		};
	}

namespace utils::output
	{
	namespace typeless
		{
		template <std::ranges::random_access_range container_T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
		inline ::std::ostream& operator<<(::std::ostream& os, const utils::matrix_wrapper<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>& container) noexcept
			{
			namespace ucc = utils::console::colour;
			os << ucc::brace << "[";

			if (!container.empty())
				{
				for (size_t y = 0; y < container.sizes().y - 1; y++)
					{
					for (size_t x = 0; x < container.sizes().x - 1; x++)
						{
						os << ucc::value << container[{x, y}] << ucc::separ << ", ";
						}
					os << ucc::value << container[{container.sizes().x - 1, y}] << " | ";
					}
				
				for (size_t x = 0; x < container.sizes().x - 1; x++)
					{
					os << ucc::value << container[{x, container.sizes().y - 1}] << ucc::separ << ", ";
					}
				os << ucc::value << container[{container.sizes().x - 1, container.sizes().y - 1}];
				}
			return os << ucc::brace << "]";
			}
		}

	template <std::ranges::random_access_range container_T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::matrix_wrapper<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>& container) noexcept
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "mat" << container.sizes().x << "x" << container.sizes().y << typeid(T).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}