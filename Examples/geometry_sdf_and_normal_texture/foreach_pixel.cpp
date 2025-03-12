#include "../geometry_sdf_and_normal_texture.h"

void geometry_sdf_and_normal_texture::foreach_pixel() const noexcept
	{
	utils::matrix<utils::math::geometry::sdf::direction_signed_distance> direction_signed_distance_field(image_sizes);
	utils::graphics::sdf::debug debug_renderer;

	utils::math::geometry::shape::aabb shape_padding{-32.f, -32.f, 32.f, 32.f};
	//utils::math::geometry::shape::aabb shape_padding{utils::math::geometry::shape::aabb::create::infinite()};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed        {mixed        , shape_padding};
	utils::graphics::sdf::shape_bounding_box_wrapper bb_wrapper_mixed_inverse{mixed_inverse, shape_padding};
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


	utils::clock<std::chrono::high_resolution_clock, float> clock;

	

	const auto rendered_image{debug_renderer.render<true>(camera_transform, image_sizes, [&](const utils::math::vec2f coords_f)
		{
		const auto evaluated_mixed        {bb_wrapper_mixed        .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_mixed_inverse{bb_wrapper_mixed_inverse.evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_world_point  {bb_wrapper_world_point  .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_world_aabb   {bb_wrapper_world_aabb   .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_circle       {bb_wrapper_circle       .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_polyline     {bb_wrapper_polyline     .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_triangle     {bb_wrapper_triangle     .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_triangle_b   {bb_wrapper_triangle_b   .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_bezier_3_pt  {bb_wrapper_bezier_3_pt  .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_bezier_4_pt  {bb_wrapper_bezier_4_pt  .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_bezier_loop  {bb_wrapper_bezier_loop  .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_segments_1   {bb_wrapper_segments_1   .evaluate_direction_signed_distance(coords_f)};
		const auto evaluated_segments_2   {bb_wrapper_segments_2   .evaluate_direction_signed_distance(coords_f)};

		utils::math::geometry::sdf::direction_signed_distance gdist;
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge_absolute(gdist, evaluated_mixed        );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge_absolute(gdist, evaluated_mixed_inverse);
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_world_point  );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_world_aabb   );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_circle       );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_polyline     );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_triangle     );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_triangle_b   );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_bezier_3_pt  );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_bezier_4_pt  );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_bezier_loop  );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_segments_1   );
		gdist = utils::math::geometry::sdf::direction_signed_distance::merge         (gdist, evaluated_segments_2   );

		return gdist;
		}, supersampling)};
	
	auto elapsed{clock.get_elapsed()};
	std::cout << "foreach_pixel: " << elapsed.count() << std::endl;
	
	utils::graphics::image::save_to_file(rendered_image, "./output/foreach_pixel.png");
	}
