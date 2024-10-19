#include "../geometry_sdf_and_normal_texture.h"

void geometry_sdf_and_normal_texture::gsdf_in_regions() const noexcept
	{
	utils::matrix<utils::math::geometry::sdf::gradient_signed_distance> gradient_signed_distance_field(image_sizes);
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

	bb_wrapper_mixed        .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge_absolute, gradient_signed_distance_field, supersampling);
	bb_wrapper_mixed_inverse.evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge_absolute, gradient_signed_distance_field, supersampling);
	bb_wrapper_world_point  .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_world_aabb   .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_circle       .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_polyline     .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_triangle     .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_triangle_b   .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_bezier_3_pt  .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_bezier_4_pt  .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_bezier_loop  .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_segments_1   .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	bb_wrapper_segments_2   .evaluate_gsdf(camera_transform, &utils::math::geometry::sdf::gradient_signed_distance::merge         , gradient_signed_distance_field, supersampling);
	
	const auto rendered_image{debug_renderer.render({}, gradient_signed_distance_field, supersampling)};
	
	auto elapsed{clock.get_elapsed()};
	std::cout << "gsdf_in_regions: " << elapsed.count() << std::endl;
	
	utils::graphics::image::save_to_file(rendered_image, "./output/gsdf_in_regions.png");
	}
