#pragma once

#include "../vec2.h"
#include "../angle.h"
#include "../Transform2.h"
#include "segment.h"
#include "polygon.h"
#include "circle.h"

namespace utils::math::geometry::transformations
	{
#pragma region point
	// NOT to do //note, there's a space between to and do to not make it appear when searching for "TO[blank]DO"
#pragma endregion point

#pragma region segment
	inline geometry::segment operator*(const geometry::segment& segment, float scale) noexcept
		{
		return {segment.a * scale, segment.b * scale};
		}
	inline geometry::segment operator+(const geometry::segment& segment, angle::deg rotation) noexcept
		{
		return {segment.a + rotation, segment.b + rotation};
		}
	inline geometry::segment operator+(const geometry::segment& segment, vec2f translation) noexcept
		{
		return {segment.a + translation, segment.b + translation};
		}
	inline geometry::segment& operator*=(geometry::segment& segment, float scale) noexcept
		{
		segment = segment * scale;
		return segment;
		}
	inline geometry::segment& operator+=(geometry::segment& segment, angle::deg rotation) noexcept
		{
		segment = segment + rotation;
		return segment;
		}
	inline geometry::segment& operator+=(geometry::segment& segment, vec2f translation) noexcept
		{
		segment = segment + translation;
		return segment;
		}

	inline geometry::segment  operator* (const geometry::segment& shape, const Transform2& transform) noexcept { return         ((shape * transform.size) + transform.orientation) + transform.position; }
	inline geometry::segment& operator*=(      geometry::segment& shape, const Transform2& transform) noexcept { return shape = ((shape * transform.size) + transform.orientation) + transform.position; }
#pragma endregion segment

#pragma region polygon
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type> operator*(const geometry::polygon<polygon_type>& polygon, float scale) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v *= scale; }
		return {std::move(new_vertices)};
		}
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type> operator+(const geometry::polygon<polygon_type>& polygon, angle::deg rotation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += rotation; }
		return {std::move(new_vertices)};
		}
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type> operator+(const geometry::polygon<polygon_type>& polygon, vec2f translation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += translation; }
		return {std::move(new_vertices)};
		}
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type>& operator*=(geometry::polygon<polygon_type>& polygon, float scale) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v *= scale; }
		return polygon;
		}
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type>& operator+=(geometry::polygon<polygon_type>& polygon, angle::deg rotation) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v += rotation; }
		return polygon;
		}
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type>& operator+=(geometry::polygon<polygon_type>& polygon, vec2f translation) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v += translation; }
		return polygon;
		}
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type>  operator* (const geometry::polygon<polygon_type>& shape, const Transform2& transform) noexcept { return         ((shape * transform.size) + transform.orientation) + transform.position; }
	template <polygon_type_t polygon_type>
	inline geometry::polygon<polygon_type>& operator*=(      geometry::polygon<polygon_type>& shape, const Transform2& transform) noexcept { return shape = ((shape * transform.size) + transform.orientation) + transform.position; }

#pragma endregion polygon
#pragma region circle
	inline geometry::circle operator*(const geometry::circle& circle, float scale) noexcept
		{
		return {circle.center * scale, circle.radius * scale};
		}
	inline geometry::circle operator+(const geometry::circle& circle, angle::deg rotation) noexcept
		{
		return {circle.center + rotation, circle.radius};
		}
	inline geometry::circle operator+(const geometry::circle& circle, vec2f translation) noexcept
		{
		return {circle.center + translation, circle.radius};
		}
	inline geometry::circle& operator*=(geometry::circle& circle, float scale) noexcept
		{
		circle = circle * scale;
		return circle;
		}
	inline geometry::circle& operator+=(geometry::circle& circle, angle::deg rotation) noexcept
		{
		circle = circle + rotation;
		return circle;
		}
	inline geometry::circle& operator+=(geometry::circle& circle, vec2f translation) noexcept
		{
		circle = circle + translation;
		return circle;
		}

	inline geometry::circle  operator* (const geometry::circle& shape, const Transform2& transform) noexcept { return         ((shape * transform.size) + transform.orientation) + transform.position; }
	inline geometry::circle& operator*=(      geometry::circle& shape, const Transform2& transform) noexcept { return shape = ((shape * transform.size) + transform.orientation) + transform.position; }
#pragma endregion circle
	}