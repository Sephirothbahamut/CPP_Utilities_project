#include "../geometry_sdf_and_normal_texture.h"

#include <utils/algorithm/for_each.h>

void geometry_sdf_and_normal_texture::full_manual() const noexcept
	{

	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};
	utils::math::geometry::shape::aabb bb_mixed                       {mixed                       .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_mixed_inverse               {mixed_inverse               .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_mixed_cut_self_intersections{mixed_cut_self_intersections.bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_world_point                 {world_point                 .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_world_aabb                  {world_aabb                  .bounding_box() + shape_padding};
	utils::math::geometry::shape::aabb bb_circle                      {circle                      .bounding_box() + shape_padding};
	
	#ifndef __INTELLISENSE__
	utils::math::geometry::shape::aabb bb_capsule{capsule.bounding_box() + shape_padding};
	#endif
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
	const utils::math::geometry::shape::bezier<4> quadratic_as_cubic
		{
		utils::math::vec2f{2.5f     + 195.f, 107.5625f  },
		utils::math::vec2f{2.5f     + 195.f, 103.979172f},
		utils::math::vec2f{3.09375f + 195.f, 100.84375f },
		utils::math::vec2f{4.28125f + 195.f, 98.15625f  },
		};
	utils::math::geometry::shape::aabb bb_quadratic_as_cubic           {quadratic_as_cubic.bounding_box() + shape_padding};
	#endif

	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
	utils::matrix<utils::graphics::colour::rgba_f> image_dsdf(image_sizes);

	std::ranges::iota_view indices(size_t{0}, image_lit.size());

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	utils::algorithm::for_each::coords<true>(image_lit.sizes(), [&](const utils::algorithm::for_each::coords_callback_params& params)
		{
		const utils::math::vec2f coords_f
			{
			utils::math::vec2f
				{
				static_cast<float>(params.indices.x()),
				static_cast<float>(params.indices.y())
				}
			.transform(camera_transform)
			.scale    (1.f / supersampling)
			};

		utils::math::geometry::sdf::direction_signed_distance gdist;

		#ifndef __INTELLISENSE__ //intellisense dies at ".direction_signed_distance()" and I can't figure out why *shrugs*
		if (bb_mixed                       .contains(coords_f)) { gdist.merge_self_absolute(mixed                       .sdf(coords_f).direction_signed_distance()); }
		if (bb_mixed_inverse               .contains(coords_f)) { gdist.merge_self_absolute(mixed_inverse               .sdf(coords_f).direction_signed_distance()); }
		if (bb_mixed_cut_self_intersections.contains(coords_f)) { gdist.merge_self         (mixed_cut_self_intersections.sdf(coords_f).direction_signed_distance()); }
		if (bb_world_point                 .contains(coords_f)) { gdist.merge_self         (world_point                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_world_aabb                  .contains(coords_f)) { gdist.merge_self         (world_aabb                  .sdf(coords_f).direction_signed_distance()); }
		if (bb_circle                      .contains(coords_f)) { gdist.merge_self         (circle                      .sdf(coords_f).direction_signed_distance()); }
		if (bb_capsule                     .contains(coords_f)) { gdist.merge_self         (capsule                     .sdf(coords_f).direction_signed_distance()); }
		if (bb_polyline                    .contains(coords_f)) { gdist.merge_self         (polyline                    .sdf(coords_f).direction_signed_distance()); }
		if (bb_triangle                    .contains(coords_f)) { gdist.merge_self         (triangle                    .sdf(coords_f).direction_signed_distance()); }
		if (bb_triangle_b                  .contains(coords_f)) { gdist.merge_self         (triangle_b                  .sdf(coords_f).direction_signed_distance()); }
		if (bb_bezier_3_pt                 .contains(coords_f)) { gdist.merge_self         (bezier_3_pt                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_bezier_4_pt                 .contains(coords_f)) { gdist.merge_self         (bezier_4_pt                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_bezier_loop                 .contains(coords_f)) { gdist.merge_self         (bezier_loop                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_segments_1                  .contains(coords_f)) { gdist.merge_self         (segments[1]                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_segments_2                  .contains(coords_f)) { gdist.merge_self         (segments[2]                 .sdf(coords_f).direction_signed_distance()); }
		if (bb_quadratic_as_cubic          .contains(coords_f)) { gdist.merge_self         (quadratic_as_cubic          .sdf(coords_f).direction_signed_distance()); }
		#endif
		
		const auto sample_gdist{utils::graphics::sdf::debug_sample_direction_sdf(gdist            )};
		const auto sample_lit  {dsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};

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
	std::cout << "full_manual: " << elapsed.count() << std::endl;

	utils::graphics::image::save_to_file(image_dsdf    , "./output/full_manual.png");
	utils::graphics::image::save_to_file(image_lit     , "./output/full_manual_lit.png" );
	}
