#pragma once
#include "../shapes.h"

namespace utils::math::details
	{
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::point         & b) const noexcept { return *this; }
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::segment       & b) const noexcept { return *this; }
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::aabb          & b) const noexcept { return *this; }
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::polygon       & b) const noexcept { return *this; }
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::convex_polygon& b) const noexcept { return *this; }
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::circle        & b) const noexcept { return *this; }
	template <class T, typename DT> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::capsule       & b) const noexcept { return *this; }
	template <class T, typename DT> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::point         & b) const noexcept { return vec2f::distance(derived(), vec2f{b}); }
	template <class T, typename DT> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::segment       & b) const noexcept { return distance_min(b.closest_point_to(derived())); }
	template <class T, typename DT> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::aabb          & b) const noexcept { return distance_min(b.closest_point_to(derived())); }
	template <class T, typename DT> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::polygon       & b) const noexcept { return distance_min(b.closest_point_to(derived())); }
	template <class T, typename DT> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::convex_polygon& b) const noexcept { return distance_min(b.closest_point_to(derived())); }
	template <class T, typename DT> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::circle        & b) const noexcept { return distance_min(b.closest_point_to(derived())); }

	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::point         & b) const noexcept { return derived() == b; }
	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::segment       & b) const noexcept { return derived() == b.a  && derived() == b.b; }
	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::aabb          & b) const noexcept { return derived() == b.ul() && derived() == b.ur() && derived() == b.dr() && derived() == b.dl(); }
	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::polygon       & b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != derived()) { return false; } } return true; }
	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::convex_polygon& b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != derived()) { return false; } } return true; }
	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::circle        & b) const noexcept { return (derived() == b.center) && (b.radius == 0); }
	template <class T, typename DT> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::capsule       & b) const noexcept { return false; } //TODO
	}