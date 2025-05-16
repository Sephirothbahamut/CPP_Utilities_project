#include "../geometry_sdf_and_normal_texture.h"

#include <utils/math/geometry/sdf/dynamic.h>
#include <utils/math/geometry/shape/dynamic.h>

geometry_sdf_and_normal_texture::geometry_sdf_and_normal_texture() noexcept
	{
	// Closed polygon with variable vertices count
	triangle.vertices.storage.emplace_back(50.f, 50.f);

	// Mixed shapes can be initialized with their starting vertex
	mixed.add_segments
		({
		utils::math::vec2f{180.f, 580.f},
		utils::math::vec2f{500.f, 600.f}
		});
	mixed.add_bezier
		({
		utils::math::vec2f{380.f, 670.f},
		utils::math::vec2f{470.f, 820.f}
		});
	mixed.add_bezier
		({
		utils::math::vec2f{470.f, 820.f} + (utils::math::vec2f{470.f, 820.f} - utils::math::vec2f{380.f, 670.f}),
		utils::math::vec2f{295.f, 750.f},
		utils::math::vec2f{190.f, 790.f}
		});

	mixed_inverse.add_segments
		({
		utils::math::vec2f{166.f, 684.f},
		utils::math::vec2f{197.f, 700.f}
		});
	mixed_inverse.add_bezier
		({
		utils::math::vec2f{160.f, 640.f},
		utils::math::vec2f{340.f, 730.f}
		});
	mixed_inverse.add_segment({183.f, 592.f});

	// You can cut away self intersections with the last added element when constructing mixed shapes
	mixed_cut_self_intersections.add_segment_cutting_intersection_with_last_element(utils::math::vec2f{722.f, 784.f});
	mixed_cut_self_intersections.add_bezier_4pt_cutting_intersection_with_last_element
		(
		utils::math::vec2f{777.f, 785.f},
		utils::math::vec2f{692.f, 726.f},
		utils::math::vec2f{658.f, 790.f}
		);

	// Transforming shapes
	polyline  .translate_self({50.f, 50.f});
	triangle  .scale_self    (.5f);
	triangle  .translate_self({122.f, 143.f});
	triangle_b.translate_self({222.f, 143.f});
	

	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(mixed                       )                       >>(mixed                       ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(mixed_cut_self_intersections)                       >>(mixed_cut_self_intersections));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<typename decltype(world_point                 )::const_observer_self_t>>(world_point                 .create_observer()));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<typename decltype(world_aabb                  )::const_observer_self_t>>(world_aabb                  .create_observer()));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<typename decltype(circle                      )::const_observer_self_t>>(circle                      .create_observer()));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<typename decltype(capsule                     )::const_observer_self_t>>(capsule                     .create_observer()));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(polyline                    )                       >>(polyline                    ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(triangle                    )                       >>(triangle                    ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(triangle_b                  )                       >>(triangle_b                  ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(bezier_3_pt                 )                       >>(bezier_3_pt                 ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(bezier_4_pt                 )                       >>(bezier_4_pt                 ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(bezier_loop                 )                       >>(bezier_loop                 ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(quadratic_as_cubic          )                       >>(quadratic_as_cubic          ));
	shapes.emplace_back(utils::make_polymorphic_value<utils::math::geometry::shape::dynamic::details::templated_child<         decltype(mixed_inverse               )                       >>(mixed_inverse               ));
	
	}
