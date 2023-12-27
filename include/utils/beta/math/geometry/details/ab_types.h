#pragma once
#include "../common.h"

namespace utils::math::geometry::shape::details
	{
	namespace ab_types
		{
		template <typename derived_t>
		struct interface : details::base<derived_t>
			{
			using crtp = details::base<derived_t>;

			utils_gpu_available constexpr const auto& a() const noexcept { return crtp::derived().a(); }
			utils_gpu_available constexpr const auto& b() const noexcept { return crtp::derived().b(); }
		
			utils_gpu_available constexpr float length2() const noexcept { return vec2f::distance2(a(), b()); }
			utils_gpu_available constexpr float length () const noexcept { return vec2f::distance (a(), b()); }
		
			utils_gpu_available constexpr const vec2f& operator[](const size_t& index) const noexcept { return index == 0 ? a() : b(); }
			utils_gpu_available constexpr       vec2f& operator[](const size_t& index)       noexcept { return index == 0 ? a() : b(); }
		
			/// <summary> Unit vector from a towards b. </summary>
			utils_gpu_available constexpr vec2f forward() const noexcept { return (b() - a()).normalize(); }
			/// <summary> Unit vector perpendicular on the left from a to b. </summary>
			utils_gpu_available constexpr vec2f perpendicular_right() const noexcept { return forward().perpendicular_right(); }
			/// <summary> Unit vector perpendicular on the right from a to b. </summary>
			utils_gpu_available constexpr vec2f perpendicular_left()  const noexcept { return forward().perpendicular_left (); }
			
			utils_gpu_available constexpr vec2f closest_vertex(const concepts::point auto& other) const noexcept { return other.distance(a()) < other.distance_min(b()) ? a() : b(); }

			utils_gpu_available constexpr float some_significant_name_ive_yet_to_figure_out(const concepts::point auto& other) const noexcept
				{
				//signed distance from line in proportion to the distance between a and b, idk, i'm not a math guy
				//enough alone to get the sign for side, but needs to be divided by (a-b).length to get the signed distance
				return ((other.x - a().x) * (b().y - a().y)) - ((b().x - a().x) * (other.y - a().y));
				}

			/// <summary> Projecting point to the line that goes through a-b, at what percentage of the segment a-b it lies. <0 is before a, >1 is after b, proportionally to the a-b distance </summary>
			utils_gpu_available constexpr float projected_percent(const concepts::point    auto& other) const noexcept
				{
				//from shadertoy version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//const utils::math::vec2f b_a{b()   - a()};
				//const utils::math::vec2f p_a{other - a()};
				//return utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a);

				//previous version, mathematically equivalent I think maybe perhaps, idk, i'm not into maths
				//http://csharphelper.com/blog/2016/09/find-the-shortest-distance-between-a-point-and-a-line-segment-in-c/
				const vec2f delta{b() - a()};
				return ((other.x - a().x) * delta.x + (other.y - a().y) * delta.y) / (delta.x * delta.x + delta.y * delta.y);
				}

			template <bool clamp_a, bool clamp_b>
			utils_gpu_available constexpr vec2f closest_point_custom_clamp(const concepts::point auto& other) noexcept
				{
				const vec2f delta{b() - a()};
				const float t{projected_percent(other)};
				if constexpr (clamp_a) { if (t < 0.f) { return a(); } }
				if constexpr (clamp_b) { if (t > 1.f) { return b(); } }
				return {a().x + t * delta.x, a().y + t * delta.y};
				}

			#pragma region point
				utils_gpu_available constexpr geometry::side       side_of        (const concepts::point auto& other) const noexcept { return {some_significant_name_ive_yet_to_figure_out(other)}; }

				utils_gpu_available constexpr bool                 contains       (const concepts::point auto& other) const noexcept { return distance(other) == 0; }
				utils_gpu_available constexpr bool                 intersects     (const concepts::point auto& other) const noexcept { return distance(other) == 0; }
				utils_gpu_available constexpr std::optional<vec2f> intersection   (const concepts::point auto& other) const noexcept { return intersects(other) ? other.vec : std::nullopt; }
			#pragma endregion point
			};

		struct data
			{
			data(const ::utils::math::vec2f& point_a, const ::utils::math::vec2f& point_b) : point_a{point_a}, point_b{point_b} {}
			::utils::math::vec2f point_a;
			::utils::math::vec2f point_b;
			utils_gpu_available constexpr const auto& a() const noexcept { return point_a; }
			utils_gpu_available constexpr const auto& b() const noexcept { return point_b; }
			};
		struct view
			{
			::utils::math::vecref2f point_a;
			::utils::math::vecref2f point_b;
			utils_gpu_available constexpr const auto& a() const noexcept { return point_a; }
			utils_gpu_available constexpr const auto& b() const noexcept { return point_b; }
			};
		}
	}

