#pragma once

#include "root.h"

#include "../../vec2.h"
#include "../../rect.h"
#include "../../transform2.h"

namespace utils::math::geometry
	{
	struct closest_point_and_distance_t 
		{
		point position{std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()}; 
		float distance{std::numeric_limits<float>::infinity()}; 
		};

	template <typename derived_t>
	template <concepts::shape other_t>
	closest_point_and_distance_t shape_base<derived_t>::closest_point_and_distance(const other_t& b) const noexcept { return {derived().closest_point_to(b), derived().distance_min(b)}; };

	template <typename derived_t>
	template <concepts::shape other_t>
	point shape_base<derived_t>::closest_point_to(const other_t& b) const noexcept { return derived().closest_point_and_distance(b).position; };

	template <typename derived_t>
	template <concepts::shape other_t>
	float shape_base<derived_t>::distance_min(const other_t& b) const noexcept { return derived().closest_point_and_distance(b).distance; }

	template <typename derived_t>
	template <concepts::shape other_t>
	point shape_base<derived_t>::vector_to(const other_t& b) const noexcept { return b.closest_point_to(derived()) - derived().closest_point_to(b); }

	template <typename derived_t>
	template <concepts::shape other_t>
	std::optional<point> shape_base<derived_t>::intersects(const other_t& b) const noexcept { return derived().distance_min(b) == 0; };

	template <typename derived_t>
	template <concepts::shape other_t>
	std::optional<point> shape_base<derived_t>::intersection(const other_t& b) const noexcept { return derived().intersects(b) ? std::optional<point>{derived().closest_point_to(b)} : std::nullopt; };

	template <typename derived_t>
	template <concepts::shape other_t>
	bool shape_base<derived_t>::contains(const other_t& b) const noexcept { return false; };

	template <typename derived_t>
	template <concepts::shape other_t>
	bool shape_base<derived_t>::collides_with(const other_t& b) const noexcept
		{
		/*if constexpr (!  hollow)*/ { if (derived().contains(b)) { return true; } }
		/*if constexpr (!b.hollow)*/ { if (b.contains(derived())) { return true; } }
		return derived().intersection(b).has_value();
		};

	template <typename derived_t> derived_t shape_base<derived_t>::scale    (const float      & scaling    ) const noexcept { auto ret{derived()}; ret.scale_self    (scaling    ); return ret; }
	template <typename derived_t> derived_t shape_base<derived_t>::rotate   (const angle::radf& rotation   ) const noexcept { auto ret{derived()}; ret.rotate_self   (rotation   ); return ret; }
	template <typename derived_t> derived_t shape_base<derived_t>::translate(const vec2f      & translation) const noexcept { auto ret{derived()}; ret.translate_self(translation); return ret; }
	template <typename derived_t> derived_t shape_base<derived_t>::transform(const transform2 & transform  ) const noexcept { auto ret{derived()}; ret.transform_self(transform  ); return ret; }
	template <typename derived_t> derived_t& shape_base<derived_t>::transform_self(const transform2& transform) noexcept 
		{
		derived().scale_self    (transform.scaling    );
		derived().rotate_self   (transform.rotation   );
		derived().translate_self(transform.translation);
		return derived();
		}
	}

