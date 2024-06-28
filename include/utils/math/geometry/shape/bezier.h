#pragma once

#include <span>
#include <array>
#include <vector>
#include <optional>

#include "../details/base_types.h"
#include "ab.h"
#include "point.h"

namespace utils::math::geometry::shape
	{
	namespace generic
		{
		template <storage::type storage_type, size_t extent = std::dynamic_extent>
		struct bezier : utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, extent, true>, utils::math::geometry::shape_flag
			{
			using storage_t = utils::storage::multiple<storage::storage_type_for<geometry::shape::point, storage_type>, extent, true>;
			using storage_t::size;
			using storage_t::multiple;
			using storage_t::operator[];
			using typename storage_t::value_type;
			using typename storage_t::const_aware_value_type;

			using self_t = bezier<storage_type, extent>;
			using nonref_self_t = bezier<storage::type::create::owner(), extent>;

			struct at_proxy
				{
				template <storage::type storage_type, size_t extent>
				friend struct bezier;
				public:
					utils_gpu_available constexpr vec2f point() const noexcept
						{
						if (bezier_curve.size() == size_t{3})
							{
							float inverse_t{1.f - t};
							return bezier_curve[0] * inverse_t * inverse_t + bezier_curve[1] * 2.f * t * inverse_t + bezier_curve[2] * t * t;
							}
						}
					utils_gpu_available constexpr vec2f tangent() const noexcept
						{
						if (bezier_curve.size() == size_t{3})
							{
							//return 2.f * (bezier_curve[0] * (t - 1.f) + (1.f - 2.f * t) * bezier_curve[1] + bezier_curve[2] * t);
							return ((bezier_curve[0] * (t - 1.f)) + (bezier_curve[1] * (1.f - 2.f * t)) + bezier_curve[2] * t) * 2.f;
							}
						}
					utils_gpu_available constexpr vec2f normal() const noexcept
						{
						return tangent().perpendicular_left();
						}

					utils_gpu_available constexpr operator vec2f() const noexcept { return point(); }

				private:
					utils_gpu_available constexpr at_proxy(const self_t& bezier_curve, const float t) : bezier_curve{bezier_curve}, t{t} {}
					const self_t& bezier_curve;
					const float t;
				};

			utils_gpu_available constexpr const at_proxy at(float t) const noexcept
				{
				return {*this, t};
				}
			utils_gpu_available constexpr const at_proxy at_equidistant(float t) const noexcept
				{
				return {*this, t_to_equidistant_t(t)};
				}

			utils_gpu_available constexpr float t_to_equidistant_t(float t) const noexcept
				{
				//TODO
				return t;
				}

			utils_gpu_available constexpr float length(float t_min = 0.f, float t_max = 1.f) const noexcept
				{
				if (size() == 3)
					{
					const utils::math::vec2f b_to_a{operator[](1) - operator[](0)};
					const utils::math::vec2f c_to_b{operator[](2) - operator[](1)};
					const utils::math::vec2f cb_to_ba = c_to_b - b_to_a;

					const float a{utils::math::vec2f::dot(cb_to_ba, cb_to_ba)};
					const float b{utils::math::vec2f::dot(b_to_a  , cb_to_ba)};
					const float c{utils::math::vec2f::dot(b_to_a  , b_to_a  )};

					utils::math::vec_f<4> pre{a * c - b * b, b / a, 2.f * b, 0.f};
					pre.w() = std::sqrt(pre.x());

					const float value_at_min{a * t_min};
					const float value_at_max{a * t_max};

					const float q1{(t_max + pre.y()) * std::sqrt(c + pre.z() * t_max + value_at_max * t_max) - (t_min + pre.y()) * std::sqrt(c + pre.z() * t_min + value_at_min * t_min)};
					const float q2{pre.x() / std::pow(a, 1.5) * (std::asinh((value_at_max + b) / pre.w()) - std::asinh((value_at_min + b) / pre.w()))};

					return q1 + q2;
					}
				}

			template <bool equidistant>
			struct edges_view : std::ranges::view_interface<edges_view<equidistant>>
				{
				using edge = utils::math::geometry::shape::segment;

				struct iterator : std::contiguous_iterator_tag
					{
					//TODO random access iterator implementation https://en.cppreference.com/w/cpp/iterator/random_access_iterator
					using difference_type   = std::ptrdiff_t;
					using value_type        = edge;
					using pointer           = value_type*;
					using reference         = value_type&;

					utils_gpu_available constexpr iterator() noexcept = default;
					utils_gpu_available constexpr iterator(const edges_view* edges_view_ptr, size_t index = 0) noexcept : edges_view_ptr{edges_view_ptr}, index{index} {}

					utils_gpu_available constexpr edge operator*() const noexcept { return edges_view_ptr->operator[](index); }
					utils_gpu_available constexpr edge operator*()       noexcept { return edges_view_ptr->operator[](index); }
				
					utils_gpu_available constexpr iterator& operator++(   ) noexcept { index++; return *this; }
					utils_gpu_available constexpr iterator& operator--(   ) noexcept { index--; return *this; }
					utils_gpu_available constexpr iterator  operator++(int) noexcept { iterator ret{*this}; ++(*this); return ret; }
					utils_gpu_available constexpr iterator  operator--(int) noexcept { iterator ret{*this}; --(*this); return ret; }
				
					utils_gpu_available friend constexpr bool operator== (const iterator& a, const iterator& b) noexcept { return a.index == b.index && a.edges_view_ptr == b.edges_view_ptr; };
					utils_gpu_available friend constexpr auto operator<=>(const iterator& a, const iterator& b) noexcept { return a.index <=> b.index; }

					const edges_view* edges_view_ptr;
					size_t index{0};
					};
				
				//static_assert(std::bidirectional_iterator<iterator<true>>); //TODO check why non copy constructible if storage inner container is span?
				//static_assert(std::random_access_iterator<iterator>);
				//static_assert(std::condiguous_iterator   <iterator>);
			
				const self_t* bezier_curve_ptr;
				size_t subdivisions{1};
				
				utils_gpu_available const float index_to_t(const size_t& index) const noexcept
					{
					const float t{static_cast<float>(index) / static_cast<float>(subdivisions)};
					if constexpr (equidistant) 
						{
						return bezier_curve_ptr->t_to_equidistant_t(t);
						}
					return t;
					}
					
				utils_gpu_available edge operator[](const size_t& index) const noexcept                          
					{
					const float t_a{index_to_t(index)};
					const float t_b{index_to_t(index + 1)};
					return edge{bezier_curve_ptr->at(t_a).point(), bezier_curve_ptr->at(t_b).point()};
					}

				utils_gpu_available constexpr auto begin() const noexcept { return iterator{this, 0     }; }
				utils_gpu_available constexpr auto begin()       noexcept { return iterator{this, 0     }; }
				utils_gpu_available constexpr auto end  () const noexcept { return iterator{this, size()}; }
				utils_gpu_available constexpr auto end  ()       noexcept { return iterator{this, size()}; }
			
				utils_gpu_available constexpr bool   empty() const noexcept { return bezier_curve_ptr->size() <= 1; }
				utils_gpu_available constexpr size_t size () const noexcept
					{
					return subdivisions;
					}

				utils_gpu_available constexpr edges_view(const self_t& bezier_curve, size_t subdivisions = 1) : bezier_curve_ptr{&bezier_curve}, subdivisions{subdivisions} {}
				};
			
			/// <summary> 
			/// Usage note: This shape contains vertices, not edges. 
			/// The operator[] and dereferencing an iterator in the edges view will not return a reference to an edge, it will return an actual edge object which itself contains references to the vertices.
			/// So don't write `for(auto& edge : x.get_edges())`, write `for(auto edge : x.get_edges())` instead.
			/// If this shape is const or the edges view is stored in a const variable, edges will be const observers.
			/// Otherwise they will be simple observers, and changing them will affect the vertices of this shape.
			/// A lot of tears and blood were poured into making this seemingly seamless, it's part of the reason I restarted the geometry portion of this library from scratch at least 3 times, 
			/// please appreciate my efforts for such an useless feature nobody will ever need :)
			/// </summary>
			utils_gpu_available constexpr auto get_edges            (size_t divisions) const noexcept { return edges_view<false>{*this, divisions}; }
			utils_gpu_available constexpr auto get_edges_equidistant(size_t divisions) const noexcept { return edges_view<true >{*this, divisions}; }

			};
		}

	
	namespace concepts
		{
		template <typename T> concept bezier = std::derived_from<T, shape::generic::bezier<T::storage_type, T::extent>>;
		}
	
	namespace owner 
		{
		template <size_t extent = std::dynamic_extent>
		using bezier = shape::generic::bezier<storage::type::create::owner(), extent>;
		}
	namespace observer
		{
		template <size_t extent = std::dynamic_extent>
		using bezier = shape::generic::bezier<storage::type::create::observer(), extent>;
		}
	namespace const_observer
		{
		template <size_t extent = std::dynamic_extent>
		using bezier = shape::generic::bezier<storage::type::create::const_observer(), extent>;
		}
	}
