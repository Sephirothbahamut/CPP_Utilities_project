#include "../details/base_types.h"
#include "point.h"

// References:
// https://github.com/oysteinmyrmo/bezier
// Leander's answer to https://stackoverflow.com/questions/2742610/closest-point-on-a-cubic-bezier-curve






/* Copyright © 2023 Øystein Myrmo (oystein.myrmo@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

namespace utils::math::geometry::shape::details::bezier_data
	{
	template <size_t EXTENT>
	struct owner : std::array<vec2f, EXTENT>
		{
		static constexpr std::size_t extent{EXTENT};
		//using std::array<vec2f, EXTENT>::array;
		
		template <std::same_as<utils::math::vec2f> ...Args>
		utils_gpu_available owner(Args&&... control_points) : std::array<vec2f, EXTENT>{std::forward<Args>(control_points)...} {}
		};

	template <>
	struct owner<std::numeric_limits<std::size_t>::max()> : std::vector<vec2f> 
		{
		static constexpr std::size_t extent{std::numeric_limits<std::size_t>::max()};

		using std::vector<vec2f>::vector;
		};
	}

namespace utils::math::geometry::shape::details
	{
	template <typename derived_T>
	struct bezier : utils::math::geometry::shape::details::base<derived_T, shapes_enum::bezier>
		{
		protected:
			using derived_t = derived_T;
			using self_t = bezier<derived_t>;
			utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
			utils_gpu_available size_t order() const noexcept { return derived().control_points.size() - 1; }

			class value_at_proxy
				{
				public:
					utils_gpu_available constexpr vec2f point() const noexcept
						{
						//TODO
						}
					utils_gpu_available constexpr vec2f normal() const noexcept
						{
						//TODO
						}
					utils_gpu_available constexpr vec2f tangent() const noexcept
						{
						//TODO
						}

					utils_gpu_available constexpr operator vec2f() const noexcept { return point(); }

				private:
					utils_gpu_available constexpr value_at_proxy(const self_t& bezier_curve, const float t) : bezier_curve{bezier_curve}, t{t} {}
					const self_t& bezier_curve;
					const float t;
				};

			utils_gpu_available constexpr const value_at_proxy operator[](float t) const noexcept
				{
				return {*this, t};
				}
		};
	}

namespace utils::math::geometry::shape
	{
	template <size_t points_count = 0>
	struct bezier : utils::math::geometry::shape::details::bezier<bezier<points_count>>
		{
		utils_gpu_available bezier(std::initializer_list<utils::math::vec2f> control_points) requires(points_count == 0) : control_points{control_points} {}
		template <std::same_as<utils::math::vec2f> ...Args>
		utils_gpu_available bezier(Args&&... control_points) requires(points_count != 0) : control_points{std::forward<Args>(control_points)...} {}
		utils::math::geometry::shape::details::bezier_data::owner<(points_count > 0 ? points_count : std::dynamic_extent)> control_points;
		};

	namespace view
		{
		template <bool const_control_points, size_t points_count = 0>
		struct bezier : utils::math::geometry::shape::details::bezier<bezier<const_control_points, points_count>>
			{
			using vertex_t = std::conditional_t<const_control_points, const vec2f, vec2f>;
			using span_t   = std::span<vertex_t, (points_count > 0 ? points_count : std::dynamic_extent)>;
			utils_gpu_available bezier(span_t control_points) : control_points(control_points) {}
			span_t control_points;
			};
		}
	}