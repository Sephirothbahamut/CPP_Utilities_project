#pragma once

#include "../common/begin.h"

namespace utils::math::details
	{
	template <class T, typename DT> geometry::aabb vec_sized_specialization<T, 2, DT>::bounding_box() const noexcept { return {.ll{derived().x}, .up{derived().y}, .rr{derived().x}, .dw{derived().y}}; }

	template <class T, typename DT> DT& vec_sized_specialization<T, 2, DT>::scale_self    (const float      & scaling    ) noexcept { derived() *= scaling    ; return derived(); }
	template <class T, typename DT> DT& vec_sized_specialization<T, 2, DT>::rotate_self   (const angle::radf& rotation   ) noexcept { derived() += rotation   ; return derived(); }
	template <class T, typename DT> DT& vec_sized_specialization<T, 2, DT>::translate_self(const vec2f      & translation) noexcept { derived() += translation; return derived(); }
	}