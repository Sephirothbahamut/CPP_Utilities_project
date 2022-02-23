#pragma once

#include "../vec2.h"
#include "../angle.h"
#include "../Transform2.h"
#include "segment.h"
#include "polygon.h"
#include "circle.h"

namespace utils::math::geometry
	{
#pragma region point
	// NOT to do //note, there's a space between to and do to not make it appear when searching for "TO[blank]DO"
#pragma endregion point

#pragma region segment
	geometry::segment operator*(const geometry::segment& segment, float scale) noexcept
		{
		return {segment.a * scale, segment.b * scale};
		}
	geometry::segment operator+(const geometry::segment& segment, angle::deg rotation) noexcept
		{
		return {segment.a + rotation, segment.b + rotation};
		}
	geometry::segment operator+(const geometry::segment& segment, vec2f translation) noexcept
		{
		return {segment.a + translation, segment.b + translation};
		}
	geometry::segment& operator*=(geometry::segment& segment, float scale) noexcept
		{
		segment = segment * scale;
		return segment;
		}
	geometry::segment& operator+=(geometry::segment& segment, angle::deg rotation) noexcept
		{
		segment = segment + rotation;
		return segment;
		}
	geometry::segment& operator+=(geometry::segment& segment, vec2f translation) noexcept
		{
		segment = segment + translation;
		return segment;
		}
#pragma endregion segment

#pragma region polygon
	geometry::convex_polygon operator*(const geometry::convex_polygon& polygon, float scale) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v *= scale; }
		return {std::move(new_vertices)};
		}
	geometry::convex_polygon operator+(const geometry::convex_polygon& polygon, angle::deg rotation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += rotation; }
		return {std::move(new_vertices)};
		}
	geometry::convex_polygon operator+(const geometry::convex_polygon& polygon, vec2f translation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += translation; }
		return {std::move(new_vertices)};
		}
	geometry::convex_polygon& operator*=(geometry::convex_polygon& polygon, float scale) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v *= scale; }
		return polygon;
		}
	geometry::convex_polygon& operator+=(geometry::convex_polygon& polygon, angle::deg rotation) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v += rotation; }
		return polygon;
		}
	geometry::convex_polygon& operator+=(geometry::convex_polygon& polygon, vec2f translation) noexcept
		{
		for (auto& v : polygon.get_vertices()) { v += translation; }
		return polygon;
		}
#pragma endregion polygon
#pragma region circle
	geometry::circle operator*(const geometry::circle& circle, float scale) noexcept
		{
		return {circle.center * scale, circle.radius * scale};
		}
	geometry::circle operator+(const geometry::circle& circle, angle::deg rotation) noexcept
		{
		return {circle.center + rotation, circle.radius};
		}
	geometry::circle operator+(const geometry::circle& circle, vec2f translation) noexcept
		{
		return {circle.center + translation, circle.radius};
		}
	geometry::circle& operator*=(geometry::circle& circle, float scale) noexcept
		{
		circle = circle * scale;
		return circle;
		}
	geometry::circle& operator+=(geometry::circle& circle, angle::deg rotation) noexcept
		{
		circle = circle + rotation;
		return circle;
		}
	geometry::circle& operator+=(geometry::circle& circle, vec2f translation) noexcept
		{
		circle = circle + translation;
		return circle;
		}
#pragma endregion circle
	}