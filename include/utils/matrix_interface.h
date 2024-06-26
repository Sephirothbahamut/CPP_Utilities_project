#pragma once

#include <ranges>

#include "math/vec2.h"
#include "console/colour.h"
#include "compilation/gpu.h"
#include "compilation/debug.h"

namespace utils
	{
	enum matrix_memory { width_first, height_first };

	namespace details
		{
		template<size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
		class matrix_root
			{
			public:
				static constexpr matrix_memory memory_layout{MEMORY_LAYOUT};
				static constexpr math::vec2s static_sizes {WIDTH, HEIGHT};
				static constexpr size_t      static_size  {WIDTH * HEIGHT};
				static constexpr size_t      static_width {WIDTH };
				static constexpr size_t      static_height{HEIGHT};

				utils_gpu_available math::vec2s sizes () const noexcept { return static_sizes    ; }
				utils_gpu_available size_t      size  () const noexcept { return static_size     ; }
				utils_gpu_available size_t      width () const noexcept { return static_sizes.x(); }
				utils_gpu_available size_t      height() const noexcept { return static_sizes.y(); }

			private:
			};

		template<matrix_memory MEMORY_LAYOUT>
		class matrix_root<0, 0, MEMORY_LAYOUT>
			{
			public:
				static constexpr matrix_memory memory_layout{MEMORY_LAYOUT};

				utils_gpu_available matrix_root(math::vec2s sizes) : _sizes{sizes}, _size{sizes.x() * sizes.y()} {}

				utils_gpu_available math::vec2s sizes () const noexcept { return _sizes    ; }
				utils_gpu_available size_t      size  () const noexcept { return _size     ; }
				utils_gpu_available size_t      width () const noexcept { return _sizes.x(); }
				utils_gpu_available size_t      height() const noexcept { return _sizes.y(); }

			private:
				math::vec2s _sizes{};
				size_t      _size {};
			};
		
		template <typename DERIVED_T, typename CONTAINER_T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
		class matrix_crtp : matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>
			{
			public:
				using derived_t = DERIVED_T;

				using container_t            = CONTAINER_T;

				using value_type             = container_t::value_type;
				using size_type              = container_t::size_type;
				using reference              = container_t::reference;
				using const_reference        = container_t::const_reference;
				using pointer                = container_t::pointer;
				using const_pointer          = container_t::const_pointer;
				using iterator               = container_t::iterator;
				//using const_iterator         = container_t::const_iterator;
				using reverse_iterator       = container_t::reverse_iterator;
				//using const_reverse_iterator = container_t::const_reverse_iterator;

			protected:
				utils_gpu_available constexpr const derived_t  & derived  () const noexcept { return static_cast<const derived_t&>(*this); }
				utils_gpu_available constexpr       derived_t  & derived  ()       noexcept { return static_cast<      derived_t&>(*this); }
				utils_gpu_available constexpr const container_t& container() const noexcept { return derived().container(); }
				utils_gpu_available constexpr       container_t& container()       noexcept { return derived().container(); }

			public:
				using matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>::matrix_root;
				using matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>::memory_layout;
				using matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>::sizes ;
				using matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>::size  ;
				using matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>::width ;
				using matrix_root<WIDTH, HEIGHT, MEMORY_LAYOUT>::height;

				utils_gpu_available size_t get_index(math::vec2s coords) const noexcept { return get_index(coords.x(), coords.y()); }
				utils_gpu_available size_t get_index(size_t x, size_t y) const noexcept
					{
					if constexpr (memory_layout == matrix_memory::width_first) { return x + (y * sizes().x()); }
					else { return y + (x * sizes().y); }
					}
				utils_gpu_available size_t      get_x     (size_t index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index % sizes().x(); } else { return index / sizes().y(); } }
				utils_gpu_available size_t      get_y     (size_t index) const noexcept { if constexpr (memory_layout == matrix_memory::width_first) { return index / sizes().x(); } else { return index % sizes().y(); } }
				utils_gpu_available math::vec2s get_coords(size_t index) const noexcept { return {get_x(index), get_y(index)}; }
				
				utils_gpu_available const_reference operator[](size_type   i     ) const noexcept { return container()[i]; }
				utils_gpu_available       reference operator[](size_type   i     )       noexcept { return container()[i]; }
				utils_gpu_available const_reference operator[](math::vec2s coords) const noexcept { return container()[get_index(coords.x(), coords.y())];    }
				utils_gpu_available       reference operator[](math::vec2s coords)       noexcept { return container()[get_index(coords.x(), coords.y())];    }
				                    const_reference at(size_type i             ) const { if (!validate_index (i   )) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]( i    ); }
				                          reference at(size_type i             )       { if (!validate_index (i   )) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]( i    ); }
				                    const_reference at(size_type x, size_type y) const { if (!validate_coords(x, y)) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
				                          reference at(size_type x, size_type y)       { if (!validate_coords(x, y)) { throw std::out_of_range{"Matrix access out of bounds."}; } return operator[]({x, y}); }
				                    const_reference at(math::vec2s coords      ) const { return at(coords.x(), coords.y()); }
				                          reference at(math::vec2s coords      )       { return at(coords.x(), coords.y()); }

				utils_gpu_available bool validate_coords(math::vec2s coords      ) const noexcept { return validate_coords(coords.x(), coords.y()); }
				utils_gpu_available bool validate_coords(size_type x, size_type y) const noexcept { return (x < sizes().x()) && (y < sizes().y()); }
				utils_gpu_available bool validate_index (size_type i             ) const noexcept { return i < size(); }

				utils_gpu_available const auto begin  () const noexcept { return container().begin  (); }
				utils_gpu_available       auto begin  ()       noexcept { return container().begin  (); }
				utils_gpu_available const auto end    () const noexcept { return container().end    (); }
				utils_gpu_available       auto end    ()       noexcept { return container().end    (); }
				utils_gpu_available const auto cbegin () const noexcept { return container().cbegin (); }
				utils_gpu_available       auto cbegin ()       noexcept { return container().cbegin (); }
				utils_gpu_available const auto cend   () const noexcept { return container().cend   (); }
				utils_gpu_available       auto cend   ()       noexcept { return container().cend   (); }
				utils_gpu_available const auto rbegin () const noexcept { return container().rbegin (); }
				utils_gpu_available       auto rbegin ()       noexcept { return container().rbegin (); }
				utils_gpu_available const auto rend   () const noexcept { return container().rend   (); }
				utils_gpu_available       auto rend   ()       noexcept { return container().rend   (); }
				utils_gpu_available const auto crbegin() const noexcept { return container().crbegin(); }
				utils_gpu_available       auto crbegin()       noexcept { return container().crbegin(); }
				utils_gpu_available const auto crend  () const noexcept { return container().crend  (); }
				utils_gpu_available       auto crend  ()       noexcept { return container().crend  (); }
			};
		}

	template <typename container_T, size_t WIDTH = 0, size_t HEIGHT = 0, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
	class matrix_observer : public details::matrix_crtp<matrix_observer<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>, container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>
		{
		private:
			using self_t = matrix_observer<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>;
			using base_t = details::matrix_crtp<self_t, container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>;
			friend class base_t;

		public:
			using container_t = container_T;

			utils_gpu_available matrix_observer(container_T& container) requires(WIDTH != 0 && HEIGHT != 0) :
				base_t{},
				_container{container}
				{
				//if constexpr (utils::compilation::debug && !utils::compilation::gpu::device)
				//	{
				//	if (container.size() != base_t::size())
				//		{
				//		throw std::length_error{"The size of the provided container doesn't match the size of the matrix you're trying to create."};
				//		}
				//	}
				}

			utils_gpu_available matrix_observer(utils::math::vec2s sizes, container_T& container) requires(WIDTH == 0 && HEIGHT == 0) :
				base_t{sizes},
				_container{container}
				{
				//if constexpr (utils::compilation::debug && !utils::compilation::gpu::device)
				//	{
				//	if (container.size() != base_t::size())
				//		{
				//		throw std::length_error{"The size of the provided container doesn't match the size of the matrix you're trying to create."};
				//		}
				//	}
				}

			utils_gpu_available constexpr const container_t& container() const noexcept { return _container.get(); }
			utils_gpu_available constexpr       container_t& container()       noexcept { return _container.get(); }

		private:
			std::reference_wrapper<container_t> _container;
		};

	template <typename container_T, size_t WIDTH = 0, size_t HEIGHT = 0, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
	class matrix_wrapper : public details::matrix_crtp<matrix_wrapper<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>, container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>
		{
		// Requires container to be an observer type like a view or pointer
		private:
			using self_t = matrix_wrapper<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>;
			using base_t = details::matrix_crtp<self_t, container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>;
			friend class base_t;

		public:
			using container_t = container_T;

			utils_gpu_available constexpr matrix_wrapper(const container_T& container) requires(WIDTH != 0 && HEIGHT != 0) :
				base_t{},
				_container{container}
				{
				//if constexpr (utils::compilation::debug && !utils::compilation::gpu::device)
				//	{
				//	if (container.size() != base_t::size())
				//		{
				//		throw std::length_error{"The size of the provided container doesn't match the size of the matrix you're trying to create."};
				//		}
				//	}
				}
			
			utils_gpu_available constexpr matrix_wrapper(const utils::math::vec2s& sizes) requires(WIDTH == 0 && HEIGHT == 0) :
				base_t{sizes},
				_container(sizes.x() * sizes.y())
				{}

			utils_gpu_available matrix_wrapper(utils::math::vec2s sizes, container_T container) requires(WIDTH == 0 && HEIGHT == 0) :
				base_t{sizes},
				_container{container}
				{
				//if constexpr (utils::compilation::debug && !utils::compilation::gpu::device)
				//	{
				//	if (container.size() != base_t::size())
				//		{
				//		throw std::length_error{"The size of the provided container doesn't match the size of the matrix you're trying to create."};
				//		}
				//	}
				}

			utils_gpu_available constexpr const container_t       & container() const noexcept { return _container; }
			utils_gpu_available constexpr       container_t       & container()       noexcept { return _container; }
			utils_gpu_available constexpr const base_t::value_type* data     () const noexcept { return container().data(); }
			utils_gpu_available constexpr       base_t::value_type* data     ()       noexcept { return container().data(); }

		private:
			container_t _container;
		};

	template <typename T, size_t WIDTH = 0, size_t HEIGHT = 0, matrix_memory MEMORY_LAYOUT = matrix_memory::width_first>
	struct matrix : matrix_wrapper<std::array<T, WIDTH * HEIGHT>, WIDTH, HEIGHT, MEMORY_LAYOUT>
		{
		using container_t = std::array<T, WIDTH* HEIGHT>;
		using wrapper_t = matrix_wrapper<container_t, WIDTH, HEIGHT, MEMORY_LAYOUT>;

		template <typename ...Args>
		utils_gpu_available constexpr matrix(Args&&... args) : wrapper_t{container_t{std::forward<Args>(args)...}} {}
		};

	template <typename T, matrix_memory MEMORY_LAYOUT>
	struct matrix<T, 0, 0, MEMORY_LAYOUT> : matrix_wrapper<std::vector<T>, 0, 0, MEMORY_LAYOUT>
		{
		using container_t = std::vector<T>;
		using wrapper_t = matrix_wrapper<container_t, 0, 0, MEMORY_LAYOUT>;
		matrix(const math::vec2s& sizes) : wrapper_t{sizes, container_t(sizes.x() * sizes.y())} {}

		template <class range_t>
		matrix(const math::vec2s& sizes, const range_t& range) : wrapper_t{sizes, container_t(range.begin(), range.end())} {}
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
				for (size_t y = 0; y < container.sizes().y() - 1; y++)
					{
					for (size_t x = 0; x < container.sizes().x() - 1; x++)
						{
						os << ucc::value << container[{x, y}] << ucc::separ << ", ";
						}
					os << ucc::value << container[{container.sizes().x() - 1, y}] << " | ";
					}
				
				for (size_t x = 0; x < container.sizes().x() - 1; x++)
					{
					os << ucc::value << container[{x, container.sizes().y() - 1}] << ucc::separ << ", ";
					}
				os << ucc::value << container[{container.sizes().x() - 1, container.sizes().y() - 1}];
				}
			return os << ucc::brace << "]";
			}
		}

	template <std::ranges::random_access_range container_T, size_t WIDTH, size_t HEIGHT, matrix_memory MEMORY_LAYOUT>
	inline ::std::ostream& operator<<(::std::ostream& os, const utils::matrix_wrapper<container_T, WIDTH, HEIGHT, MEMORY_LAYOUT>& container) noexcept
		{
		namespace ucc = utils::console::colour;
		os << ucc::type << "mat" << container.sizes().x() << "x" << container.sizes().y() << typeid(container_T::value_type).name();
		return utils::output::typeless::operator<<(os, container);
		}
	}