#pragma once
#include "../shapes.h"

namespace utils::math::details
	{
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::point         <view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::segment       <view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::aabb          <view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::polygon       <view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::convex_polygon<view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::circle        <view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> vec2f vec_sized_specialization<T, 2, DT>::closest_point_to(const geometry::capsule       <view_b>& b) const noexcept { return *this; }
	template <class T, typename DT> template <bool view_b> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::point         <view_b>& b) const noexcept { return vec2f::distance(vec2f{*this}, vec2f{b}); }
	template <class T, typename DT> template <bool view_b> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::segment       <view_b>& b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	template <class T, typename DT> template <bool view_b> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::aabb          <view_b>& b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	template <class T, typename DT> template <bool view_b> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::polygon       <view_b>& b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	template <class T, typename DT> template <bool view_b> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::convex_polygon<view_b>& b) const noexcept { return distance_min(b.closest_point_to(*this)); }
	template <class T, typename DT> template <bool view_b> float vec_sized_specialization<T, 2, DT>::distance_min    (const geometry::circle        <view_b>& b) const noexcept { return distance_min(b.closest_point_to(*this)); }

	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::point         <view_b>& b) const noexcept { return *this == b; }
	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::segment       <view_b>& b) const noexcept { return *this == b.a  && *this == b.b; }
	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::aabb          <view_b>& b) const noexcept { return *this == b.ul() && *this == b.ur() && *this == b.dr() && *this == b.dl(); }
	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::polygon       <view_b>& b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != *this) { return false; } } return true; }
	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::convex_polygon<view_b>& b) const noexcept { for (const auto& vertex : b.get_vertices()) { if (vertex != *this) { return false; } } return true; }
	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::circle        <view_b>& b) const noexcept { return (*this == b.center) && (b.radius == 0); }
	template <class T, typename DT> template <bool view_b> bool vec_sized_specialization<T, 2, DT>::contains(const geometry::capsule       <view_b>& b) const noexcept { return false; } //TODO
	}