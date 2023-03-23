#pragma once

#include "begin.h"
#include "../shapes/segment.h"
#include "../../../index_range.h"

namespace utils::math::geometry
	{

	template <typename DERIVED_T>
	inline segment<true > shape_with_vertices_base<DERIVED_T>::get_edges() noexcept
		{
		auto vertices_view{derived().get_vertices()};
		return utils::index_range{0, vertices_view.size(), [&vertices_view](size_t index) { return segment<true >{vertices_view[index], vertices_view[(index + 1) % vertices_view.size()]}; }};
		}
	template <typename DERIVED_T>
	inline segment<false> shape_with_vertices_base<DERIVED_T>::get_edges() const noexcept
		{
		auto vertices_view{derived().get_vertices()};
		return utils::index_range{0, vertices_view.size(), [&vertices_view](size_t index) { return segment<false>{vertices_view[index], vertices_view[(index + 1) % vertices_view.size()]}; }};
		}
	}