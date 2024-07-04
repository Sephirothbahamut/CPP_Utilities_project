#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "../details/base_types.h"
#include "../../../oop/disable_move_copy.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type, ends ENDS, size_t extent = std::dynamic_extent>
		struct vertices : utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, extent, true>, utils::math::geometry::shape_flag
			{
			inline static constexpr ends ends{ENDS};

			using storage_t = utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, extent, true>;
			using storage_t::size;
			using storage_t::multiple;
			using typename storage_t::value_type;
			using typename storage_t::const_aware_value_type;
			using storage_t::operator[];

			template <bool is_function_const>
			using vertex = std::conditional_t
				<
				(is_function_const || storage_type.is_const()),
				const value_type, 
				const_aware_value_type
				>;

			/// <summary> For closed ends vertices sequences, wraps aaround indices over base size.</summary>
			utils_gpu_available size_t raw_index_to_ends_aware_index(size_t index) const noexcept
				{
				if constexpr (ends.is_closed())
					{
					return index % size();
					}
				else
					{
					if (index >= size())
						{
						throw std::runtime_error{"invalid index"};
						}
					return index;
					}
				}

			utils_gpu_available size_t ends_aware_size() const noexcept
				{
				return ends.is_closed() ? size() + 1 : size();
				}

			utils_gpu_available const       value_type& ends_aware_access(size_t index) const noexcept                                    { return operator[](raw_index_to_ends_aware_index(index)); }
			utils_gpu_available const_aware_value_type& ends_aware_access(size_t index)       noexcept requires(!storage_type.is_const()) { return operator[](raw_index_to_ends_aware_index(index)); }
			};
		}

	namespace concepts
		{
		template <typename T> concept vertices = std::derived_from<T, shape::generic::vertices<T::storage_type, T::ends, T::extent>>;
		}
	}