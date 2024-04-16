#pragma once

// Split from begin so it can be included in vec2 and rect that are defined outside of geometry.
// This allows for them to inherit from the base shapes
#include <memory>
#include <utility>
#include <optional>

#include "../../angle.h"
#include "../../../compilation/gpu.h"

namespace utils::math
	{
	template <typename T, size_t size>
	class vec;
	using vec2f = vec<float, 2>;

	template <typename T>
	struct rect;

	class transform2;
	}

namespace utils::math::geometry
	{
	using  point = utils::math::vec2f;
	using  aabb  = utils::math::rect<float>;
	struct segment       ;
	class  polygon       ;
	class  convex_polygon;
	struct circle        ;
	class  capsule       ;

	struct edge_ref;

	struct closest_point_and_distance_t;

	namespace details
		{
		template <typename DERIVED_T>
		struct shape_base_inner
			{
			protected:
				using derived_t = DERIVED_T;
				utils_gpu_available constexpr const derived_t& derived() const noexcept { return static_cast<const derived_t&>(*this); }
				utils_gpu_available constexpr       derived_t& derived()       noexcept { return static_cast<      derived_t&>(*this); }
			};
		}

	namespace concepts 
		{
		template <typename T>
		concept shape = std::derived_from<T, details::shape_base_inner<T>>;
		}

	template <typename DERIVED_T>
	struct shape_base : details::shape_base_inner<DERIVED_T>
		{
		private:
			using derived_t = details::shape_base_inner<DERIVED_T>::derived_t;
			using details::shape_base_inner<DERIVED_T>::derived;

		public:
#pragma region interactions
			template <concepts::shape other_t>
			closest_point_and_distance_t closest_point_and_distance(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			point closest_point_to(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			float distance_min(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			point vector_to(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			std::optional<point> intersects(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			std::optional<point> intersection_with(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			bool contains(const other_t& b) const noexcept;

			template <concepts::shape other_t>
			bool collides_with(const other_t& b) const noexcept;

#pragma endregion interactions
#pragma region transformations
			DERIVED_T  scale         (const float      & scaling    ) const noexcept;
			DERIVED_T  rotate        (const angle::radf& rotation   ) const noexcept;
			DERIVED_T  translate     (const vec2f      & translation) const noexcept;
			DERIVED_T  transform     (const transform2 & transform  ) const noexcept;
			DERIVED_T& scale_self    (const float      & scaling    )       noexcept;
			DERIVED_T& rotate_self   (const angle::radf& rotation   )       noexcept;
			DERIVED_T& translate_self(const vec2f      & translation)       noexcept;
			DERIVED_T& transform_self(const transform2 & transform  )       noexcept;
#pragma endregion transformations

			//aabb bounding_box() const noexcept = 0;
		};
	}