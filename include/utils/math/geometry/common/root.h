#pragma once

// Split from begin so it can be included in vec2 and rect that are defined outside of geometry.
// This allows for them to inherit from the base shapes

#include <memory>
#include <utility>
#include <optional>

#include "../../../compilation/CUDA.h"

namespace utils::math
	{
	template <typename T, size_t size>
	class vec;
	template <typename T>
	struct rect;
	}

namespace utils::math::geometry
	{
	template <bool view = false> using point = utils::math::vec <std::conditional_t<view, std::reference_wrapper<float>, float>, 2>;
	template <bool view = false> using aabb  = utils::math::rect<std::conditional_t<view, std::reference_wrapper<float>, float>>;
	template <bool view = false> class segment       ;
	template <bool view = false> class polygon       ;
	template <bool view = false> class convex_polygon;
	template <bool view = false> class circle        ;
	template <bool view = false> class capsule       ;

	struct closest_point_and_distance_t { point<false> position; float distance; };

	template <typename DERIVED_T>
	struct shape_base;

	namespace concepts 
		{
		template <typename T>
		concept shape = std::derived_from<T, shape_base<T>>;
		}

	template <typename DERIVED_T>
	struct shape_base
		{
		private:
			using derived_t = DERIVED_T;
			utils_cuda_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_cuda_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
#pragma region interactions
			template <concepts::shape other_t>
			closest_point_and_distance_t closest_point_and_distance(const other_t& b) const noexcept { return {derived().closest_point_to(b), derived().distance_min(b)}; };

			template <concepts::shape other_t>
			point<false> closest_point_to(const other_t& b) const noexcept { return derived().closest_point_and_distance(b).position; };

			template <concepts::shape other_t>
			float distance_min(const other_t& b) const noexcept { return derived().closest_point_and_distance(b).distance; }

			template <concepts::shape other_t>
			vec<float, 2> vector_to(const other_t& b) const noexcept { return b.closest_point_to(derived()) - derived().closest_point_to(b); }

			template <concepts::shape other_t>
			std::optional<point<false>> intersects(const other_t& b) const noexcept { return derived().distance_min(b) == 0; };

			template <concepts::shape other_t>
			std::optional<point<false>> intersection(const other_t& b) const noexcept { return derived().intersects(b) ? std::optional<point>{derived().closest_point_to(b)} : std::nullopt; };

			template <concepts::shape other_t>
			bool contains(const other_t& b) const noexcept { return false; };

			template <concepts::shape other_t>
			std::optional<point<false>> collision(const other_t& b) const noexcept
				{
				/*if constexpr (!  hollow)*/ { if (derived().contains(b)) { return true; } }
				/*if constexpr (!b.hollow)*/ { if (b.contains(derived())) { return true; } }
				return derived().intersection(b).has_value();
				};
#pragma endregion interactions
			//aabb<false> bounding_box() const noexcept = 0;
		};
	
	template <typename DERIVED_T>
	struct shape_with_vertices_base : shape_base<DERIVED_T>
		{
		private:
			using derived_t = DERIVED_T;
			utils_cuda_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
			utils_cuda_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }

		public:
#pragma region views
			segment<true > get_edges()       noexcept;
			segment<false> get_edges() const noexcept;
#pragma endregion views
		};
	}