#pragma once

#include "vec.h"
#include "angle.h"

namespace utils::math
	{
	namespace details
		{
		template<size_t size, template <typename, size_t> class unspecialized_derived_T>
		class utils_oop_empty_bases vec_typed_specialization<size_t, size, unspecialized_derived_T>
			{
			private:
				using self_t = unspecialized_derived_T<size_t, size>;
				utils_gpu_available constexpr const self_t& self() const noexcept { return static_cast<const self_t&>(*this); }
				utils_gpu_available constexpr       self_t& self()       noexcept { return static_cast<      self_t&>(*this); }

			public:
				utils_gpu_available constexpr size_t sizes_to_size() const noexcept 
					{
					size_t ret{1};
					for (const auto& value : self())
						{
						ret *= value;
						}
					return ret;
					}

				utils_gpu_available constexpr size_t coords_to_index(math::vec_s<size> coords) const noexcept
					{
					size_t ret{0};
					size_t multiplier{1};

					for (size_t i{0}; i < size; i++)
						{
						const auto coord{coords[i]};
						const auto size_{self()[i]};
						assert(coord < size_);
						ret += coord * multiplier;
						multiplier *= size_;
						}

					return ret;
					}

				//TODO N-dimensional, for now I'm tired and 2d is enough
				utils_gpu_available constexpr math::vec_s<size> index_to_coords(size_t index) const noexcept
					requires(size == 2)
					{
					//math::vec_s<size> ret;
					//size_t multiplier{size};
					//
					//for (size_t i{size}; i != 0; i--)
					//	{
					//	multiplier /= self()[i - 1];
					//	ret[i - 1] = index / multiplier;
					//	index -= ret[i - 1] * multiplier;
					//	}
					//
					//return ret;

					return 
						{
						index % self().x(),
						index / self().x()
						};
					}
			};
		}
	}
