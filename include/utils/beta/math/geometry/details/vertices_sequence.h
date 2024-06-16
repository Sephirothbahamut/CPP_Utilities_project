#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "base_types.h"
#include "../shape/ab.h"
#include "../shape/point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <typename derived_t, storage::type storage_type, ends ENDS, size_t EXTENT>
		struct vertices_sequence : utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, EXTENT, true>
			{
			using self_t        = typename derived_t::self_t       ;
			using nonref_self_t = typename derived_t::nonref_self_t;

			inline static constexpr ends   ends  {ENDS  };
			inline static constexpr size_t extent{EXTENT};
			using storage_t = utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, extent, true>;

			utils_gpu_available constexpr nonref_self_t scale    (const float                    & scaling    ) const noexcept { nonref_self_t ret{*this}; return ret.scale_self    (scaling    ); }
			utils_gpu_available constexpr nonref_self_t rotate   (const angle::base<float, 360.f>& rotation   ) const noexcept { nonref_self_t ret{*this}; return ret.rotate_self   (rotation   ); }
			utils_gpu_available constexpr nonref_self_t translate(const vec2f                    & translation) const noexcept { nonref_self_t ret{*this}; return ret.translate_self(translation); }
			utils_gpu_available constexpr nonref_self_t transform(const utils::math::transform2  & transform  ) const noexcept { nonref_self_t ret{*this}; return ret.transform_self(transform  ); }

			utils_gpu_available constexpr self_t& scale_self    (const float                    & scaling    ) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.scale_self    (scaling    ); } return *this; }
			utils_gpu_available constexpr self_t& rotate_self   (const angle::base<float, 360.f>& rotation   ) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.rotate_self   (rotation   ); } return *this; }
			utils_gpu_available constexpr self_t& translate_self(const vec2f                    & translation) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.translate_self(translation); } return *this; }
			utils_gpu_available constexpr self_t& transform_self(const utils::math::transform2  & transform  ) noexcept requires(!storage_type.is_const()) { for(auto& vertex : (*this)) { vertex.transform_self(transform  ); } return *this; }
			};
		}
	}