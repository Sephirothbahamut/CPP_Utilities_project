#include "../geometry_sdf_and_normal_texture.h"

void geometry_sdf_and_normal_texture::full_manual() const noexcept
	{
	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};
	//utils::math::geometry::shape::aabb shape_padding{utils::math::geometry::shape::aabb::create::infinite()};

	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed        {mixed        , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed_inverse{mixed_inverse, shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed_cut_self_intersections{mixed_cut_self_intersections, shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_world_point  {world_point  , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_world_aabb   {world_aabb   , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_circle       {circle       , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_polyline     {polyline     , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_triangle     {triangle     , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_triangle_b   {triangle_b   , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_bezier_3_pt  {bezier_3_pt  , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_bezier_4_pt  {bezier_4_pt  , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_bezier_loop  {bezier_loop  , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_segments_1   {segments[1]  , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_segments_2   {segments[2]  , shape_padding};

	const utils::math::geometry::shape::bezier<4> quadratic_as_cubic
		{
		utils::math::vec2f{2.5f     + 195.f, 107.5625f  },
		utils::math::vec2f{2.5f     + 195.f, 103.979172f},
		utils::math::vec2f{3.09375f + 195.f, 100.84375f },
		utils::math::vec2f{4.28125f + 195.f, 98.15625f  },
		};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_quadratic_as_cubic{quadratic_as_cubic, shape_padding};

	utils::matrix<utils::graphics::colour::rgba_f> image_lit (image_sizes);
	utils::matrix<utils::graphics::colour::rgba_f> image_gsdf(image_sizes);

	std::ranges::iota_view indices(size_t{0}, image_lit.size());

	utils::clock<std::chrono::high_resolution_clock, float> clock;

	//*
	std::for_each(std::execution::par, indices.begin(), indices.end(), [&](size_t index)
	/*/
	std::for_each(indices.begin(), indices.end(), [&](size_t index)
	/**/
		{
		const utils::math::vec2s coords_indices{image_sizes.index_to_coords(index)};
		const utils::math::vec2f coords_f
			{
			utils::math::vec2f
				{
				static_cast<float>(coords_indices.x()),
				static_cast<float>(coords_indices.y())
				}
			.transform(camera_transform)
			.scale    (1.f / supersampling)
			};


		
		const auto evaluated_mixed             {bb_wrapper_mixed             .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_mixed_inverse     {bb_wrapper_mixed_inverse     .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_mixed_cut_self_intersections{bb_wrapper_mixed_cut_self_intersections.evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_world_point       {bb_wrapper_world_point       .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_world_aabb        {bb_wrapper_world_aabb        .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_circle            {bb_wrapper_circle            .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_polyline          {bb_wrapper_polyline          .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_triangle          {bb_wrapper_triangle          .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_triangle_b        {bb_wrapper_triangle_b        .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_bezier_3_pt       {bb_wrapper_bezier_3_pt       .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_bezier_4_pt       {bb_wrapper_bezier_4_pt       .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_bezier_loop       {bb_wrapper_bezier_loop       .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_segments_1        {bb_wrapper_segments_1        .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_segments_2        {bb_wrapper_segments_2        .evaluate_gradient_signed_distance(coords_f)};
		const auto evaluated_quadratic_as_cubic{bb_wrapper_quadratic_as_cubic.evaluate_gradient_signed_distance(coords_f)};
		
		
		utils::math::geometry::sdf::gradient_signed_distance gdist;
		
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge_absolute(gdist, evaluated_mixed             );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge_absolute(gdist, evaluated_mixed_inverse     );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_mixed_cut_self_intersections);
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_world_point       );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_world_aabb        );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_circle            );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_polyline          );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_triangle          );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_triangle_b        );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_bezier_3_pt       );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_bezier_4_pt       );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_bezier_loop       );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_segments_1        );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_segments_2        );
		gdist = utils::math::geometry::sdf::gradient_signed_distance::merge         (gdist, evaluated_quadratic_as_cubic);
		
		const auto sample_gdist{utils::graphics::sdf::debug_sample_gradient_sdf(gdist            )};
		const auto sample_lit  {gsdf_helpers::apply_light            (coords_f, gdist, light, 8.f)};

		if (true)
			{
			image_gsdf[index] = sample_gdist;
			}
		if (true)
			{
			image_lit[index] = sample_lit;
			}
		});
	
	const auto elapsed{clock.get_elapsed()};
	std::cout << "full_manual: " << elapsed.count() << std::endl;

	utils::graphics::image::save_to_file(image_gsdf    , "./output/full_manual.png");
	utils::graphics::image::save_to_file(image_lit     , "./output/full_manual_lit.png" );
	}
