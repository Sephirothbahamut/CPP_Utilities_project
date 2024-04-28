#pragma once

#include "interface/all.h"
#include "../../../concepts.h"

namespace utils::math::geometry::interactions
	{
	namespace details
		{
		template <auto function>
		utils_gpu_available constexpr auto inverted(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return (b.*function)(a); }

		utils_gpu_available constexpr float distance_from_distance_signed(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept 
			{
			return distance_signed(a, b).absolute(); 
			}
		utils_gpu_available constexpr float distance_signed_from_distance_and_side(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept 
			{
			return {distance(a, b) * side_of(a, b)};
			}

		//utils_gpu_available constexpr side                           side_from_distance_signed                      (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.distance(b).side(); }
		//utils_gpu_available constexpr float                          distance_from_distance_signed                  (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.distance(b).distance(); }
		//utils_gpu_available constexpr geometry::signed_distance_t    distance_signed_from_side_and_distance         (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return {.value{a.distance(b) * a.side_of(b)}}; }
		//utils_gpu_available constexpr vec2f                          closest_from_closest_with_distance             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.closest_with_distance(b).closest ; }
		//utils_gpu_available constexpr float                          distance_from_closest_with_distance            (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.closest_with_distance(b).distance; }
		//utils_gpu_available constexpr closest_point_with_distance_t  closest_with_distance_from_closest_and_distance(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return {a.closest_point_to(b), a.distance(b)}; }
		//utils_gpu_available constexpr closest_pair_with_distance_t   closest_pair_from_closest_and_other_closest    (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return {a.closest_point_to(b), b.closest_point_to(a), a.distance(b)}; }
		//utils_gpu_available constexpr vec2f                          vector_to_from_closest_pair                    (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return b.closest_point_to(a) - a.closest_point_to(b); }
		//utils_gpu_available constexpr bool                           intersects_from_intersection                   (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.intersection_with(b).has_value(); }
		//utils_gpu_available constexpr std::optional<vec2f>           intersection_from_intersects_and_closest       (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.intersects(b) ? std::optional<vec2f>{a.closest_point_to(b)} : std::nullopt; }
		//utils_gpu_available constexpr bool                           contains_from_side                             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return b.distance(a).side().is_inside(); }
		//utils_gpu_available constexpr bool                           collides_with_from_intersects_and_contains     (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return a.intersects(b) || a.contains(b); }
		//
		////some operations (closest point to) cannot be inverted for obvious reasons
		//utils_gpu_available constexpr geometry::signed_distance_t  other_distance             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return -b.distance            (a); }
		//utils_gpu_available constexpr closest_pair_with_distance_t other_closest_pair_inverted(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return -b.closest_pair        (a); }
		//utils_gpu_available constexpr vec2f                        other_vector_to_inverted   (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return -b.vector_to           (a); }
		//utils_gpu_available constexpr bool                         other_intersects           (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return  b.intersects          (a); }
		//utils_gpu_available constexpr std::optional<vec2f>         other_intersection         (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return  b.intersection_with   (a); }
		//utils_gpu_available constexpr bool                         other_collides_with        (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { return  b.collides_with       (a); }
		}

	namespace point
		{
		namespace common
			{
			utils_gpu_available constexpr bool contains(const shape::concepts::point auto& a, const shape::concepts::any auto& b) noexcept
				{
				return false;
				}

			utils_gpu_available constexpr std::optional<vec2f> intersection_with(const shape::concepts::point auto& a, const shape::concepts::any auto& b) noexcept
				{
				return intersects(a, b) ? std::optional<vec2f>{a} : std::nullopt;
				}
			}

		namespace point
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return a == b ? geometry::side::create::coincident() : geometry::side::create::outside();
				}
			utils_gpu_available constexpr float distance(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return utils::math::vec2f::distance(a, b);
				}
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {distance(a, b)};
				}

			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return b;
				}

			utils_gpu_available constexpr bool intersects(const shape::concepts::point auto& a, const shape::concepts::point auto& b) noexcept
				{
				return a == b;
				}
			}

		namespace circle
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return {distance_signed(a, b)};
				}

			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return {interactions::point::point::distance(a, b.center_point()) - b.radius};
				}
			utils_gpu_available constexpr geometry::signed_distance_t distance(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return distance_signed(a, b).absolute();
				}

			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				const shape::segment b_center_to_a{b.center, a};
				return b.center + (b_center_to_a.forward() * b.radius);
				}

			utils_gpu_available constexpr bool intersects(const shape::concepts::point auto& a, const shape::concepts::circle auto& b) noexcept
				{
				return distance(a, b).is_coincident();
				}

			}

		namespace ab
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept 
				{ //TODO check left/right sign
				return {b.some_significant_name_ive_yet_to_figure_out(a)}; 
				}

			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{
				constexpr ends_t ends{std::remove_cvref_t<decltype(b)>::static_ends};
				return b.closest_point_to_custom_clamp<ends.is_a_finite(), ends.is_b_finite()>(a);
				}

			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{ //TODO check left/right sign
				const float tmp{b.some_significant_name_ive_yet_to_figure_out(a)};
				const float length{shape::segment{b.a, b.b}.length()}; //needs finite length

				constexpr ends_t ends{std::remove_cvref_t<decltype(b)>::static_ends};
				if constexpr (!ends.is_infinite())
					{
					const float projected_percent{b.projected_percent(a)};
					if constexpr (ends.is_a_finite())
						{
						if (projected_percent < 0.f) { return {utils::math::vec2f::distance(a, b.a) * geometry::side{tmp}}; }
						}
					if constexpr (ends.is_b_finite())
						{
						if (projected_percent > 1.f) { return {utils::math::vec2f::distance(a, b.b) * geometry::side{tmp}}; }
						}
					}

				return {tmp / length}; 
				}

			utils_gpu_available constexpr float distance(const shape::concepts::point auto& a, const shape::concepts::ab auto& b) noexcept
				{
				constexpr ends_t ends{std::remove_cvref_t<decltype(b)>::static_ends};

				const utils::math::vec2f b_a{b.b - b.a};
				const utils::math::vec2f p_a{a   - b.a};
				float h{utils::math::vec2f::dot(p_a, b_a) / utils::math::vec2f::dot(b_a, b_a)};
				if constexpr (ends.is_a_finite()) { h = utils::math::max(h, 0.f); }
				if constexpr (ends.is_b_finite()) { h = utils::math::min(h, 1.f); }

				return (p_a - (b_a * h)).length;
				}
			}

		namespace polyline
			{
			utils_gpu_available constexpr shape::segment closest_segment_of(const shape::concepts::point auto& a, const shape::concepts::polyline auto& b) noexcept
				{
				const auto edges{b.get_edges()};

				float found_distance{utils::math::constants::finf};
				shape::segment found;

				for (const auto& edge : b.get_edges())
					{
					const float candidate_distance{distance(a, edge)};
					if (found_distance < candidate_distance)
						{
						candidate_distance = found_distance;
						found = edge;
						}
					}

				return found;
				}
			
			utils_gpu_available constexpr vec2f closest_point_of(const shape::concepts::point auto& a, const shape::concepts::polyline auto& b) noexcept
				{
				const auto edges{b.get_edges()};

				float found_distance{utils::math::constants::finf};
				vec2f found;

				for (const auto& edge : b.get_edges())
					{
					const vec2f candidate{closest_point_of(a, edge)};
					const float candidate_distance{distance(a, edge)};
					if (candidate_distance < found_distance)
						{
						found_distance = candidate_distance;
						found = candidate;
						}
					}

				return found;
				}

			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::polyline auto& b) noexcept
				{
				return distance_signed(a, closest_segment_of(a, b));
				}

			utils_gpu_available constexpr float distance(const shape::concepts::point auto& a, const shape::concepts::polyline auto& b) noexcept
				{
				return distance(a, closest_segment_of(a, b));
				}
			}

		namespace polygon
			{
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::polyline auto& b) noexcept
				{
				//From inigo quilez's example https://www.shadertoy.com/view/wdBXRW
				//Sidenote: why are graphics programmers scared of using longer more explanatory variable names? Why d and s when you can type distance and sign?
				const auto& vertices{b.vertices};

				if (b.vertices.empty()) { return geometry::signed_distance_t{utils::math::constants::finf}; }
				const auto something_tmp_idk_how_to_call{a - vertices[0]};
				float found_distance{utils::math::vec2f::dot(something_tmp_idk_how_to_call, something_tmp_idk_how_to_call)};
				const float sign{1.0f};

				for (const auto& edge : b.get_edges())
					{
					found_distance = std::min(found_distance, distance(a, edge));

					const auto edge_a_to_point{a - edge.a};

					const short conditions
						{
						(a.y >= edge.a.y) +
						(a.y <  edge.b.y) +
						((edge.a_to_b().x * edge_a_to_point.y) > (edge.a_to_b().y * edge_a_to_point.x))
						};

					if (conditions == 0 || conditions == 3) { sign = -sign; }
					}

				return {found_distance * sign};
				}
			}

		namespace aabb
			{
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::point auto& a, const shape::concepts::aabb auto& b) noexcept
				{
				const auto point_from_center_ur_quadrant{utils::math::abs(a - b.get_center())};
				const auto corner_from_center{b.ur() - b.get_center()};
				const auto distances{point_from_center_ur_quadrant - corner_from_center};
				return utils::math::max(distances, 0.f).lenght + utils::math::min(utils::math::max(distances.x, distances.y), 0.f);
				}
			}
		}

	namespace ab
		{
		namespace common
			{
			}

		namespace point
			{
			}
		}

	namespace circle
		{
		namespace common
			{
			//utils_gpu_available constexpr side                           side_of              (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return side_from_distance_signed                      (a, b); }
			//utils_gpu_available constexpr vec2f                          closest_point_to     (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return closest_from_closest_and_distance              (a, b); }
			//utils_gpu_available constexpr closest_point_with_distance_t  closest_with_distance(const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return closest_with_distance_from_closest_and_distance(a, b); }
			//utils_gpu_available constexpr closest_pair_with_distance_t   closest_pair         (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return closest_pair_from_closest_and_other_closest    (a, b); }
			//utils_gpu_available constexpr vec2f                          vector_to            (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return vector_to_from_closest_pair                    (a, b); }
			//utils_gpu_available constexpr std::optional<vec2f>           intersection_with    (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return intersection_from_intersects_and_closest       (a, b); }
			//utils_gpu_available constexpr bool                           contains             (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return contains_from_side                             (a, b); }
			//utils_gpu_available constexpr bool                           collides_with        (const shape::concepts::circle auto& a, const shape::concepts::any auto& b) noexcept { return other_collides_with                            (a, b); } 
			
			//utils_gpu_available constexpr bool                           intersects           (const concepts::any auto& other) const noexcept { crtp::unimplemented_interface(); return {}; }
			}


		/*#pragma region point
		utils_gpu_available constexpr signed_distance_t              distance(const concepts::point auto& other) const noexcept { return {center_point().distance(other).value - crtp::derived().radius}; }
		utils_gpu_available constexpr vec2f                          closest_point_to(const concepts::point auto& other) const noexcept
			{
			auto vec{center_point().vector_to(other)};
			vec.length = crtp::derived().radius;
			return crtp::derived().center + vec;
			}
		#pragma endregion point

		#pragma region circle
		utils_gpu_available constexpr signed_distance_t              distance(const concepts::circle auto& other) const noexcept { return {center_point().distance(other).value - crtp::derived().radius}; }
		utils_gpu_available constexpr vec2f                          closest_point_to(const concepts::circle auto& other) const noexcept
			{
			auto vec{center_point().vector_to(other)};
			vec.length = crtp::derived().radius;
			return crtp::derived().center + vec;
			}

		utils_gpu_available constexpr side side_of(const concepts::circle auto& other) const noexcept
			{
			const auto centers_distance{center_point().distance(other.center_point())};
			if (centers_distance <= (crtp::derived().radius - other.radius)) { return side::create::outside(); } // other is inside *this
			if (centers_distance <= (other.radius - crtp::derived().radius)) { return side::create::inside(); }
			if (centers_distance < (other.radius + crtp::derived().radius)) { return side::create::coincident(); }
			return side::create::outside();
			}

		utils_gpu_available constexpr std::optional<vec2f> intersection_with(const concepts::circle auto& other) const noexcept
			{
			crtp::unimplemented_interface(); //TODO
			return {};
			}

		#pragma endregion circle

		#pragma region segment
		utils_gpu_available constexpr bool intersects(const concepts::segment auto& other) const noexcept
			{
			if (contains(other.a) && !contains(other.b)) { return true; }
			if (contains(other.b) && !contains(other.a)) { return true; }
			if (!contains(other.b) && !contains(other.a)) { return other.distance(center_point()) <= crtp::derived().radius; }
			return false;
			}
		#pragma endregion segment*/
		}

	using namespace point::common  ;
	using namespace point::point   ;
	using namespace point::circle  ;
	using namespace point::ab      ;
	using namespace point::aabb    ;
	//using namespace point::capsule ;
	using namespace point::polyline;
	//using namespace point::bezier  ;
	using namespace point::polygon ;
	
	using namespace circle::common;

	//using namespace circle  ::point;
	//using namespace line    ::point;
	//using namespace ray     ::point;
	//using namespace segment ::point;
	//using namespace aabb    ::point;
	//using namespace capsule ::point;
	//using namespace polyline::point;
	//using namespace bezier  ::point;
	//using namespace polygon ::point;


	namespace common
		{
		namespace common
			{
			utils_gpu_available constexpr bool collides_with(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept
				{
				return interactions::contains(a, b) || interactions::intersects(a, b);
				}

			utils_gpu_available constexpr vec2f closest_point_to(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept
				{
				return interactions::closest_point_of(b, a);
				}

			//utils_gpu_available constexpr side                          side_of              (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr float                         distance             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			
			//utils_gpu_available constexpr geometry::signed_distance_t   distance_signed      (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			////utils_gpu_available constexpr vec2f                         closest_point_to     (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			utils_gpu_available constexpr closest_point_with_distance_t closest_with_distance(const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			utils_gpu_available constexpr closest_pair_with_distance_t  closest_pair         (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			utils_gpu_available constexpr vec2f                         vector_to            (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr bool                          intersects           (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr std::optional<vec2f>          intersection_with    (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			//utils_gpu_available constexpr bool                          contains             (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			////utils_gpu_available constexpr bool                          collides_with        (const shape::concepts::any auto& a, const shape::concepts::any auto& b) noexcept { utils::concepts::unimplemented_specialization<decltype(a)>(); }
			}

		namespace point
			{
			utils_gpu_available constexpr geometry::side side_of(const shape::concepts::any auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {interactions::distance(a, b)};
				}
			utils_gpu_available constexpr geometry::signed_distance_t distance_signed(const shape::concepts::any auto& a, const shape::concepts::point auto& b) noexcept
				{
				return {interactions::distance(a, b)};
				}
			}
		}
	using namespace common::common;
	using namespace common::point;
	}