/*Shadertoy experimentation, modifying https://www.shadertoy.com/view/3tdSDj

// The MIT License
// Copyright © 2020 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// Signed distance to a line segment. Tutorial on how to derive
//
// the formula and code: https://www.youtube.com/watch?v=PMltMdi1Wzg

// List of some other 2D distances: https://www.shadertoy.com/playlist/MXdSRf
//
// and iquilezles.org/articles/distfunctions2d


float udSegment( in vec2 p, in vec2 a, in vec2 b )
{

	vec2 ba = b-a;
	vec2 pa = p-a;
	float h =clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
	return length(pa-h*ba);
}
float line( in vec2 p, in vec2 a, in vec2 b )
{
	return (((p.x - a.x) * (b.y - a.y)) - ((b.x - a.x) * (p.y - a.y))) / length(b - a);
	}

vec3 col_from_d(float d)
	{
	vec3 col = vec3(1.0) - sign(d)*vec3(0.1,0.4,0.7);
	col *= 1.0 - exp(-3.0*abs(d));
	col *= 0.8 + 0.2*cos(120.0*d);
	col = mix( col, vec3(1.0), 1.0-smoothstep(0.0,0.015,abs(d)) );
	return col;
	}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;
	vec2 m = (2.0*iMouse.xy-iResolution.xy)/iResolution.y;
	p *= 1.4;
	m *= 1.4;

	vec2 v1 = cos( iTime*0.5 + vec2(0.0,1.00) + 0.0 );
	vec2 v2 = cos( iTime*0.5 + vec2(0.0,3.00) + 1.5 );
	float th = 0.3*(0.5+0.5*cos(iTime*1.1+1.0));

	float d1 = udSegment( p, v1, v2 );// - th;
	float d2 = line( p, v1, v2 );// - th;

	vec3 col;
	//vec3 col = (col_from_d(d1) + col_from_d(d2)) / 2.;
	//vec3 col = col_from_d(d1 * sign(d2));
	//vec3 col = col_from_d(d2);

	if(p.x < .5)
		{
		col = col_from_d(d1 * sign(d2));
		}
	else
		{
		vec3 tmp = col_from_d(d2);
		col.x = tmp.y;
		col.y = tmp.z;
		col.z = tmp.x;
		}


	if( iMouse.z>0.001 )
	{
	d1 = udSegment(m, v1, v2) - th;
	col = mix(col, vec3(1.0,1.0,0.0), 1.0-smoothstep(0.0, 0.005, abs(length(p-m)-abs(d1))-0.0025));
	col = mix(col, vec3(1.0,1.0,0.0), 1.0-smoothstep(0.0, 0.005, length(p-m)-0.015));
	}

	fragColor = vec4(col,1.0);

	vec2 delta = (v1 - p);
	float distv1 = sqrt(delta.x * delta.x - delta.y * delta.y);
	if(abs(p.x - v1.x) < .1 && abs(p.y - v1.y) < .1)
		{
		fragColor = vec4(1., 0., 0., 0.);
		}
}













*/