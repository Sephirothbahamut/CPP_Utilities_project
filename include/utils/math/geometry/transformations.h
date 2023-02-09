#pragma once

#include "../../concepts.h"

#include "../vec2.h"
#include "../angle.h"
#include "../transform2.h"
#include "shapes.h"

//TODO replace operators with named functions (translate, rotate, scale)
namespace utils::math::geometry::transformations
	{
#pragma region point
	// NOT to do, already defined in vec2.h //note, there's a space between to and do to not make it appear when searching for "TO[blank]DO"
	// Akshually since we're not using operators anymore...

	inline geometry::point scale(const geometry::point& point, float scaling) noexcept
		{
		return geometry::point{ point * scaling };
		}
	inline geometry::point rotate(const geometry::point& point, math::angle::radf rotation) noexcept
		{
		return point + rotation;
		}
	inline geometry::point translate(const geometry::point& point, vec2f translation) noexcept
		{
		return point + static_cast<geometry::point>(translation);
		}
#pragma endregion point

#pragma region segment
	inline geometry::segment scale(const geometry::segment& segment, float scaling) noexcept
		{
		return {segment.a * scaling, segment.b * scaling};
		}
	inline geometry::segment rotate(const geometry::segment& segment, math::angle::radf rotation) noexcept
		{
		return {segment.a + rotation, segment.b + rotation};
		}
	inline geometry::segment translate(const geometry::segment& segment, vec2f translation) noexcept
		{
		return {segment.a + translation, segment.b + translation};
		}
#pragma endregion segment



#pragma region polygon
	inline geometry::polygon scale(const geometry::polygon& polygon, float scaling) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v *= scaling; }
		return {std::move(new_vertices)};
		}
	
	inline geometry::polygon rotate(const geometry::polygon& polygon, math::angle::radf rotation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += rotation; }
		return {std::move(new_vertices)};
		}
	
	inline geometry::polygon translate(const geometry::polygon& polygon, vec2f translation) noexcept
		{
		std::vector<vec2f> new_vertices{polygon.get_vertices().begin(), polygon.get_vertices().end()};
		for (auto& v : new_vertices) { v += translation; }
		return {std::move(new_vertices)};
		}

#pragma endregion polygon

#pragma region convex_polygon
	inline geometry::convex_polygon scale(const geometry::convex_polygon& convex_polygon, float scaling) noexcept
		{
		std::vector<vec2f> new_vertices	{ convex_polygon.get_vertices().begin(), convex_polygon.get_vertices().end() };
		for (auto& v : new_vertices) 	{ v *= scaling; }
		return 	{ std::move(new_vertices) };
		}

	inline geometry::convex_polygon rotate(const geometry::convex_polygon& convex_polygon, math::angle::radf rotation) noexcept
		{
		std::vector<vec2f> new_vertices	{ convex_polygon.get_vertices().begin(), convex_polygon.get_vertices().end() };
		for (auto& v : new_vertices) 	{ v += rotation; }
		return 	{ std::move(new_vertices) };
		}

	inline geometry::convex_polygon translate(const geometry::convex_polygon& convex_polygon, vec2f translation) noexcept
		{
		std::vector<vec2f> new_vertices	{ convex_polygon.get_vertices().begin(), convex_polygon.get_vertices().end() };
		for (auto& v : new_vertices) 	{ v += translation; }
		return 	{ std::move(new_vertices) };
		}

#pragma endregion convex_polygon

