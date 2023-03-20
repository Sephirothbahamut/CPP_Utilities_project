#pragma once

#include <optional>

#include "../vec2.h"
#include "../rect.h"

namespace utils::math::geometry
	{
	class point         ;
	class segment       ;
	class aabb          ;
	class polygon       ;
	class convex_polygon;
	class circle        ;
	class capsule       ;

	struct closest_point_and_distance_t { vec2f point; float distance; };

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
			template <concepts::shape other_t>
			closest_point_and_distance_t closest_point_and_distance(const other_t& b) const noexcept { return {closest_point_to(b), distance_min(b)}; };

			template <concepts::shape other_t>
			vec2f closest_point_to(const other_t& b) const noexcept { return closest_point_and_distance().point; };

			template <concepts::shape other_t>
			float distance_min(const other_t& b) const noexcept { return closest_point_and_distance().distance; }

			template <concepts::shape other_t>
			vec2f vector_to(const other_t& b) const noexcept { return b.closest_point_to(derived()) - derived().closest_point_to(b); }

			template <concepts::shape other_t>
			std::optional<vec2f> intersects(const other_t& b) const noexcept { return distance_min(b) == 0; };

			template <concepts::shape other_t>
			std::optional<vec2f> intersection(const other_t& b) const noexcept { return intersects(b) ? closest_point_to(b) : std::nullopt; };

			template <concepts::shape other_t>
			bool contains(const other_t& b) const noexcept { return false; };

			template <concepts::shape other_t>
			std::optional<vec2f> collision(const other_t& b) const noexcept 
				{
				/*if constexpr (!  hollow)*/ { if (  contains(b        )) { return true; } }
				/*if constexpr (!b.hollow)*/ { if (b.contains(derived())) { return true; } }
				return intersection(b).has_value();
				};

			//aabb<false> bounding_box() const noexcept = 0;
		};
	}

#include "aabb.h"