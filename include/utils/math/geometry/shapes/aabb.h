#pragma once

#include "../../../index_range.h"
#include "edge_ref.h"
#include "../common/begin.h"

namespace utils::math
	{
	template <>
	inline auto rect<float>::get_edges() noexcept
		{
		return utils::index_range{0, 4, [this](size_t index)
			{
			switch (index)
				{
				case 0:          return geometry::edge_ref{ul(), ur()};
				case 1:          return geometry::edge_ref{ur(), dr()};
				case 2:          return geometry::edge_ref{dr(), dl()};
				case 3: default: return geometry::edge_ref{dl(), ul()};
				}
			}};
		}
	template <>
	inline auto rect<float>::get_edges() const noexcept
		{
		return utils::index_range{0, 4, [this](size_t index)
			{
			switch (index)
				{
				case 0:          return geometry::segment{ul(), ur()};
				case 1:          return geometry::segment{ur(), dr()};
				case 2:          return geometry::segment{dr(), dl()};
				case 3: default: return geometry::segment{dl(), ul()};
				}
			}};
		}

	template <>
	inline rect<float>& rect<float>::scale_self    (const float      & scaling    ) noexcept
		{
		ll *= scaling;
		up *= scaling;
		rr *= scaling;
		dw *= scaling;
		return *this; 
		}

	template <>
	inline rect<float>& rect<float>::rotate_self   (const angle::radf& rotation   ) noexcept { return *this; }

	template <>
	inline rect<float>& rect<float>::translate_self(const vec2f      & translation) noexcept { pos() += translation; return *this; }
	}