#pragma region aabb
	inline geometry::aabb scale(const geometry::aabb& aabb, float scaling) noexcept
		{
		return 	{aabb.ll * scaling, aabb.up * scaling, aabb.rr * scaling, aabb.dw * scaling, };
		}

	inline geometry::aabb translate(const geometry::aabb& aabb, vec2f translation) noexcept
		{
		return 	{aabb.ll + translation.x, aabb.up + translation.y, aabb.rr + translation.x, aabb.dw + translation.y};
		}

	/*namespace _
		{
		class proxy_rotation
			{
			const geometry::aabb& aabb;
			angle::rad        rotation;

		public:
			proxy_rotation(const geometry::aabb& aabb, beta::math::angle::rad rotation) noexcept : aabb	{ aabb }, rotation	{ rotation }	{};

			operator geometry::aabb() 	{ return aabb; };
			//operator geometry::convex_polygon() 	{ return geometry::convex_polygon	{ aabb.ul, aabb.ur, aabb.dr, aabb.dl } + rotation; };
			};

		class proxy_transform
			{
			const geometry::aabb&  aabb;
			const transform2& transform;

		public:
			proxy_transform(const geometry::aabb& shape, const transform2& transform) noexcept : aabb	{ aabb }, transform	{ transform }	{};

			operator geometry::aabb()           	{ return static_cast<geometry::aabb>          ((aabb * transform.scaling) + transform.rotation) + transform.translation; };
			//operator geometry::convex_polygon() 	{ return static_cast<geometry::convex_polygon>((aabb * transform.scaling) + transform.rotation) + transform.translation; };
			};
		}
	
	inline auto rotate(const geometry::aabb& aabb, beta::math::angle::rad rotation) noexcept
		{
		return _::proxy_rotation	{ aabb, rotation };
		}*/

	inline geometry::aabb rotate(const geometry::aabb& aabb, math::angle::radf rotation) noexcept
		{
		return aabb;
		}

	/*inline auto scale (const geometry::aabb& shape, const transform2& transform) noexcept
		{
		return _::proxy_transform	{ shape, transform };
		}*/
#pragma endregion aabb

#pragma region circle
	inline geometry::circle scale(const geometry::circle& circle, float scaling) noexcept
		{
		return {circle.center * scaling, circle.radius * scaling};
		}
	inline geometry::circle rotate(const geometry::circle& circle, math::angle::radf rotation) noexcept
		{
		return {circle.center + rotation, circle.radius};
		}
	inline geometry::circle translate(const geometry::circle& circle, vec2f translation) noexcept
		{
		return {circle.center + translation, circle.radius};
		}
#pragma endregion circle


	///////////////////////////////////////////////////////////////////////// COMMON
	template <shape T>
	inline T  transform(const T& shape, const transform2& transform) noexcept { return translate(rotate(scale(shape, transform.scaling), transform.rotation), transform.translation); }
	template <shape T>
	inline T& transform_self(T& shape, const transform2& transform) noexcept { shape = transformations::transform(shape, transform); return shape; }

	template <shape_discrete T>
	inline T& scale_self(T& shape, float scaling) noexcept { shape = scale(shape, scaling); return shape; }
	template <shape_discrete T>
	inline T& rotate_self(T& shape, math::angle::radf rotation) noexcept { shape = rotate(shape, rotation); return shape; }
	template <shape_discrete T>
	inline T  translate_self(T& shape, vec2f translation) noexcept { shape = translate(shape, translation); return shape; }

#pragma region continuous shapes
	template <shape_continuous T>
	inline geometry::continuous_point scale(const T& continuous, float scaling, float scaling_next) noexcept
		{
		return T{scale(continuous.a, scaling), scale(continuous.b, scaling_next)};
		}
	template <shape_continuous T>
	inline geometry::continuous_point rotate(const T& continuous, math::angle::radf rotation, math::angle::radf rotation_next) noexcept
		{
		return T{rotate(continuous.a, rotation), rotate(continuous.b, rotation_next)};
		}
	template <shape_continuous T>
	inline geometry::continuous_point translate(const T& continuous, vec2f translation, vec2f translation_next) noexcept
		{
		return T{translate(continuous.a, translation), translate(continuous.b, translation_next)};
		}
	template <shape_continuous T>
	inline T& scale_self(T& shape, float scaling, float scaling_next) noexcept { shape = scale(shape, scaling, scaling_next); return shape; }
	template <shape_continuous T>
	inline T& rotate_self(T& shape, math::angle::radf rotation, math::angle::radf rotation_next) noexcept { shape = rotate(shape, rotation, rotation_next); return shape; }
	template <shape_continuous T>
	inline T  translate_self(T& shape, vec2f translation, vec2f translation_next) noexcept { shape = translate(shape, translation, translation_next); return shape; }
#pragma endregion continuous shapes
	}