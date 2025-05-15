#include "../geometry_sdf_and_normal_texture.h"

#include <utils/algorithm/for_each.h>

#include "../log.h"

void geometry_sdf_and_normal_texture::static_for_each_pixel() const noexcept
	{
	const auto _{logger.section("Static foreach pixel")};
	logger.flush();

	utils::math::geometry::shape::aabb bb_mixed                       {mixed                       .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_mixed_inverse               {mixed_inverse               .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_mixed_cut_self_intersections{mixed_cut_self_intersections.bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_world_point                 {world_point                 .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_world_aabb                  {world_aabb                  .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_circle                      {circle                      .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_capsule                     {capsule                     .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_polyline                    {polyline                    .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_triangle                    {triangle                    .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_triangle_b                  {triangle_b                  .bounding_box() + shape_padding};
	#ifndef __INTELLISENSE__
	utils::math::geometry::shape::aabb bb_bezier_3_pt                 {bezier_3_pt                 .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_bezier_4_pt                 {bezier_4_pt                 .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_bezier_loop                 {bezier_loop                 .bounding_box() + shape_padding};
	#endif
	utils::math::geometry::shape::aabb bb_segments_1                  {segments[1]                 .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_segments_2                  {segments[2]                 .bounding_box() + shape_padding};

	#ifndef __INTELLISENSE__
	utils::math::geometry::shape::aabb bb_quadratic_as_cubic          {quadratic_as_cubic          .bounding_box() + shape_padding};
	#endif

	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
	utils::matrix<utils::graphics::colour::rgba_f> image_dsdf(image_sizes);

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	const auto for_each{utils::algorithm::for_each::in_sizes(image_sizes).scale(supersampling)};
	for_each.execute<true>([&](const decltype(for_each)::callback_params& params)
		{
		const utils::math::vec2f coords_f{params.global.floating};

		utils::math::geometry::sdf::direction_signed_distance directional_distance;

		#ifndef __INTELLISENSE__ //intellisense dies at ".direction_signed_distance()" and I can't figure out why *shrugs*
		if (bb_mixed                       .contains(coords_f)) { directional_distance.merge_self_absolute(mixed                       .sdf(coords_f).direction_signed_distance()); }
		if (bb_mixed_inverse               .contains(coords_f)) { directional_distance.merge_self_absolute(mixed_inverse               .sdf(coords_f).direction_signed_distance()); }
		if (bb_mixed_cut_self_intersections.contains(coords_f)) { directional_distance.merge_self         (mixed_cut_self_intersections.sdf(coords_f).direction_signed_distance()); }
		if (bb_world_point                 .contains(coords_f)) { directional_distance.merge_self         (world_point                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_world_aabb                  .contains(coords_f)) { directional_distance.merge_self         (world_aabb                  .sdf(coords_f).direction_signed_distance()); }
		if (bb_circle                      .contains(coords_f)) { directional_distance.merge_self         (circle                      .sdf(coords_f).direction_signed_distance()); }
		if (bb_capsule                     .contains(coords_f)) { directional_distance.merge_self         (capsule                     .sdf(coords_f).direction_signed_distance()); }
		if (bb_polyline                    .contains(coords_f)) { directional_distance.merge_self         (polyline                    .sdf(coords_f).direction_signed_distance()); }
		if (bb_triangle                    .contains(coords_f)) { directional_distance.merge_self         (triangle                    .sdf(coords_f).direction_signed_distance()); }
		if (bb_triangle_b                  .contains(coords_f)) { directional_distance.merge_self         (triangle_b                  .sdf(coords_f).direction_signed_distance()); }
		if (bb_bezier_3_pt                 .contains(coords_f)) { directional_distance.merge_self         (bezier_3_pt                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_bezier_4_pt                 .contains(coords_f)) { directional_distance.merge_self         (bezier_4_pt                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_bezier_loop                 .contains(coords_f)) { directional_distance.merge_self         (bezier_loop                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_segments_1                  .contains(coords_f)) { directional_distance.merge_self         (segments[1]                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_segments_2                  .contains(coords_f)) { directional_distance.merge_self         (segments[2]                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_quadratic_as_cubic          .contains(coords_f)) { directional_distance.merge_self         (quadratic_as_cubic          .sdf(coords_f).direction_signed_distance()); }
		#endif
		
		const auto sample_gdist{utils::graphics::sdf::debug_sample_direction_sdf(directional_distance)};
		const auto sample_lit  {dsdf_helpers::apply_light(coords_f, directional_distance, light, 8.f)};

		if (true)
			{
			image_dsdf[params.index] = sample_gdist;
			}
		if (true)
			{
			image_lit[params.index] = sample_lit;
			}
		});
	
	const auto elapsed{clock.get_elapsed()};
	logger.log("Time: " + std::to_string(elapsed.count()));

	utils::graphics::image::save_to_file(image_dsdf, "./output/static_foreach_pixel_dsdf.png");
	utils::graphics::image::save_to_file(image_lit , "./output/static_foreach_pixel_lit.png" );
	}
